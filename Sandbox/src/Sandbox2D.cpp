#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: _name(name), _func(func), _stopped(false)
	{
		_startTimePoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (false == _stopped)
		{
			Stop();
		}
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimePoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		_stopped = true;

		float duration = (end - start) * 0.001f;
		_func({ _name, duration });
	}

private:
	const char* _name;
	Fn _func;
	std::chrono::time_point<std::chrono::steady_clock> _startTimePoint;
	bool _stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { _profileResults.push_back(profileResult); })

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
	PROFILE_SCOPE("Sandbox2D::OnUpdate");
	// Update
	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		_cameraCtrl.OnUpdate(ts);
	}

	// Render
	{
		PROFILE_SCOPE("Renderer Prep");
		Raying::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Raying::RendererCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		Raying::Renderer2D::BeginScene(_cameraCtrl.GetCamera());
		Raying::Renderer2D::DrawQuad({ -0.8f,  0.0f }, { 0.8f, 0.8f }, _color);
		Raying::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.2f, 0.6f }, { 0.1f, 0.6f, 0.1f, 1 });
		Raying::Renderer2D::DrawQuad({ -0.5f, 0.5f, -0.1f }, { 10.0f, 10.0f }, _texture);
		Raying::Renderer2D::EndScene();
	}


}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(_color));

	for (auto& result : _profileResults)
	{
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	_profileResults.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(Raying::Event & e)
{
	_cameraCtrl.OnEvent(e);
}

