#include "hzpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace Raying
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
			Ray_Trace(e);
		if (e.IsInCategory(EventCategoryInput))
			Ray_Trace(e);
		while (true)
		{

		}
	}


}
