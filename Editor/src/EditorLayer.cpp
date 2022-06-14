#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Raying/Scene/SceneSerializer.h"
#include "Raying/Utils/PlatformUtils.h"
#include "Raying/Math/Math.h"
#include "ImGuizmo/ImGuizmo.h"

namespace Raying {

	extern const std::filesystem::path _assetPath;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), _cameraCtrl(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		Raying_Profile_FUNCTION();

		_texture = Texture2D::Create("assets/textures/Checkerboard.png");
		_iconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
		_iconStop = Texture2D::Create("Resources/Icons/StopButton.png");

		FramebufferSpecification spec;
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		spec.Width = 1280;
		spec.Height = 720;

		_fbo = Framebuffer::Create(spec);

		_activeScene = CreateRef<Scene>();

		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(_activeScene);
			serializer.DeSerialize(sceneFilePath);
		}

		_editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0
		auto square = _activeScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 0.3f, 0.0f, 1.0f));
		_squareEntity = square;

		_cameraEntity = _activeScene->CreateEntity("Camera A");
		_cameraEntity.AddComponent<CameraComponent>();

		_secondCameraEntity = _activeScene->CreateEntity("Camera B");
		auto& cc = _secondCameraEntity.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			virtual void OnCreate() override
			{
				auto& position = GetComponent<TransformComponent>().Position;
				position.x = rand() % 10 - 5.0f;
			}

			virtual void OnDestroy() override
			{
			}

			virtual void OnUpdate(Timestep ts) override
			{
				auto& position = GetComponent<TransformComponent>().Position;
				float speed = 5.0f;

				if (Input::IsKeyPressed(Key::A))
					position.x -= speed * ts;
				if (Input::IsKeyPressed(Key::D))
					position.x += speed * ts;
				if (Input::IsKeyPressed(Key::W))
					position.y += speed * ts;
				if (Input::IsKeyPressed(Key::S))
					position.y -= speed * ts;
			}
		};

		_cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		_secondCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

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

			_editorCamera.SetViewportSize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
		}

		// Render
		Renderer2D::ResetStats();
		{
			Raying_Profile_SCOPE("Renderer Prep");
			_fbo->Bind();
			RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RendererCommand::Clear();

			_fbo->ClearAttachment(1, -1);
		}

		
		switch (_sceneState)
		{
			case SceneState::Edit:
			{
				if (_viewportFocused)
					_cameraCtrl.OnUpdate(ts);

				_editorCamera.OnUpdate(ts);
				_activeScene->OnUpdateEditor(ts, _editorCamera);
				break;
			}
			case SceneState::Play:
			{
				_activeScene->OnUpdateRuntime(ts);
				break;
			}
		}

		auto[mx, my] = ImGui::GetMousePos();
		mx -= _viewportBounds[0].x;
		my -= _viewportBounds[0].y;
		//Raying_Core_Trace("Pixed Data {0}，{1}", mx, my);
		glm::vec2 viewportSize = _viewportBounds[1] - _viewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = mx;
		int mouseY = my;
		if (mouseX > 0 && mouseY > 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixedData = _fbo->ReadPixed(1, mouseX, mouseY);
			_hoverdEntity = pixedData == -1 ? Entity() : Entity((entt::entity)pixedData, _activeScene.get());
		}

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

		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save...", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		_sceneHierarchyPanel.OnImGuiRender();
		_contentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		std::string name = "None";
		if (_hoverdEntity)
			name = _hoverdEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hoverd Entity: %s", name.c_str());

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		//auto viewportOffset = ImGui::GetCursorPos();//Viewport 窗口偏移 包含 tab栏
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		_viewportFocused = ImGui::IsWindowFocused();
		_viewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!_viewportFocused && !_viewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = _fbo->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ _viewportSize.x, _viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content_Browser_Item"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(_assetPath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		//auto windowSize = ImGui::GetWindowSize();//Viewport的大小
		//ImVec2 minBound = ImGui::GetWindowPos();//Viewport全屏位置
		//minBound.x += viewportOffset.x;
		//minBound.y += viewportOffset.y;

		//ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		//_viewportBounds[0] = { minBound.x, minBound.y };
		//_viewportBounds[1] = { maxBound.x, maxBound.y };

		Entity selectedEntity = _sceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && _gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float width = (float)ImGui::GetWindowWidth();
			float height = (float)ImGui::GetWindowHeight();

			ImGuizmo::SetRect(_viewportBounds[0].x, _viewportBounds[0].y, _viewportBounds[1].x - _viewportBounds[0].x, _viewportBounds[1].y - _viewportBounds[0].y);

			//auto cameraEntity = _activeScene->GetPrimaryCameraEntity();
			//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			//const glm::mat4& cameraProjection = camera.GetProjection();
			//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			const glm::mat4& cameraProjection = _editorCamera.GetProjection();
			glm::mat4 cameraView = _editorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (_gizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Position = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}

			
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();

	}

	void EditorLayer::OnEvent(Event & e)
	{
		_cameraCtrl.OnEvent(e);
		_editorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(Raying_Bind_Event_Fn(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(Raying_Bind_Event_Fn(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent & e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();
				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();
				break;
			}
			case Key::S:
			{
				if (control && shift)
					SaveSceneAs();
				else if (control)
				{
					if(_currentScenePath.empty())
						SaveSceneAs();
					else
						SaveScene();
				}
				break;
			}

			// Gizmos
			case Key::Q:
				if (!ImGuizmo::IsUsing())
					_gizmoType = -1;
				break;
			case Key::W:
				if (!ImGuizmo::IsUsing())
					_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::E:
				if (!ImGuizmo::IsUsing())
					_gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::R:
				if (!ImGuizmo::IsUsing())
					_gizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}

		return true;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent & e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (_viewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				_sceneHierarchyPanel.SetSelectedEntity(_hoverdEntity);
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		_activeScene = CreateRef<Scene>();
		_activeScene->OnViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
		_sceneHierarchyPanel.SetContext(_activeScene);

		_currentScenePath = std::string();
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Raying Scene (*.linda)\0*.linda\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path & path)
	{
		if (path.extension().string() != ".linda")
		{
			Raying_Warn("Could not load {0} -- not a scene file!", path.filename().string());
			return;
		}
		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serialize(newScene);
		if (serialize.DeSerialize(path.string()))
		{
			_activeScene = newScene;
			_activeScene->OnViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
			_sceneHierarchyPanel.SetContext(_activeScene);
		}

		_currentScenePath = path.string();
	}

	void EditorLayer::SaveScene()
	{
		SceneSerializer serialize(_activeScene);
		serialize.Serialize(_currentScenePath);
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Raying Scene (*.linda)\0*.linda\0");
		if (!filepath.empty())
		{
			//int ret = filepath.find(".linda");
			//if (ret == -1)
			//{
			//	filepath += ".linda";
			//}
			SceneSerializer serialize(_activeScene);
			serialize.Serialize(filepath);
			_currentScenePath = filepath;
		}
	}

	void EditorLayer::OnScenePlay()
	{
		_sceneState = SceneState::Play;
		_activeScene->OnRuntimeStart();
	}

	void EditorLayer::OnSceneStop()
	{
		_sceneState = SceneState::Edit;
		_activeScene->OnRuntimeStop();
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Ref<Texture2D> icon = _sceneState == SceneState::Edit ? _iconPlay : _iconStop;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (_sceneState == SceneState::Edit)
				OnScenePlay();
			else if (_sceneState == SceneState::Play)
				OnSceneStop();
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

}
