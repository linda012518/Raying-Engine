#pragma once

#include "Raying/Core.h"

namespace Raying {

	class Raying_API GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}