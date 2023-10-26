#include "ompch.h"
#include "AssetBrowserPanel.h"
#include "PropertiesPanel.h"
#include "PanelAccessor.h"
#include "Icons.h"
#include "asset/AssetManager.h"
#include "core/UUID.h"

#include <yaml-cpp/yaml.h>

namespace Omni
{
	AssetBrowserPanel::AssetBrowserPanel()
		: m_CurrentDirectory(AssetManager::GetAssetPath())
	{
	}

	void Omni::AssetBrowserPanel::OnImGuiRender()
	{
		if (m_Open)
		{
			if (ImGui::Begin("Asset Browser", &m_Open, m_WindowFlags))
			{
				if (ImGui::BeginMenuBar())
				{
					bool disableBackButton = true;
					if (m_CurrentDirectory != std::filesystem::path(AssetManager::GetAssetPath()))
					{
						disableBackButton = false;
					}

					ImGui::BeginDisabled(disableBackButton);
					if (ImGui::MenuItem("<-"))
					{
						m_CurrentDirectory = m_CurrentDirectory.parent_path();
						RefreshFileCache();
					}
					ImGui::EndDisabled();

					if (ImGui::BeginMenu("New"))
					{
						DrawNewEntryMenuItems();
						ImGui::EndMenu();
					}

					ImGui::EndMenuBar();
				}

				// Left-click on blank space
				if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
				{
					ClearSelectedAsset();
					PanelAccessor::m_SceneHierarchyPanel.ClearSelectedEntity();
				}

				// Right-click on blank space
				if (ImGui::BeginPopupContextWindow(0, ImGuiMouseButton_Right, false))
				{
					ClearSelectedAsset();
					PanelAccessor::m_SceneHierarchyPanel.ClearSelectedEntity();

					DrawBrowserContextMenu();
					ImGui::EndPopup();
				}

				if (m_DeleteEntryPopupOpen)
				{
					bool isDirectory = m_SelectionContext.is_directory();

					if (!isDirectory)
						ImGui::OpenPopup("Delete File?");
					else
						ImGui::OpenPopup("Delete Folder?");

					ImVec2 center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

					if (ImGui::BeginPopupModal("Delete File?"))
					{
						std::string fmt = "Are you sure you want to delete \"" + m_SelectionContext.path().filename().string() + "\"?";
						ImGui::Text(fmt.c_str());

						if (ImGui::Button("Yes"))
						{
							AssetManager::DeleteAssetFile(m_SelectionContext.path().string());

							RefreshFileCache();
							m_DeleteEntryPopupOpen = false;
							ImGui::CloseCurrentPopup();
						}

						if (ImGui::Button("No"))
						{
							m_DeleteEntryPopupOpen = false;
							ImGui::CloseCurrentPopup();
						}

						ImGui::EndPopup();
					}

					if (ImGui::BeginPopupModal("Delete Folder?"))
					{
						std::string fmt = "Are you sure you want to delete \"" + m_SelectionContext.path().stem().string() + "\"?";
						ImGui::Text(fmt.c_str());

						if (ImGui::Button("Yes"))
						{
							AssetManager::DeleteFolder(m_SelectionContext.path().string());

							RefreshFileCache();
							m_DeleteEntryPopupOpen = false;
							ImGui::CloseCurrentPopup();
						}

						if (ImGui::Button("No"))
						{
							m_DeleteEntryPopupOpen = false;
							ImGui::CloseCurrentPopup();
						}

						ImGui::EndPopup();
					}
				}

				if (m_NewFolderPopupOpen)
				{
					ImGui::OpenPopup("New Folder");

					ImVec2 center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

					if (ImGui::BeginPopupModal("New Folder", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
					{
						std::string& name = m_NewFolderName;
						char buffer[256];
						memset(buffer, 0, sizeof(buffer));
						strcpy_s(buffer, sizeof(buffer), name.c_str());

						ImGui::Text("Folder Name");
						ImGui::SameLine();
						ImGui::SetNextItemWidth((float)name.length() * (ImGui::GetFontSize() / 1.9f));
						ImGui::InputText("##Folder Name", buffer, sizeof(buffer), ImGuiInputTextFlags_CallbackCharFilter, TextInputFilters::FilterFileNameLetters);
						if (ImGui::IsItemDeactivatedAfterEdit())
						{
							m_NewFolderName = buffer;
						}

						if (ImGui::Button("Create"))
						{
							if (m_NewFolderName != "")
							{
								AssetManager::CreateFolder(m_CurrentDirectory.string() + "\\" + m_NewFolderName);
								RefreshFileCache();
							}

							m_NewFolderPopupOpen = false;
							ImGui::CloseCurrentPopup();
						}

						ImGui::SameLine();

						if (ImGui::Button("Cancel"))
						{
							m_NewFolderPopupOpen = false;
							ImGui::CloseCurrentPopup();
						}

						ImGui::EndPopup();
					}
				}

				if (m_NewTXTFilePopupOpen)
				{
					ImGui::OpenPopup("New Text Document");

					ImVec2 center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

					if (ImGui::BeginPopupModal("New Text Document", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
					{
						std::string& name = m_NewTXTFileName;
						char buffer[256];
						memset(buffer, 0, sizeof(buffer));
						strcpy_s(buffer, sizeof(buffer), name.c_str());

						ImGui::Text("File Name");
						ImGui::SameLine();
						ImGui::SetNextItemWidth((float)name.length() * (ImGui::GetFontSize() / 1.9f));
						ImGui::InputText("##File Name", buffer, sizeof(buffer), ImGuiInputTextFlags_CallbackCharFilter, TextInputFilters::FilterFileNameLetters);
						if (ImGui::IsItemDeactivatedAfterEdit())
						{
							m_NewTXTFileName = buffer;
						}

						if (ImGui::Button("Create"))
						{
							if (m_NewTXTFileName != "")
							{
								AssetManager::WriteToTextFile(m_CurrentDirectory.string() + "\\" + m_NewTXTFileName, "");
								RefreshFileCache();
							}

							m_NewTXTFilePopupOpen = false;
							ImGui::CloseCurrentPopup();
						}

						ImGui::SameLine();

						if (ImGui::Button("Cancel"))
						{
							m_NewTXTFilePopupOpen = false;
							ImGui::CloseCurrentPopup();
						}

						ImGui::EndPopup();
					}
				}

				static float padding = 8.0f;
				static float thumbnailSize = 85.0f;
				float cellSize = thumbnailSize + padding;

				float panelWidth = ImGui::GetContentRegionAvail().x;
				int columnCount = (int)(panelWidth / cellSize);
				if (columnCount < 1)
					columnCount = 1;

				ImGui::Columns(columnCount, 0, false);

				for (auto directoryEntry : m_FileCache)
				{
					const auto& path = directoryEntry.path();
					auto relativePath = std::filesystem::relative(path, AssetManager::GetAssetPath());
					std::string fileNameString = relativePath.filename().string();

					Ref<Texture2D> icon;
					if (directoryEntry.is_directory())
					{
						icon = Icons::m_DirectoryIcon;
					}
					else if (relativePath.extension().string() == ".png")
					{
						icon = Icons::m_TextureIcon;
					}
					else
					{
						icon = Icons::m_FileIcon;
					}

					bool isSelected = m_SelectionContext == directoryEntry;

					ImGui::PushID(fileNameString.c_str());
					if (!isSelected)
						ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
					ImGui::ImageButton(reinterpret_cast<void*>(icon->GetRendererID()), { thumbnailSize, thumbnailSize }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
					if (!isSelected)
						ImGui::PopStyleColor();
					if (ImGui::IsItemClicked() || ImGui::IsItemClicked(ImGuiMouseButton_Right))
					{
						SetSelectedAsset(directoryEntry);
						isSelected = true;
					}
					if (isSelected)
					{
						if (ImGui::BeginPopupContextWindow(fileNameString.c_str(), ImGuiMouseButton_Right, true))
						{
							DrawEntryContextMenu();
							ImGui::EndPopup();
						}
					}
					ImGui::PopID();

					if (ImGui::BeginDragDropSource())
					{
						const wchar_t* itemPath = relativePath.c_str();
						if (relativePath.extension().string() == ".omscene")
						{
							ImGui::SetDragDropPayload("ASSET_BROWSER_ITEM_OMSCENE", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
						}
						else if (relativePath.extension().string() == ".png")
						{
							ImGui::SetDragDropPayload("ASSET_BROWSER_ITEM_TEXTURE", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
						}
						else
						{
							ImGui::SetDragDropPayload("ASSET_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
						}
						ImGui::EndDragDropSource();
					}

					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						if (directoryEntry.is_directory())
						{
							m_CurrentDirectory /= path.filename();
							RefreshFileCache();
						}
					}
					ImGui::TextWrapped(fileNameString.c_str());

					ImGui::NextColumn();
				}

				ImGui::End();
			}
			else
			{
				ImGui::End();
			}
		}

		ImGui::Begin("Asset Manager");

		ImGui::Text("Asset Map");

		ImGui::Separator();

		for (auto& asset : AssetManager::s_AssetMap)
		{
			ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, AssetManager::AssetTypeToString(asset.second.AssetType).c_str());

			ImGui::SameLine();

			if (asset.second.IsLoaded)
				ImGui::TextColored({ 0.5f, 0.5f, 0.0f, 1.0f }, "Loaded");
			else
				ImGui::TextColored({ 0.0f, 0.5f, 0.5f, 1.0f }, "Not Loaded");

			ImGui::SameLine();

			ImGui::Text(std::to_string(asset.first).c_str());
			ImGui::SameLine();
			ImGui::Text(asset.second.FilePath.c_str());
		}

		ImGui::Separator();

		ImGui::Text("Textures");

		ImGui::Separator();

		for (auto& texture : AssetManager::s_Texture2Ds)
		{
			ImGui::Text("Refs:");
			ImGui::SameLine();
			ImGui::Text(std::to_string(texture.second.use_count()).c_str());
			ImGui::SameLine();
			ImGui::Text("ID:");
			ImGui::SameLine();
			ImGui::Text(std::to_string(texture.second->GetRendererID()).c_str());

			ImGui::Text(std::to_string(texture.first).c_str());

			ImGui::ImageButton(reinterpret_cast<void*>(texture.second->GetRendererID()), { 80.0f, 80.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}

		ImGui::End();
	}

	void AssetBrowserPanel::OnFixedUpdate()
	{
		if (m_FixedUpdateCount % m_FileRefreshInterval == 0)
		{
			RefreshFileCache();
		}

		m_FixedUpdateCount++;
	}

	void AssetBrowserPanel::RefreshFileCache()
	{
		m_FileCache.clear();

		if (std::filesystem::directory_entry(m_CurrentDirectory).exists())
		{
			for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
			{
				if (directoryEntry.path().extension() != ".assetprops")
				{
					m_FileCache.push_back(directoryEntry);
				}
			}
		}
		else
		{
			m_CurrentDirectory = AssetManager::GetAssetPath();

			for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
			{
				m_FileCache.push_back(directoryEntry);
			}
		}
	}

	void AssetBrowserPanel::SetSelectedAsset(const std::filesystem::directory_entry& asset)
	{
		m_SelectionContext = asset;
		PanelAccessor::m_PropertiesPanel.SetSelectedAsset(m_SelectionContext);
		PanelAccessor::m_SceneHierarchyPanel.ClearSelectedEntity();
		OnAssetSelected();
	}

	void AssetBrowserPanel::ClearSelectedAsset()
	{
		m_SelectionContext = std::filesystem::directory_entry("");
		PanelAccessor::m_PropertiesPanel.SetSelectedAsset();
	}

	void AssetBrowserPanel::OnAssetSelected()
	{
		UUID assetID = AssetManager::GetAssetID(m_SelectionContext.path().string());
		auto& asset = AssetManager::GetAsset(assetID);

		if (asset.AssetType == AssetManager::AssetType_Texture2D)
		{
			PanelAccessor::m_PropertiesPanel.m_Texture2DProps = AssetManager::LoadTexture2DAssetProps(assetID);
		}
	}

	void AssetBrowserPanel::DrawEntryContextMenu()
	{
		if (ImGui::MenuItem("Delete..."))
		{
			m_DeleteEntryPopupOpen = true;
		}
	}

	void AssetBrowserPanel::DrawBrowserContextMenu()
	{
		if (ImGui::BeginMenu("New"))
		{
			DrawNewEntryMenuItems();
			ImGui::EndMenu();
		}
	}

	void AssetBrowserPanel::DrawNewEntryMenuItems()
	{
		if (ImGui::MenuItem("Folder..."))
		{
			m_NewFolderPopupOpen = true;
			m_NewFolderName = "New Folder";
		}

		if (ImGui::MenuItem("Text Document..."))
		{
			m_NewTXTFilePopupOpen = true;
			m_NewTXTFileName = "New Text Document.txt";
		}
	}
}
