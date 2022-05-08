#pragma once

#include "Raying/Core/Base.h"

namespace Raying {

	class Raying_API GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Scope<GraphicsContext> Create(void* window);
	};
}