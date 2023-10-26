#pragma once

#include "core/Core.h"
#include "core/Log.h"
#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/Components.h"
#include "scene/ScriptableEntity.h"

#include <imgui.h>

namespace Omni
{
	class SceneHierarchyPanel
	{
	public:

		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(const Entity& entity = {});
		void ClearSelectedEntity();

	public:

		bool m_Open = true;

	private:

		void DrawEntityNode(Entity entity);

		// Draws the menu items available when in the context of the whole scene
		void DrawSceneContextMenu();
		Entity CreateEmpty();
		Entity CreateSpriteEntity();

	private:

		Ref<Scene> m_Context;
		Entity m_SelectionContext;

		ImGuiWindowFlags m_HierarchyWindowFlags =
			ImGuiWindowFlags_MenuBar|
			ImGuiWindowFlags_NoCollapse;

		friend class PropertiesPanel;
	};
}
