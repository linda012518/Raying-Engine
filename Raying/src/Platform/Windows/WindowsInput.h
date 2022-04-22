#pragma once

#include "Raying/Core/Input.h"

namespace Raying
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMouseButtonPositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}


