#pragma once

#include "Base.h"

#include "Window.h"
#include "LayerStack.h"
#include "Raying/Events/Event.h"
#include "Raying/Events/ApplicationEvent.h"

#include "Timestep.h"

#include "Raying/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Raying {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator [](int index) const
		{
			Raying_Core_Assert(index < Count);
			return Args[index];
		}
	};

	class Raying_API Application
	{
	public:
		Application(const std::string& name = "Raying App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *_window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return _imguiLayer; }

		static Application& Get() { return *_instance; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return _commandLineArgs; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		ApplicationCommandLineArgs _commandLineArgs;
		Scope<Window> _window;//unique_ptr ��ռ��ָ��Ķ���
		ImGuiLayer* _imguiLayer;
		bool _running = true;
		bool _minimized = false; // ��С��
		LayerStack _layerStack;

		float _lastFrameTime = 0.0f;

		static Application* _instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}
