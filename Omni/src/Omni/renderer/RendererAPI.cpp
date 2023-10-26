#include "ompch.h"
#include "renderer/RendererAPI.h"

#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace Omni
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None: OM_CORE_ASSERT(false, "RendererAPI::None is currently unsupported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateScope<OpenGLRendererAPI>();
		}

		OM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
