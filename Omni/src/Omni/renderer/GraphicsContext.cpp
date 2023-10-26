#include "ompch.h"
#include "GraphicsContext.h"

#include "Renderer.h"
#include "platform/OpenGL/OpenGLContext.h"

namespace Omni
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: OM_CORE_ASSERT(false, "RendererAPI::None is currently unsupported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}
	}
}
