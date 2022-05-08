#pragma once

#include "hzpch.h"
#include "Base.h"

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Raying
{
	class Raying_API Input
	{
	protected:
		Input() = default;
	public:
		Input(const Input&) = delete;
		Input& operator = (const Input&) = delete;

		inline static bool IsKeyPressed(KeyCode key) { return _instance->IsKeyPressedImpl(key); }

		inline static bool IsMouseButtonPressed(MouseCode button) { return _instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return _instance->GetMouseButtonPositionImpl(); }
		inline static float GetMouseX() { return _instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return _instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(KeyCode key) = 0;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual std::pair<float, float> GetMouseButtonPositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

		static Scope<Input> Create();
	private:
		static Scope<Input> _instance;
	};
}
