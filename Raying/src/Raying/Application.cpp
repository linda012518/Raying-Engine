#include "hzpch.h"
#include "Application.h"
#include "Log.h"
#include "Renderer/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Raying
{
	#define Bind_Event_Fn(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::_instance = nullptr;

	Application::Application()
	{
		Raying_Core_Assert(!_instance, "Application alread exists!");
		_instance = this;

		_window = std::unique_ptr<Window>(Window::Create());
		_window->SetEventCallback(Bind_Event_Fn(OnEvent));
		_window->SetVSync(false);

		Renderer::Init();

		_imguiLayer = new ImGuiLayer();
		PushLayer(_imguiLayer);
	}

	void Application::PushLayer(Layer* layer)
	{
		_layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		_layerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(Bind_Event_Fn(OnWindowClose));

		for (auto itr = _layerStack.end(); itr != _layerStack.begin(); )
		{
			(*--itr)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (_running)
		{			
			float time = (float)glfwGetTime();
			Timestep ts = time - _lastFrameTime;
			_lastFrameTime = time;

			for (Layer* layer : _layerStack)
				layer->OnUpdate(ts);

			_imguiLayer->Begin();
			for (Layer* layer : _layerStack)
				layer->OnImGuiRender();
			_imguiLayer->End();

			_window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_running = false;
		return true;
	}

}
