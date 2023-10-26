#pragma once

#include "SceneHierarchyPanel.h"
#include "PropertiesPanel.h"
#include "AssetBrowserPanel.h"

namespace Omni
{
	static class PanelAccessor
	{
	public:

		inline static SceneHierarchyPanel m_SceneHierarchyPanel;
		inline static PropertiesPanel m_PropertiesPanel;
		inline static AssetBrowserPanel m_AssetBrowserPanel;
	};
}
