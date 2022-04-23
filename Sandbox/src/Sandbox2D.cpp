#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), _cameraCtrl(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Raying::Timestep ts)
{
	// Update
	_cameraCtrl.OnUpdate(ts);

	// Render
	Raying::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Raying::RendererCommand::Clear();

	Raying::Renderer2D::BeginScene(_cameraCtrl.GetCamera());
	Raying::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f,1.0f }, _color);
	Raying::Renderer2D::EndScene();

}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(_color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Raying::Event & e)
{
	_cameraCtrl.OnEvent(e);
}

