#pragma once

#include "Core.h"

namespace Raying {

	class Raying_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}