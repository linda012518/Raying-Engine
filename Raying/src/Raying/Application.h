#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Raying {

	class Raying_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> _window;//unique_ptr 独占所指向的对象
		bool _running = true;
	};

	Application* CreateApplication();
}