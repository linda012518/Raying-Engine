
#include <Raying.h>

#include "imgui/imgui.h"

class ExampleLayer : public Raying::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (Raying::Input::IsKeyPressed(RAYING_KEY_TAB))
			Raying_Trace("Tab key is pressed (poll) !");
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test!!!");
		ImGui::Text("Hello My Raying Engine!!!");
		ImGui::End();
	}

	void OnEvent(Raying::Event& event) override
	{
		if (event.GetEventType() == Raying::EventType::KeyPressed)
		{
			Raying::KeyPressedEvent& e = (Raying::KeyPressedEvent&)event;
			if (Raying::Input::IsKeyPressed(RAYING_KEY_TAB))
				Raying_Trace("Tab key is pressed (event) !");
			Raying_Trace("{0}", (char)e.GetKeyCode());
		}
	}

};

class Sandbox : public Raying::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

Raying::Application* Raying::CreateApplication()
{
	return new Sandbox();
}
