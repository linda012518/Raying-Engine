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
	Raying_Profile_FUNCTION();

	_texture = Raying::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	Raying_Profile_FUNCTION();
}

void Sandbox2D::OnUpdate(Raying::Timestep ts)
{
	Raying_Profile_FUNCTION();

	_cameraCtrl.OnUpdate(ts);

	// Render
	{
		Raying_Profile_SCOPE("Renderer Prep");
		Raying::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Raying::RendererCommand::Clear();
	}

	{
		Raying_Profile_SCOPE("Renderer Draw");
		Raying::Renderer2D::BeginScene(_cameraCtrl.GetCamera());
		Raying::Renderer2D::DrawRotatedQuad({ -0.8f,  0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), _color);
		Raying::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, _color);
		Raying::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Raying::Renderer2D::DrawQuad({ -5.0f, -5.0f, -0.1f }, { 10.0f, 10.0f }, _texture, 10.0f);
		Raying::Renderer2D::DrawQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, _texture, 20.0f);
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

