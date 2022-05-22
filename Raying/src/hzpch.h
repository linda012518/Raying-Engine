#pragma once

#include "Raying/Core/PlatformDetection.h"

#ifdef Raying_Platform_Windows
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Raying/Core/Log.h"

#include "Raying/Debug/Instrumentor.h"

#ifdef Raying_Platform_Windows
	#include <Windows.h>
#endif // !Raying_Platform_Windows

