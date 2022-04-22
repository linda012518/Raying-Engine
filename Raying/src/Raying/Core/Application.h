#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Raying/Events/Event.h"
#include "Raying/Events/ApplicationEvent.h"

#include "Timestep.h"

#include "Raying/ImGui/ImGuiLayer.h"

namespace Raying {

	class Raying_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *_window; }

		inline static Application& Get() { return *_instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> _window;//unique_ptr 独占所指向的对象
		ImGuiLayer* _imguiLayer;
		bool _running = true;
		bool _minimized = false; // 最小化
		LayerStack _layerStack;

		float _lastFrameTime = 0.0f;

		static Application* _instance;

	};

	Application* CreateApplication();
}