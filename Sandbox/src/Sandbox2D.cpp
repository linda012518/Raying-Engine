#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), _cameraCtrl(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	_texture = Raying::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Raying::Timestep ts)
{
	Raying_Profile_FUNCTION();
	// Update
	{
		Raying_Profile_SCOPE("CameraController::OnUpdate");
		_cameraCtrl.OnUpdate(ts);
	}

	// Render
	{
		Raying_Profile_SCOPE("Renderer Prep");
		Raying::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Raying::RendererCommand::Clear();
	}

	{
		Raying_Profile_SCOPE("Renderer Draw");
		Raying::Renderer2D::BeginScene(_cameraCtrl.GetCamera());
		Raying::Renderer2D::DrawQuad({ -0.8f,  0.0f }, { 0.8f, 0.8f }, _color);
		Raying::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.2f, 0.6f }, { 0.1f, 0.6f, 0.1f, 1 });
		Raying::Renderer2D::DrawQuad({ -0.5f, 0.5f, -0.1f }, { 10.0f, 10.0f }, _texture);
		Raying::Renderer2D::EndScene();
	}


}

void Sandbox2D::OnImGuiRender()
{
	Raying_Profile_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(_color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Raying::Event & e)
{
	_cameraCtrl.OnEvent(e);
}

