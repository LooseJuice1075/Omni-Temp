#include "ompch.h"
#include "RenderCommand.h"

namespace Omni
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
