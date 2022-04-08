#pragma once

#include "Raying/Core.h"

namespace Raying {

	enum class Raying_API RendererAPI
	{
		None = 0, OpenGL = 1
	};

	class Raying_API Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return _rendererAPI; }

	private:
		static RendererAPI _rendererAPI;
	};

}