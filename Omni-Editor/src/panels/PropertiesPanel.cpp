#include "ompch.h"
#include "PropertiesPanel.h"
#include "PanelAccessor.h"
#include "Icons.h"
#include "events/KeyCodes.h"

#include <glm/gtc/type_ptr.hpp>

namespace Omni
{
	void Omni::PropertiesPanel::OnImGuiRender()
	{
		if (m_Open)
		{
			if (ImGui::Begin("Properties", &m_Open, m_PropertiesWindowFlags))
			{
				if (m_SelectedEntity)
				{
					DrawComponents(m_SelectedEntity);

					if (ImGui::Button("Add Component"))
					{
						ImGui::OpenPopup("AddComponent");
					}

					if (ImGui::BeginPopup("AddComponent"))
					{
						if (ImGui::MenuItem("Camera"))
						{
							if (!m_SelectedEntity.HasComponent<CameraComponent>())
							{
								m_SelectedEntity.AddComponent<CameraComponent>().Camera.SetFOV(0.2f);
							}
							ImGui::CloseCurrentPopup();
						}

						if (ImGui::MenuItem("Sprite Renderer"))
						{
							if (!m_SelectedEntity.HasComponent<SpriteRendererComponent>())
							{
								m_SelectedEntity.AddComponent<SpriteRendererComponent>();
							}
							ImGui::CloseCurrentPopup();
						}

						ImGui::EndPopup();
					}
				}
				else if (m_SelectedAsset.path() != "")
				{
					// File
					if (!m_SelectedAsset.is_directory())
					{
						UUID assetID = AssetManager::GetAssetID(m_SelectedAsset.path().string());
						auto& asset = AssetManager::GetAsset(assetID);

						auto& name = m_SelectedAsset.path().filename().string();
						char buffer[256];
						memset(buffer, 0, sizeof(buffer));
						strcpy_s(buffer, sizeof(buffer), name.c_str());

						ImGui::InputText("##FileName", buffer, sizeof(buffer), ImGuiInputTextFlags_CallbackCharFilter, TextInputFilters::FilterFileNameLetters);
						if (ImGui::IsItemDeactivatedAfterEdit())
						{
							if (!std::string(buffer).empty())
							{
								std::filesystem::directory_entry newPath = std::filesystem::directory_entry(m_SelectedAsset.path().parent_path().append(buffer));
								if (!newPath.exists())
								{
									if (AssetManager::RenameFile(assetID, buffer))
									{
										SetSelectedAsset(newPath);
									}
								}
							}
						}

						ImGui::Text(AssetManager::AssetTypeToString(asset.AssetType).c_str());

						if (asset.AssetType == AssetManager::AssetType_Text)
						{
							DrawTextFileWindow(assetID, asset);
						}
						else if (asset.AssetType == AssetManager::AssetType_Texture2D)
						{
							DrawTexture2DWindow(assetID, asset);
						}
					}
					// Directory
					else
					{
						auto& name = m_SelectedAsset.path().stem().string();
						char buffer[256];
						memset(buffer, 0, sizeof(buffer));
						strcpy_s(buffer, sizeof(buffer), name.c_str());

						ImGui::InputText("##FolderName", buffer, sizeof(buffer), ImGuiInputTextFlags_CallbackCharFilter, TextInputFilters::FilterFileNameLetters);
						if (ImGui::IsItemDeactivatedAfterEdit())
						{
							if (!std::string(buffer).empty())
							{
								std::filesystem::directory_entry newPath = std::filesystem::directory_entry(m_SelectedAsset.path().parent_path().append(buffer));
								if (!newPath.exists())
								{
									if (AssetManager::RenameFolder(m_SelectedAsset, buffer))
									{
										SetSelectedAsset(newPath);
									}
								}
							}
						}
					}
				}

				ImGui::End();
			}
			else
			{
				ImGui::End();
			}
		}
	}

	void PropertiesPanel::OnEntitySelected()
	{
		m_SelectionType = Scene_Entity;
	}

	void PropertiesPanel::OnAssetSelected()
	{
		m_SelectionType = Browser_Asset;
	}

	bool PropertiesPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
	{
		bool itemUsed = false;
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = 0.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		float itemWidth = ImGui::GetContentRegionAvail().x / 4.0f;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushItemWidth(itemWidth);
		if (ImGui::Button("X"))
		{
			values.x = resetValue;
			itemUsed = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f"))
			itemUsed = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushItemWidth(itemWidth);
		if (ImGui::Button("Y"))
		{
			values.y = resetValue;
			itemUsed = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f"))
			itemUsed = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushItemWidth(itemWidth);
		if (ImGui::Button("Z"))
		{
			values.z = resetValue;
			itemUsed = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f"))
			itemUsed = true;
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return itemUsed;
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& label, Entity entity, bool* componentActive, UIFunction uiFunction)
	{
		if (entity.HasComponent<T>())
		{
			const ImGuiTreeNodeFlags treeNodeFlags =
				ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap |
				ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed |
				ImGuiTreeNodeFlags_FramePadding;

			auto& component = entity.GetComponent<T>();

			if (componentActive != nullptr)
			{
				ImGui::PushID((void*)typeid(T).hash_code());
				ImGui::Checkbox("", componentActive);
				ImGui::PopID();
				ImGui::SameLine();
			}
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 2 });
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, label.c_str());
			ImGui::SameLine();

			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}
		}
	}

	void PropertiesPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<ActiveComponent>())
		{
			ImGui::PushID((void*)typeid(ActiveComponent).hash_code());
			ImGui::Checkbox("##", &entity.GetComponent<ActiveComponent>().Active);
			ImGui::PopID();
			ImGui::SameLine();
		}

		if (entity.HasComponent<NameComponent>())
		{
			auto& name = entity.GetName();

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), name.c_str());
			ImGui::PushID((void*)typeid(NameComponent).hash_code());
			if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			{
				entity.SetName(buffer);
			}
			ImGui::PopID();
		}

		// TEMP

		//uint32_t index = 0;
		//for (auto itr = m_Context->m_SceneOrder.cbegin(); itr != m_Context->m_SceneOrder.cend(); itr++)
		//{
		//	if (*itr == entity)
		//	{
		//		ImGui::Text("Index: %i", index);
		//	}

		//	index++;
		//}

		//ImGui::Text("ID: %u",(uint32_t)entity);
		//ImGui::Text("UUID: %u", (uint64_t)entity.GetUUID());

		// TEMP

		const ImGuiTreeNodeFlags treeNodeFlags =
			ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<TransformComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");

			if (open)
			{
				auto& transformComponent = entity.GetComponent<TransformComponent>();

				bool hasParent = entity.HasParent();

				if (!entity.HasParent())
				{
					if (DrawVec3Control("Position", transformComponent.Translation, 0.0f, 90.0f))
						entity.SetDescendantsWorldTransformDirty();
					if (DrawVec3Control("Rotation", transformComponent.Rotation, 0.0f, 90.0f))
						entity.SetDescendantsWorldTransformDirty();
					if (DrawVec3Control("Scale", transformComponent.Scale, 1.0f, 90.0f))
						entity.SetDescendantsWorldTransformDirty();
				}
				else
				{
					glm::vec3 transform = transformComponent.LocalTranslation;
					glm::vec3 rotation = transformComponent.LocalRotation;
					glm::vec3 scale = transformComponent.LocalScale;
					if (DrawVec3Control("Position", transform, 0.0f, 90.0f))
					{
						entity.SetDescendantsWorldTransformDirty();
						entity.SetLocalTransform(transform, rotation, scale);
					}
					if (DrawVec3Control("Rotation", rotation, 0.0f, 90.0f))
					{
						entity.SetDescendantsWorldTransformDirty();
						entity.SetLocalTransform(transform, rotation, scale);
					}
					if (DrawVec3Control("Scale", scale, 1.0f, 90.0f))
					{
						entity.SetDescendantsWorldTransformDirty();
						entity.SetLocalTransform(transform, rotation, scale);
					}
				}

				ImGui::TreePop();
			}
		}

		// CameraComponent
		{
			bool* componentActive = nullptr;
			if (entity.HasComponent<CameraComponent>())
			{
				componentActive = &entity.GetComponent<CameraComponent>().Active;
			}

			DrawComponent<CameraComponent>("Camera", entity, componentActive, [&entity](auto& component)
				{
					ImGui::Checkbox("Main Camera", &component.IsMainCamera);

					const char* camTypeStrings[] = { "Orthographic", "Projection" };
					const char* currentCamTypeString = camTypeStrings[(int)component.Camera.GetCameraType()];

					if (ImGui::BeginCombo("Type", currentCamTypeString))
					{
						for (int i = 0; i < 2; i++)
						{
							bool isSelected = currentCamTypeString == camTypeStrings[i];
							if (ImGui::Selectable(camTypeStrings[i], isSelected))
							{
								currentCamTypeString = camTypeStrings[i];
								component.Camera.SetCameraType((CameraType)i);
							}

							if (isSelected)
								ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}

					float fov = component.Camera.GetFOV();
					if (ImGui::DragFloat("Fov", &fov, 0.1f))
					{
						component.Camera.SetFOV(fov);
					}

					float nearClip = component.Camera.GetNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip, 1.0f))
					{
						component.Camera.SetNearClip(nearClip);
					}
					float farClip = component.Camera.GetFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip, 1.0f))
					{
						component.Camera.SetFarClip(farClip);
					}

					if (ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio))
					{
						Scene* context = entity.GetScene();

						/*if (component.Camera.m_ResolutionX != context->m_ViewportWidth
							|| component.Camera.m_ResolutionY != context->m_ViewportHeight)
						{
							component.Camera.m_ResolutionX = context->m_ViewportWidth;
							component.Camera.m_ResolutionY = context->m_ViewportHeight;
						}*/
					}

					if (component.FixedAspectRatio)
					{
						int resX = component.Camera.GetResolutionX();
						if (ImGui::DragInt("Horizontal Resolution", &resX))
						{
							if (resX <= 0)
								resX = 1;
							component.Camera.SetResolutionX(resX);
						}

						int resY = component.Camera.GetResolutionY();
						if (ImGui::DragInt("Vertical Resolution", &resY))
						{
							if (resY <= 0)
								resY = 1;
							component.Camera.SetResolutionY(resY);
						}
					}

					//ImGui::Image((void*)component.Camera.m_FrameBuffer->GetTexture()->GetID(),
					//	{ component.Camera.m_ResolutionX * 0.2f, component.Camera.m_ResolutionY * 0.2f });
				});
		}

		// SpriteRendererComponent
		{
			bool* componentActive = nullptr;
			if (entity.HasComponent<SpriteRendererComponent>())
			{
				componentActive = &entity.GetComponent<SpriteRendererComponent>().Active;
			}

			DrawComponent<SpriteRendererComponent>("Sprite", entity, componentActive, [](auto& component)
				{
					if (component.HasTexture)
					{
						Ref<Texture2D> texture = AssetManager::GetTexture2D(component.TextureAssetID);
						float aspectRatio = texture->GetWidth() / texture->GetHeight();
						float height = (160.0f / aspectRatio);

						ImGui::ImageButton(reinterpret_cast<void*>(texture->GetRendererID()), { 160.0f, height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
					}
					else
					{
						ImGui::ImageButton(reinterpret_cast<void*>(Icons::m_TextureIcon->GetRendererID()), { 160.0f, 160.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
					}

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM_TEXTURE"))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::wstring pathWS(path);
							std::string pathString(pathWS.begin(), pathWS.end());
							pathString = AssetManager::GetAssetPath().string() + "\\" + pathString;
							if (AssetManager::DoesAssetExist(pathString))
							{
								component.TextureAssetID = AssetManager::GetAssetID(pathString);
								component.HasTexture = true;
								Ref<Texture2D> texture;
								if (!AssetManager::IsAssetLoaded(component.TextureAssetID))
								{
									texture = AssetManager::LoadTexture2DAsync(component.TextureAssetID);
								}
								else
								{
									texture = AssetManager::GetTexture2D(component.TextureAssetID);
								}

								component.Size = { texture->GetWidth(), texture->GetHeight() };
							}
						}

						ImGui::EndDragDropTarget();
					}

					ImGui::Text("Color");
					ImGui::SameLine();
					ImGui::ColorEdit4("##Color", glm::value_ptr(component.Color));

					ImGui::BeginDisabled(!component.HasTexture);

					ImGui::Text("Tiling");
					ImGui::SameLine();
					ImGui::InputFloat("##Tiling", &component.TilingFactor);

					ImGui::Text("Sprite Position");
					ImGui::SameLine();

					ImGui::PushItemWidth(ImGui::GetContentRegionMax().x / 8.6f);
					ImGui::InputFloat("##SpritePosX", &component.SpritePosition.x, 0.0f, 0.0f, "%.0f");
					ImGui::PopItemWidth();

					ImGui::SameLine(0.0f, 1.0f);
					if (ImGui::ArrowButton("##SpritePosXButtonLeft", ImGuiDir_Left))
					{
						component.SpritePosition.x--;
					}
					ImGui::SameLine(0.0f, 1.0f);
					if (ImGui::ArrowButton("##SpritePosXButtonRight", ImGuiDir_Right))
					{
						component.SpritePosition.x++;
					}

					ImGui::SameLine(0.0f, 1.0f);

					ImGui::PushItemWidth(ImGui::GetContentRegionMax().x / 8.6f);
					ImGui::InputFloat("##SpritePosY", &component.SpritePosition.y, 0.0f, 0.0f, "%.0f");
					ImGui::PopItemWidth();

					ImGui::SameLine(0.0f, 1.0f);
					if (ImGui::ArrowButton("##SpritePosYButtonUp", ImGuiDir_Up))
					{
						component.SpritePosition.y++;
					}
					ImGui::SameLine(0.0f, 1.0f);
					if (ImGui::ArrowButton("##SpritePosYButtonDown", ImGuiDir_Down))
					{
						component.SpritePosition.y--;
					}

					ImGui::Text("Size");
					ImGui::SameLine();
					ImGui::InputFloat2("##Size", glm::value_ptr(component.Size), "%.0f");

					ImGui::EndDisabled();
				});
		}
	}

	void PropertiesPanel::DrawTextFileWindow(const UUID& assetID, AssetManager::AssetFile& asset)
	{
		if (!asset.IsLoaded)
		{
			AssetManager::LoadTextFile(assetID);
		}
		else
		{
			ImGui::BeginChild("Text File", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_HorizontalScrollbar);

			const std::string& textFileContent = AssetManager::GetTextFile(assetID);
			ImGui::TextUnformatted(textFileContent.c_str());

			ImGui::EndChild();
		}
	}

	void PropertiesPanel::DrawTexture2DWindow(const UUID& assetID, AssetManager::AssetFile& asset)
	{
		if (!asset.IsLoaded)
		{
			AssetManager::LoadTexture2DAsync(assetID);
		}
		else
		{
			Ref<Texture2D> texture = AssetManager::GetTexture2D(assetID);
			float size = ImGui::GetContentRegionAvailWidth() - 20.0f;
			if (size > 320.0f)
				size = 320.0f;
			if (size < 80.0f)
				size = 80.0f;
			float aspectRatio = texture->GetWidth() / texture->GetHeight();
			float height = (size / aspectRatio);

			ImGui::ImageButton(reinterpret_cast<void*>(texture->GetRendererID()), { size, height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			ImGui::Separator();

			int selectedMinFilter = (int)m_Texture2DProps.MinFilter;
			ImGui::Text("Min Filter");
			ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			if (ImGui::Combo("##Min Filter", &selectedMinFilter, m_Texture2DFilteringModes, IM_ARRAYSIZE(m_Texture2DFilteringModes)))
				m_Texture2DProps.MinFilter = (TextureFilteringMode)selectedMinFilter;

			int selectedMagFilter = (int)m_Texture2DProps.MagFilter;
			ImGui::Text("Mag Filter");
			ImGui::SameLine();
			if (ImGui::Combo("##Mag Filter", &selectedMagFilter, m_Texture2DFilteringModes, IM_ARRAYSIZE(m_Texture2DFilteringModes)))
				m_Texture2DProps.MagFilter = (TextureFilteringMode)selectedMagFilter;

			int selectedWrapMode = (int)m_Texture2DProps.WrapMode;
			ImGui::Text("Wrap Mode");
			ImGui::SameLine();
			if (ImGui::Combo("##Wrap Mode", &selectedWrapMode, m_Texture2DWrapModes, IM_ARRAYSIZE(m_Texture2DWrapModes)))
				m_Texture2DProps.WrapMode = (TextureWrapMode)selectedWrapMode;

			ImGui::PopItemWidth();

			ImGui::Text("Generate MipMaps");
			ImGui::SameLine();
			ImGui::Checkbox("##Generate MipMaps", &m_Texture2DProps.GenerateMips);

			if (ImGui::Button("Save Properties"))
			{
				AssetManager::SaveTexture2DAssetProps(assetID, m_Texture2DProps);
				AssetManager::UnloadTexture2D(assetID);
				AssetManager::LoadTexture2DAsync(assetID);
			}
		}
	}
}
