#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Raying
{
	OpenGLContext::OpenGLContext(GLFWwindow * windowHandle) : _windowHandle(windowHandle)
	{
		Raying_Core_Assert(windowHandle, "windowHandle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		Raying_Core_Assert(status, "Failed to initialize Glad!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(_windowHandle);
	}

}