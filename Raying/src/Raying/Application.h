#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Window.h"

namespace Raying {

	class Raying_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> _window;//unique_ptr ��ռ��ָ��Ķ���
		bool _running = true;
	};

	Application* CreateApplication();
}