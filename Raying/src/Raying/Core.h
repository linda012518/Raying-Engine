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

#define BIT(x) (1 << x)
