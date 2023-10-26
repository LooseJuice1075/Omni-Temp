#include "ompch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"

namespace Omni
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    OM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}

		OM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
