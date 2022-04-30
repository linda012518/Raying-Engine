#include "hzpch.h"
#include "Window.h"

#ifdef Raying_Platform_Windows
	#include "Platform/Windows/WindowsWindow.h"
#endif // Raying_Platform_Windows

namespace Raying
{

	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef Raying_Platform_Windows
		return CreateScope<WindowsWindow>(props);
#endif // Raying_Platform_Windows

		Raying_Core_Assert(false, "Unknow platform!");
		return nullptr;
	}

}
