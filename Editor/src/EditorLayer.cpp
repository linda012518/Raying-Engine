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
	}

	void EditorLayer::OnDetach()
	{
		Raying_Profile_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		Raying_Profile_FUNCTION();

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

		{
			static float rotation = 0.0f;
			rotation += ts * 50.0f;

			Raying_Profile_SCOPE("Renderer Draw");
			Renderer2D::BeginScene(_cameraCtrl.GetCamera());
			Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
			Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
			Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
			Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, _texture, 10.0f);
			Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, _texture, 20.0f);
			Renderer2D::EndScene();

			Renderer2D::BeginScene(_cameraCtrl.GetCamera());

			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				for (float y = -5.0f; y < 5.0f; y += 0.5f)
				{
					Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, _color);
				}
			}

			Renderer2D::EndScene();
			_fbo->Unbind();
		}
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

		ImGui::Begin("Settings");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(_color));

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		_viewportFocused = ImGui::IsWindowFocused();
		_viewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!_viewportFocused || !_viewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (_viewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			_fbo->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			_cameraCtrl.OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}
		uint32_t textureID = _fbo->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ _viewportSize.x, _viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

	}

	void EditorLayer::OnEvent(Event & e)
	{
		_cameraCtrl.OnEvent(e);
	}

}