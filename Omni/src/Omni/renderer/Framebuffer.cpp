#include "ompch.h"
#include "Framebuffer.h"

#include "Renderer.h"

#include "platform/OpenGL/OpenGLFramebuffer.h"

namespace Omni
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    OM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
		}
	
		OM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
