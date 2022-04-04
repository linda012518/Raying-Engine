#pragma once

#include "Raying/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Raying
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* _windowHandle;
	};

}


