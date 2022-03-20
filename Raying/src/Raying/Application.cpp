#include "hzpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

#include <GLFW/glfw3.h>

namespace Raying
{
	Application::Application()
	{
		_window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (_running)
		{
			glClearColor(0, 0.3f, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			_window->OnUpdate();
		}
	}


}
