#include "hzpch.h"
#include "Input.h"

#ifdef Raying_Platform_Windows
	#include "Platform/Windows/WindowsInput.h"
#endif // Raying_Platform_Windows

namespace Raying
{
	Scope<Input> Input::_instance = Input::Create();

	Scope<Input> Input::Create()
	{
#ifdef Raying_Platform_Windows
		return CreateScope<WindowsInput>();
#endif // Raying_Platform_Windows

		Raying_Core_Assert(false, "Unknow platform!");
		return nullptr;
	}

}
