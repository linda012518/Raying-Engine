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

	// Init here
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
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
	Raying::Renderer2D::ResetStats();
	{
		Raying_Profile_SCOPE("Renderer Prep");
		Raying::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Raying::RendererCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		Raying_Profile_SCOPE("Renderer Draw");
		Raying::Renderer2D::BeginScene(_cameraCtrl.GetCamera());
		Raying::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Raying::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Raying::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Raying::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, _texture, 10.0f);
		Raying::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, _texture, 20.0f);
		Raying::Renderer2D::EndScene();

		Raying::Renderer2D::BeginScene(_cameraCtrl.GetCamera());
		
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				Raying::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, _color);
			}
		}

		Raying::Renderer2D::EndScene();
	}


	if (Raying::Input::IsMouseButtonPressed((Raying::MouseCode)RAYING_MOUSE_BUTTON_LEFT))
	{
		auto[x, y] = Raying::Input::GetMousePosition();
		auto width = Raying::Application::Get().GetWindow().GetWidth();
		auto height = Raying::Application::Get().GetWindow().GetHeight();

		auto bounds = _cameraCtrl.GetBounds();
		auto pos = _cameraCtrl.GetCamera().GetPostion();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(_cameraCtrl.GetCamera());
}

void Sandbox2D::OnImGuiRender()
{
	Raying_Profile_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Raying::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats : ");
	ImGui::Text("Draw Calls : %d", stats.DrawCalls);
	ImGui::Text("Quad : %d", stats.QuadCount);
	ImGui::Text("Vertices : %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices : %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(_color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Raying::Event & e)
{
	_cameraCtrl.OnEvent(e);
}

