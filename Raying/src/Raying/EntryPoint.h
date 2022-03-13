#pragma once

#ifdef Raying_Platform_Windows

extern Raying::Application* Raying::CreateApplication();

int main(int arcg, char** argv)
{
	Raying::Log::Init();
	Ray_Core_Warn("Initialized Log!");

	int a = 5;
	Ray_Info("Hello Var={0}", a);

	Raying::Application* app = Raying::CreateApplication();

	app->Run();

	delete app;

	return 0;
}

#endif // Raying_Platform_Windows

