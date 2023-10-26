#include "ompch.h"
#include "UniformBuffer.h"

#include "Renderer.h"
#include "platform/OpenGL/OpenGLUniformBuffer.h"

namespace Omni
{

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    OM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		OM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}