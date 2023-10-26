#pragma once

#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/Components.h"
#include "asset/AssetManager.h"

#include <imgui.h>

namespace Omni
{
	enum SelectionType
	{
		No_Selection,
		Scene_Entity,
		Browser_Asset,
	};

	class PropertiesPanel
	{
	public:

		void OnImGuiRender();

		const Entity GetSelectedEntity() { return m_SelectedEntity; }
		void SetSelectedEntity(const Entity& entity) { m_SelectedEntity = entity; OnEntitySelected(); }

		const std::filesystem::directory_entry GetSelectedAsset() { return m_SelectedAsset; }
		void SetSelectedAsset(const std::filesystem::directory_entry& asset = std::filesystem::directory_entry("")) { m_SelectedAsset = asset; OnAssetSelected(); }

	public:

		bool m_Open = true;

	private:

		void OnEntitySelected();
		void OnAssetSelected();

		bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		void DrawComponents(Entity entity);

		void DrawTextFileWindow(const UUID& assetID, AssetManager::AssetFile& asset);
		void DrawTexture2DWindow(const UUID& assetID, AssetManager::AssetFile& asset);

	private:

		SelectionType m_SelectionType;
		Entity m_SelectedEntity = {};
		std::filesystem::directory_entry m_SelectedAsset = std::filesystem::directory_entry("");

		struct TextInputFilters
		{
			static int FilterFileNameLetters(ImGuiInputTextCallbackData* data)
			{
				if (data->EventChar < 256 && strchr("<>:\"/\\|?*", (char)data->EventChar))
					return 1;
				return 0;
			}
		};

		// Texture2D Props

		AssetManager::Texture2DProps m_Texture2DProps;

		const char* m_Texture2DFilteringModes[2] = { "Nearest", "Linear" };
		const char* m_Texture2DWrapModes[2] = { "Repeat", "Clamp" };

		// Texture2D Props

		ImGuiWindowFlags m_PropertiesWindowFlags =
			ImGuiWindowFlags_NoCollapse;

		friend class SceneHierarchyPanel;
		friend class AssetBrowserPanel;
	};
}
