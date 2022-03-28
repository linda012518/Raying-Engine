#pragma once

#include "hzpch.h"
#include "Raying/Core.h"

namespace Raying
{
	class Raying_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return _instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return _instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return _instance->GetMouseButtonPositionImpl(); }
		inline static float GetMouseX() { return _instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return _instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMouseButtonPositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Input* _instance;
	};
}
