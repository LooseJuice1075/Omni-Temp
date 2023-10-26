#include <Omni.h>
#include <Omni/core/EntryPoint.h>

#include "EditorLayer.h"

namespace Omni
{
	class OmniEditor : public Application
	{
	public:
		EditorLayer* m_EditorLayer = new EditorLayer();

		OmniEditor()
			: Application("Omni Editor")
		{
			PushLayer(m_EditorLayer);
		}

		~OmniEditor()
		{
			PopLayer(m_EditorLayer);
		}
	};

	Application* CreateApplication()
	{
		return new OmniEditor();
	}
}
