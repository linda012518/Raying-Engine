#pragma once

#ifdef Raying_Platform_Windows
	#ifdef Raying_Build_Dll
		#define Raying_API __declspec(dllexport)
	#else
		#define Raying_API __declspec(dllimport)
	#endif // !Raying_Build_Dll
#else
	#error Raying only supports Windows!
#endif // Raying_Platform_Windows

#ifdef Raying_Debug
	#define HZ_ENABLE_ASSERTS
#endif

#ifdef HZ_ENABLE_ASSERTS
	#define Raying_Assert(x, ...) { if(!(x)) { Raying_Error("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define Raying_Core_Assert(x, ...) { if(!(x)) { Raying_Core_Error("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define Raying_Assert(x, ...)
	#define Raying_Core_Assert(x, ...)
#endif

#define BIT(x) (1 << x)

#define Raying_Bind_Event_Fn(fn) std::bind(&fn, this, std::placeholders::_1)
