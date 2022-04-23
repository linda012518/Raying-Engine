#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define Raying_Platform_Windows
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define Raying_Platform_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define Raying_Platform_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
 /* We also have to check __ANDROID__ before __linux__
  * since android is based on the linux kernel
  * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define Raying_Platform_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define Raying_Platform_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection

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
	#define Raying_Enable_Asserts
#endif

#ifdef Raying_Enable_Asserts
	#define Raying_Assert(x, ...) { if(!(x)) { Raying_Error("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define Raying_Core_Assert(x, ...) { if(!(x)) { Raying_Core_Error("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define Raying_Assert(x, ...)
	#define Raying_Core_Assert(x, ...)
#endif

#define BIT(x) (1 << x)

#define Raying_Bind_Event_Fn(fn) std::bind(&fn, this, std::placeholders::_1)

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
