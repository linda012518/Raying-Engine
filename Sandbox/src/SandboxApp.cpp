
#include <Raying.h>
#include <Raying/Core/EntryPoint.h>

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public Raying::Application
{
public:
	Sandbox(Raying::ApplicationCommandLineArgs args)
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}

};

Raying::Application* Raying::CreateApplication(Raying::ApplicationCommandLineArgs args)
{
	return new Sandbox(args);
}
