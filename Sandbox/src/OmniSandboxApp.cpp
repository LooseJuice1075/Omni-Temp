#include <Omni.h>
#include <Omni/core/EntryPoint.h>

#include "SandboxApp.h"

namespace Omni
{
	class OmniSandbox : public Application
	{
	public:
		SandboxLayer* m_SandboxLayer = new SandboxLayer();

		OmniSandbox()
			: Application("Sandbox")
		{
			PushLayer(m_SandboxLayer);
		}

		~OmniSandbox()
		{
			PopLayer(m_SandboxLayer);
		}
	};

	Application* CreateApplication()
	{
		return new OmniSandbox();
	}
}
