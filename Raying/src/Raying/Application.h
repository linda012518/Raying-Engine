#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace Raying {

	class Raying_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *_window; }

		inline static Application& Get() { return *_instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> _window;//unique_ptr 独占所指向的对象
		bool _running = true;
		LayerStack _layerStack;

	private:
		static Application* _instance;
	};

	Application* CreateApplication();
}