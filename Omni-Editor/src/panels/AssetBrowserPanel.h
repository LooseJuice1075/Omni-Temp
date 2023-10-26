#pragma once

#include "asset/AssetManager.h"

#include <filesystem>
#include <imgui.h>

namespace Omni
{
	class AssetBrowserPanel
	{
	public:

		AssetBrowserPanel();

		void OnImGuiRender();
		void OnFixedUpdate();

		void RefreshFileCache();

		void SetSelectedAsset(const std::filesystem::directory_entry& asset = std::filesystem::directory_entry(""));
		void ClearSelectedAsset();

	public:

		bool m_Open = true;

	private:

		void OnAssetSelected();

		void DrawEntryContextMenu();
		void DrawBrowserContextMenu();
		void DrawNewEntryMenuItems();

	private:

		std::filesystem::path m_CurrentDirectory;
		std::filesystem::directory_entry m_SelectionContext = std::filesystem::directory_entry("");

		std::vector<std::filesystem::directory_entry> m_FileCache;

		uint64_t m_FixedUpdateCount = 0;
		uint64_t m_FileRefreshInterval = 60;

		ImGuiWindowFlags m_WindowFlags =
			ImGuiWindowFlags_MenuBar|
			ImGuiWindowFlags_NoCollapse;

		struct TextInputFilters
		{
			static int FilterFileNameLetters(ImGuiInputTextCallbackData* data)
			{
				if (data->EventChar < 256 && strchr("<>:\"/\\|?*", (char)data->EventChar))
					return 1;
				return 0;
			}
		};

		bool m_DeleteEntryPopupOpen = false;

		bool m_NewFolderPopupOpen = false;
		std::string m_NewFolderName = "New Folder";

		bool m_NewTXTFilePopupOpen = false;
		std::string m_NewTXTFileName = "New Text Document.txt";

		friend class PropertiesPanel;
	};
}
