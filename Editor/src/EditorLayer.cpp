#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Raying {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), _cameraCtrl(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		Raying_Profile_FUNCTION();

		_texture = Texture2D::Create("assets/textures/Checkerboard.png");

		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;

		_fbo = Framebuffer::Create(spec);

		_activeScene = CreateRef<Scene>();

		auto square = _activeScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 0.3f, 0.0f, 1.0f));
		_squareEntity = square;

		_cameraEntity = _activeScene->CreateEntity("Camera Entity");
		_cameraEntity.AddComponent<CameraComponent>();

		_secondCameraEntity = _activeScene->CreateEntity("Clip-Space Entity");
		auto& cc = _secondCameraEntity.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			virtual void OnCreate() override
			{
				auto& transform = GetComponent<TransformComponent>().Transform;
				transform[3][0] = rand() % 10 - 5.0f;
			}

			virtual void OnDestroy() override
			{
			}

			virtual void OnUpdate(Timestep ts) override
			{
				auto& transform = GetComponent<TransformComponent>().Transform;
				float speed = 5.0f;

				if (Input::IsKeyPressed(Key::A))
					transform[3][0] -= speed * ts;
				if (Input::IsKeyPressed(Key::D))
					transform[3][0] += speed * ts;
				if (Input::IsKeyPressed(Key::W))
					transform[3][1] += speed * ts;
				if (Input::IsKeyPressed(Key::S))
					transform[3][1] -= speed * ts;
			}
		};

		_cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		_secondCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		_sceneHierarchyPanel.SetContext(_activeScene);
	}

	void EditorLayer::OnDetach()
	{
		Raying_Profile_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		Raying_Profile_FUNCTION();

		FramebufferSpecification spec = _fbo->GetSpecification();
		if (_viewportSize.x > 0.0f && _viewportSize.y > 0.0f && (spec.Width != _viewportSize.x || spec.Height != _viewportSize.y))
		{
			_fbo->Resize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
			_cameraCtrl.OnResize(_viewportSize.x, _viewportSize.y);

			_activeScene->OnViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
		}

		if (_viewportFocused)
			_cameraCtrl.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		{
			Raying_Profile_SCOPE("Renderer Prep");
			_fbo->Bind();
			RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RendererCommand::Clear();
		}

		_activeScene->OnUpdate(ts);

		_fbo->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		Raying_Profile_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		_sceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Settings");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		if (_squareEntity)
		{
			ImGui::Separator();
			auto& tag = _squareEntity.GetComponent<TagComponent>().Tag;

			ImGui::Text("%s", tag.c_str());

			auto& color = _squareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(color));
			ImGui::Separator();
		}

		ImGui::DragFloat3("Camera Transform", glm::value_ptr(_cameraEntity.GetComponent<TransformComponent>().Transform[3]));
		if (ImGui::Checkbox("Camera A", &_primoryCamera))
		{
			_cameraEntity.GetComponent<CameraComponent>().Primary = _primoryCamera;
			_secondCameraEntity.GetComponent<CameraComponent>().Primary = !_primoryCamera;
		}

		{
			auto& camera = _secondCameraEntity.GetComponent<CameraComponent>().Camera;
			float size = camera.GetOrthographicSize();
			if (ImGui::DragFloat("Second Camera Ortho Size", &size))
				camera.SetOrthographicSize(size);
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		_viewportFocused = ImGui::IsWindowFocused();
		_viewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!_viewportFocused || !_viewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = _fbo->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ _viewportSize.x, _viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

	}

	void EditorLayer::OnEvent(Event & e)
	{
		_cameraCtrl.OnEvent(e);
	}

}
