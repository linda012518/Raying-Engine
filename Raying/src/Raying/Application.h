#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

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
		ImGuiLayer* _imguiLayer;
		bool _running = true;
		LayerStack _layerStack;

		std::shared_ptr<Shader> _shader;
		std::shared_ptr<VertexArray> _vao;

		std::shared_ptr<Shader> _blueShader;
		std::shared_ptr<VertexArray> _blue_vao;

	private:
		static Application* _instance;
	};

	Application* CreateApplication();
}