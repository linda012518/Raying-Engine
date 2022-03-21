#include "hzpch.h"
#include "Application.h"

#include "Log.h"

#include <GLFW/glfw3.h>

namespace Raying
{
	#define Bind_Event_Fn(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		_window = std::unique_ptr<Window>(Window::Create());
		_window->SetEventCallback(Bind_Event_Fn(OnEvent));
	}

	Application::~Application()
	{

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
			glClearColor(0, 0.3f, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : _layerStack)
				layer->OnUpdate();

			_window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_running = false;
		return true;
	}

}
