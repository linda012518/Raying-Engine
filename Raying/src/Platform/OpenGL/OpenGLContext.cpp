#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Raying
{
	OpenGLContext::OpenGLContext(GLFWwindow * windowHandle) : _windowHandle(windowHandle)
	{
		Raying_Core_Assert(windowHandle, "windowHandle is null!");
	}

	void OpenGLContext::Init()
	{
		Raying_Profile_FUNCTION();

		glfwMakeContextCurrent(_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		Raying_Core_Assert(status, "Failed to initialize Glad!");

		Raying_Core_Info("OpenGL Info:");
		Raying_Core_Info("	Vendor: {0}", glGetString(GL_VENDOR));//返回负责当前OpenGL实现厂商的名字
		Raying_Core_Info("	Renderer: {0}", glGetString(GL_RENDERER));//返回一个渲染器标识符，通常是个硬件平台
		Raying_Core_Info("	Version: {0}", glGetString(GL_VERSION));//返回当前OpenGL实现的版本号

		Raying_Core_Assert(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Hazel requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		Raying_Profile_FUNCTION();

		glfwSwapBuffers(_windowHandle);
	}

}