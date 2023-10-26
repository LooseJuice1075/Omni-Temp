#include "ompch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Omni
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		OM_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		OM_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		OM_CORE_ASSERT(status, "Failed to initialize Glad!");

		if (Log::s_LogRendererInfo)
		{
			OM_CORE_INFO("OpenGL Info:");
			OM_CORE_INFO("	Vendor: {0}", glGetString(GL_VENDOR));
			OM_CORE_INFO("	Renderer: {0}", glGetString(GL_RENDERER));
			OM_CORE_INFO("	Version: {0}", glGetString(GL_VERSION));
		}

		OM_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Omni requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		OM_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
