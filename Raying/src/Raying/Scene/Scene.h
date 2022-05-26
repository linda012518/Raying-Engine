#pragma once

#include "entt.hpp"

#include "Raying/Core/Timestep.h"

namespace Raying {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		entt::registry& Reg() { return _registry; }

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry _registry;
		uint32_t _width = 0, _height = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};

}
