#pragma once

#include <memory>

#include "PlatformDetection.h"

#ifdef Raying_Platform_Windows
	#if Raying_Dynamic_Link
		#ifdef Raying_Build_Dll
			#define Raying_API __declspec(dllexport)
		#else
			#define Raying_API __declspec(dllimport)
		#endif // !Raying_Build_Dll
	#else
		#define Raying_API
	#endif
#else
	#error Raying only supports Windows!
#endif // Raying_Platform_Windows

#ifdef Raying_Debug
	#if defined(Raying_Platform_Windows)
		#define Raying_DebugBreak() __debugbreak()
	#elif defined(Raying_Platform_LINUX)
		#include <signal.h>
		#define Raying_DebugBreak() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif

	#define Raying_Enable_Asserts
#else
	#define Raying_DebugBreak()
#endif

#ifdef Raying_Enable_Asserts
	#define Raying_Assert(x, ...) { if(!(x)) { Raying_Error("Assertion Failed: {0}", __VA_ARGS__); Raying_DebugBreak(); } }
	#define Raying_Core_Assert(x, ...) { if(!(x)) { Raying_Core_Error("Assertion Failed: {0}", __VA_ARGS__); Raying_DebugBreak(); } }
#else
	#define Raying_Assert(x, ...)
	#define Raying_Core_Assert(x, ...)
#endif

#define BIT(x) (1 << x)

#define Raying_Bind_Event_Fn(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Raying
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) //constexpr 将运算尽量放在编译阶段
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) //constexpr 将运算尽量放在编译阶段
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
