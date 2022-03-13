
#include <Raying.h>

class Sandbox : public Raying::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

Raying::Application* Raying::CreateApplication()
{
	return new Sandbox();
}
