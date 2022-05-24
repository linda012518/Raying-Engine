#pragma once

#include "Base.h"
#include "Log.h"
#include <filesystem>

#ifdef Raying_Enable_Asserts

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define Raying_Internal_Assert_Impl(type, check, msg, ...) { if(!(check)) { Raying##type##Error(msg, __VA_ARGS__); Raying_DebugBreak(); } }
	#define Raying_Internal_Assert_With_Msg(type, check, ...) Raying_Internal_Assert_Impl(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define Raying_Internal_Assert_No_Msg(type, check) Raying_Internal_Assert_Impl(type, check, "Assertion '{0}' failed at {1}:{2}", Raying_StringFY_Macro(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
	
	#define Raying_Internal_Assert_Get_Macro_Name(arg1, arg2, macro, ...) macro
	#define Raying_Internal_Assert_Get_Macro(...) Raying_Expand_Macro( Raying_Internal_Assert_Get_Macro_Name(__VA_ARGS__, Raying_Internal_Assert_With_Msg, Raying_Internal_Assert_No_Msg) )
	
	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define Raying_Assert(...) Raying_Expand_Macro( Raying_Internal_Assert_Get_Macro(__VA_ARGS__)(_, __VA_ARGS__) )
	#define Raying_Core_Assert(...) Raying_Expand_Macro( Raying_Internal_Assert_Get_Macro(__VA_ARGS__)(_Core_, __VA_ARGS__) )
#else
	#define Raying_Assert(x, ...)
	#define Raying_Core_Assert(x, ...)
#endif
