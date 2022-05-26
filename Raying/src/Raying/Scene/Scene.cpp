#include "hzpch.h"
#include "Scene.h"

#include "Comonents.h"
#include "Raying/Renderer/Renderer2D.h"

#include "Entity.h"

namespace Raying {

	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE
		entt::entity entity = _registry.create();
		_registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		_registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();


		if (_registry.has<TransformComponent>(entity))
			TransformComponent& transform = _registry.get<TransformComponent>(entity);


		auto view = _registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = _registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto&[transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}
#endif

	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { _registry.create(), this };

		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();

		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		_registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->_entity = Entity(entity, this);

				nsc.Instance->OnCreate();
			}

			nsc.Instance->OnUpdate(ts);
		});

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		{
			auto view = _registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		_width = width;
		_height = height;

		auto view = _registry.view<CameraComponent>();

		for (auto entity : view)
		{
			auto& component = view.get<CameraComponent>(entity);
			if (!component.FixedAspectRatio)
				component.Camera.SetViewportSize(_width, _height);
		}

	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T & component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(_width, _height);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}

}
