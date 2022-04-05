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

		Raying_Core_Info("OpenGL Info:");
		Raying_Core_Info("	Vendor: {0}", glGetString(GL_VENDOR));//���ظ���ǰOpenGLʵ�ֳ��̵�����
		Raying_Core_Info("	Renderer: {0}", glGetString(GL_RENDERER));//����һ����Ⱦ����ʶ����ͨ���Ǹ�Ӳ��ƽ̨
		Raying_Core_Info("	Version: {0}", glGetString(GL_VERSION));//���ص�ǰOpenGLʵ�ֵİ汾��
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(_windowHandle);
	}

}