#pragma once

#include "hzpch.h"
#include "Base.h"

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Raying
{
	class Raying_API Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
