#include "ompch.h"
#include "WindowsWindow.h"
#include "core/Application.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "events/Event.h"
#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "Renderer/Renderer.h"

#include "platform/OpenGL/OpenGLContext.h"

namespace Omni
{
	static bool s_GLFWInitialized = false;

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		OM_CORE_ERROR("GLFW Error: ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		OM_PROFILE_FUNCTION();
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		OM_PROFILE_FUNCTION();
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		OM_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.WindowedWidth = props.Width;
		m_Data.WindowedHeight = props.Height;
		m_Data.Fullscreen = false;
		m_Data.CursorMode = CursorMode::Normal;

		if (Log::s_LogWindowEvents)
		{
			OM_CORE_INFO("Creating window \"{0}\" ({1}, {2})", props.Title, props.Width, props.Height);
		}

		// Initialize glfw
		if (s_GLFWWindowCount == 0)
		{
			OM_PROFILE_SCOPE("glfwInit");

			int success = glfwInit();
			OM_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		{
			OM_PROFILE_SCOPE("glfwCreateWindow");

		#if defined(OM_DEBUG)
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			{
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			}
		#endif

			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;

			// Center window in the middle of the screen

			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			int xpos = (mode->width / 2) - ((int)props.Width / 2);
			int ypos = (mode->height / 2) - ((int)props.Height / 2);
			glfwSetWindowPos(m_Window, xpos, ypos);
			m_Data.XPos = xpos;
			m_Data.YPos = ypos;
			m_Data.WindowedXPos = xpos;
			m_Data.WindowedYPos = ypos;
		}

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetMSAA(0);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			data.Width = width;
			data.Height = height;

			WindowResizeEvent e(width, height);
			data.EventCallback(e);
		});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.XPos = xpos;
			data.YPos = ypos;

			WindowMovedEvent e(xpos, ypos);
			data.EventCallback(e);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent e;
			data.EventCallback(e);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					data.EventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);
					data.EventCallback(e);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent e(keycode);
			data.EventCallback(e);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(button);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(button);
					data.EventCallback(e);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent e((float)xOffset, (float)yOffset);
			data.EventCallback(e);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent e((float)xPos, (float)yPos);
			data.EventCallback(e);
		});
	}

	void WindowsWindow::Shutdown()
	{
		OM_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		OM_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	inline void WindowsWindow::SetSize(uint32_t width, uint32_t height)
	{
		glfwSetWindowSize(m_Window, (int)width, (int)height);
	}

	void WindowsWindow::SetMSAA(int samples)
	{
		glfwWindowHint(GLFW_SAMPLES, samples);
		m_Data.MSAASamples = samples;
	}

	int WindowsWindow::GetMSAA() const
	{
		return m_Data.MSAASamples;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		OM_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::SetFullscreen(bool fullscreen)
	{
		if (IsFullscreen() == fullscreen)
			return;

		m_Data.Fullscreen = fullscreen;

		if (fullscreen)
		{
			m_Data.WindowedWidth = m_Data.Width;
			m_Data.WindowedHeight = m_Data.Height;
			m_Data.WindowedXPos = m_Data.XPos;
			m_Data.WindowedYPos = m_Data.YPos;

			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
			SetVSync(m_Data.VSync);
		}
		else
		{
			glfwSetWindowMonitor(m_Window, nullptr, m_Data.WindowedXPos, m_Data.WindowedYPos, m_Data.WindowedWidth, m_Data.WindowedHeight, GLFW_DONT_CARE);
			SetVSync(m_Data.VSync);
		}
	}

	bool WindowsWindow::IsFullscreen() const
	{
		return glfwGetWindowMonitor(m_Window) != nullptr;
	}

	void WindowsWindow::SetCursorMode(CursorMode cursorMode)
	{
		int flag = GLFW_CURSOR_NORMAL;
		switch (cursorMode)
		{
		case Omni::Normal:
			flag = GLFW_CURSOR_NORMAL;
			break;

		case Omni::Hidden:
			flag = GLFW_CURSOR_HIDDEN;
			break;

		case Omni::LockedAndHidden:
			flag = GLFW_CURSOR_DISABLED;
			break;

		default:
			break;
		}

		glfwSetInputMode(m_Window, GLFW_CURSOR, flag);
	}

	CursorMode WindowsWindow::GetCursorMode() const
	{
		return m_Data.CursorMode;
	}

	void WindowsWindow::SetCursorPosition(int x, int y)
	{
		SetCursorPos(x, y);
	}
}
