#include "hzpch.h"
#include "WindowsWindow.h"

#include "Raying/Core/Input.h"

#include "Raying/Events/ApplicationEvent.h"
#include "Raying/Events/MouseEvent.h"
#include "Raying/Events/KeyEvent.h"

#include "Raying/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Raying
{
	static bool s_GLFWInitialized = false;
	static uint8_t _GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		Raying_Core_Error("GLFW Error ({0}) : {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Raying_Profile_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Raying_Profile_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		Raying_Profile_FUNCTION();

		_data.Title = props.Title;
		_data.Width = props.Width;
		_data.Height = props.Height;

		Raying_Core_Info("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (_GLFWWindowCount == 0)
		{
			Raying_Profile_SCOPE("glfwInit");
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			Raying_Core_Assert(success, "Could not initialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			Raying_Profile_SCOPE("glfwCreateWindow");
#if defined(Raying_Debug)
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
			_window = glfwCreateWindow((int)props.Width, (int)props.Height, _data.Title.c_str(), nullptr, nullptr);
			++_GLFWWindowCount;
		}

		_context = GraphicsContext::Create(_window);
		_context->Init();

		glfwSetWindowUserPointer(_window, &_data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});

	}

	void WindowsWindow::Shutdown()
	{
		Raying_Profile_FUNCTION();

		glfwDestroyWindow(_window);

		--_GLFWWindowCount;

		if (_GLFWWindowCount == 0)
		{
			Raying_Core_Info("Terminate GLFW!");
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		Raying_Profile_FUNCTION();

		glfwPollEvents();
		_context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		Raying_Profile_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		_data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return _data.VSync;
	}
}