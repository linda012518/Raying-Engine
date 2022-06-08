#pragma once
#include "Base.h"
#include "Application.h"

#ifdef Raying_Platform_Windows

extern Raying::Application* Raying::CreateApplication(ApplicationCommandLineArgs args);

int main(int arcg, char** argv)
{
	Raying::Log::Init();

	Raying_Profile_BEGIN_SESSION("Startup", "RayingProfile-Startup.json");
	Raying::Application* app = Raying::CreateApplication({ arcg, argv });
	Raying_Profile_END_SESSION();

	Raying_Profile_BEGIN_SESSION("Runtime", "RayingProfile-Runtime.json");
	app->Run();
	Raying_Profile_END_SESSION();

	Raying_Profile_BEGIN_SESSION("Shutdown", "RayingProfile-Shutdown.json");
	delete app;
	Raying_Profile_END_SESSION();

	return 0;
}

#endif // Raying_Platform_Windows

