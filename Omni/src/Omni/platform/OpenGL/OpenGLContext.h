#pragma once

#include "renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Omni
{
	class OpenGLContext : public GraphicsContext
	{
	public:

		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:

		GLFWwindow* m_WindowHandle;
	};
}
