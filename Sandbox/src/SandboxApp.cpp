
#include <Raying.h>

class ExampleLayer : public Raying::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		Ray_Info("ExampleLayer::Update");
	}

	void OnEvent(Raying::Event& event) override
	{
		Ray_Trace("{0}", event);
	}

};

class Sandbox : public Raying::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Raying::ImGuiLayer());
	}

	~Sandbox()
	{

	}

};

Raying::Application* Raying::CreateApplication()
{
	return new Sandbox();
}
