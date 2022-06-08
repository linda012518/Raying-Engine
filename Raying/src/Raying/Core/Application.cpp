#include "hzpch.h"
#include "Application.h"
#include "Log.h"
#include "Raying/Renderer/Renderer.h"
#include "Input.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Raying
{
	Application* Application::_instance = nullptr;

	Application::Application(const std::string& name, ApplicationCommandLineArgs args)
		: _commandLineArgs(args)
	{
		Raying_Profile_FUNCTION();

		Raying_Core_Assert(!_instance, "Application alread exists!");
		_instance = this;

		_window = Window::Create(WindowProps(name));
		_window->SetEventCallback(Raying_Bind_Event_Fn(Application::OnEvent));
		_window->SetVSync(false);

		Renderer::Init();

		_imguiLayer = new ImGuiLayer();
		PushLayer(_imguiLayer);
	}

	Application::~Application()
	{
		Raying_Profile_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		Raying_Profile_FUNCTION();

		_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		Raying_Profile_FUNCTION();

		_layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		_running = false;
	}

	void Application::OnEvent(Event& e)
	{
		Raying_Profile_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(Raying_Bind_Event_Fn(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(Raying_Bind_Event_Fn(Application::OnWindowResize));

		for (auto itr = _layerStack.rbegin(); itr != _layerStack.rend(); ++itr)
		{
			if (e.Handled)
				break;
			(*itr)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		Raying_Profile_FUNCTION();

		while (_running)
		{			
			Raying_Profile_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Timestep ts = time - _lastFrameTime;
			_lastFrameTime = time;

			if (!_minimized) 
			{
				{
					Raying_Profile_SCOPE("layerStack OnUpdate");
					for (Layer* layer : _layerStack)
						layer->OnUpdate(ts);
				}

				_imguiLayer->Begin();
				{
					Raying_Profile_SCOPE("layerStack OnImGuiRender");
					for (Layer* layer : _layerStack)
						layer->OnImGuiRender();
				}
				_imguiLayer->End();

			}

			_window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		Raying_Profile_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			_minimized = true;
			return false;
		}

		_minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}
