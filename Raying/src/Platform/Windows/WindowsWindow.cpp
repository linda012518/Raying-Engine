#include "hzpch.h"
#include "WindowsWindow.h"

namespace Raying
{
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		_data.Title = props.Title;
		_data.Width = props.Width;
		_data.Height = props.Height;

		Ray_Core_Info("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			Raying_Core_Assert(success, "Could not intialize GLFW!");

			s_GLFWInitialized = true;
		}

		_window = glfwCreateWindow((int)props.Width, (int)props.Height, _data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(_window);
		glfwSetWindowUserPointer(_window, &_data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(_window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(_window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
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