#include "SceneHierarchyPanel.h"
#include "PropertiesPanel.h"
#include "PanelAccessor.h"
#include "events/KeyCodes.h"
#include "asset/AssetManager.h"
#include "Icons.h"

#include <imgui.h>

#include <filesystem>
#include <glm/gtc/type_ptr.hpp>

namespace Omni
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		ClearSelectedEntity();
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (m_Open)
		{
			if (ImGui::Begin("Scene Hierarchy", &m_Open, m_HierarchyWindowFlags))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("+"))
					{
						DrawSceneContextMenu();

						ImGui::EndMenu();
					}

					ImGui::EndMenuBar();
				}

				for (auto itr = m_Context->m_SceneOrder.cbegin(); itr != m_Context->m_SceneOrder.cend(); itr++)
				{
					Entity entity = { *itr };
					if (!entity.HasParent())
					{
						DrawEntityNode(*itr);
					}
				}

				// Left-click on blank space
				if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
				{
					ClearSelectedEntity();
					PanelAccessor::m_AssetBrowserPanel.ClearSelectedAsset();
				}

				// Right-click on blank space
				if (ImGui::BeginPopupContextWindow(0, ImGuiMouseButton_Right, false))
				{
					PanelAccessor::m_AssetBrowserPanel.ClearSelectedAsset();

					DrawSceneContextMenu();
					ImGui::EndPopup();
				}

				ImGui::End();
			}
			else
			{
				ImGui::End();
			}
		}
	}

	void SceneHierarchyPanel::SetSelectedEntity(const Entity& entity)
	{
		m_SelectionContext = entity;
		PanelAccessor::m_PropertiesPanel.SetSelectedEntity(m_SelectionContext);
		PanelAccessor::m_AssetBrowserPanel.ClearSelectedAsset();
	}

	void SceneHierarchyPanel::ClearSelectedEntity()
	{
		m_SelectionContext = {};
		PanelAccessor::m_PropertiesPanel.SetSelectedEntity({});
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& name = entity.GetName();
		RelationshipComponent* rc = &entity.GetComponent<RelationshipComponent>();

		ImGuiTreeNodeFlags flags;
		bool hasChildren = !rc->Children.empty();
		if (hasChildren)
		{
			flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)
				| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		}
		else
		{
			flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)
				| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Leaf;
		}
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		// Entity re-ordering system
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, -3.5f });
			// Invisible buttons cannot have a size of 0, so an offset is added
			ImGui::InvisibleButton("##", { ImGui::GetContentRegionAvailWidth() + 0.001f, 3.5f });
			ImGui::PopStyleVar();
			if (ImGui::BeginDragDropTarget())
			{
				ImGui::EndDragDropTarget();

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 2.5f });
				ImGui::PushStyleColor(ImGuiCol_DragDropTarget, { 0.0f, 0.0f, 0.0f, 0.0f });
				ImGui::Button("##", { ImGui::GetContentRegionAvailWidth(), 3.5f });
				ImGui::PopStyleVar();

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
					{
						const char* message = static_cast<const char*>(payload->Data);
						uint32_t entityID = std::atoi(message);
						Entity droppedEntity = { (entt::entity)entityID, m_Context.get() };
						auto& droppedEntityRC = droppedEntity.GetComponent<RelationshipComponent>();
						bool droppedEnityHasChildren = !droppedEntityRC.Children.empty();

						if (droppedEntity != entity)
						{
							if (entity.HasParent())
							{
								entity.GetParent().GetComponent<RelationshipComponent>().IsChildrenCacheDirty = true;
								droppedEntity.SetParent(entity.GetParent());
							}
							else
							{
								droppedEntity.RemoveParent();
							}

							// Remove dropped entity and its children
							m_Context->m_SceneOrder.remove(droppedEntity);
							if (droppedEnityHasChildren)
							{
								for (Entity child : droppedEntityRC.Children)
								{
									m_Context->m_SceneOrder.remove(child);
								}
							}

							// Get index of this entity and insert
							// the dropped entity before it
							std::list<Entity>::const_iterator entityPos;
							for (entityPos = m_Context->m_SceneOrder.cbegin(); entityPos != m_Context->m_SceneOrder.cend(); entityPos++)
							{
								if (*entityPos == entity)
								{
									break;
								}
							}

							m_Context->m_SceneOrder.insert(entityPos, droppedEntity);

							if (!m_Context->m_IsSpriteComponentCacheDirty)
							{
								if (droppedEntity.HasComponent<SpriteRendererComponent>())
								{
									m_Context->m_IsSpriteComponentCacheDirty = true;
								}
							}

							// Reorder the dropped entity's children
							if (droppedEnityHasChildren)
							{
								for (Entity child : droppedEntityRC.Children)
								{
									m_Context->m_SceneOrder.insert(entityPos, child);

									if (!m_Context->m_IsSpriteComponentCacheDirty)
									{
										if (child.HasComponent<SpriteRendererComponent>())
										{
											m_Context->m_IsSpriteComponentCacheDirty = true;
										}
									}
								}
							}
						}
					}

					ImGui::EndDragDropTarget();
				}
				ImGui::PopStyleColor();
			}
			else
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 2.5f });
				// Invisible buttons cannot have a size of 0, so an offset is added
				ImGui::InvisibleButton("##", { ImGui::GetContentRegionAvailWidth() + 0.001f, 3.5f });
				ImGui::PopStyleVar();
			}
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 4 });
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
		ImGui::PopStyleVar();
		if (ImGui::IsItemClicked())
		{
			SetSelectedEntity(entity);
		}
		if (ImGui::IsItemHovered() && entity == m_SelectionContext)
		{
			if (ImGui::IsKeyDown(OM_KEY_DELETE))
			{
				ClearSelectedEntity();
				m_Context->m_DeletedEntities.push_back(entity);
			}
		}

		// Drag and Drop
		if (ImGui::BeginDragDropSource())
		{
			std::string message = std::to_string((uint32_t)entity);
			ImGui::SetDragDropPayload("_TREENODE", message.c_str(), std::strlen(message.c_str()));
			ImGui::Text(entity.GetName().c_str());
			ImGui::EndDragDropSource();
		}

		bool isDragDrop = false;
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
			{
				const char* message = static_cast<const char*>(payload->Data);
				uint32_t entityID = std::atoi(message);
				Entity droppedEntity = { (entt::entity)entityID, m_Context.get() };

				#pragma region Check if parent is a descendant
				bool droppedEntityValid = true;
				bool foundThis = false;
				bool foundParent = false;
				bool needToSearch = true;
				for (auto itr = m_Context->m_SceneOrder.cbegin(); itr != m_Context->m_SceneOrder.cend(); itr++)
				{
					// If the parent comes after, 
					// we need to search
					if (foundThis && !foundParent)
						break;
					// If the parent comes before, 
					// we don't need to search
					if (foundParent && !foundThis)
					{
						needToSearch = false;
						break;
					}

					Entity curr = { *itr, m_Context.get() };
					if (curr == droppedEntity)
					{
						foundThis = true;
					}
					else if (curr == entity)
					{
						foundParent = true;
					}
				}

				if (needToSearch)
				{
					RelationshipComponent* parentrc = &entity.GetComponent<RelationshipComponent>();

					if (parentrc->Parent != entt::null)
					{
						Entity curr = { parentrc->Parent, m_Context.get() };
						bool search = true;
						while (search)
						{
							if (curr == droppedEntity)
							{
								droppedEntityValid = false;
								search = false;
							}
							else
							{
								if (curr.HasParent())
								{
									curr = curr.GetParent();
								}
								else
								{
									search = false;
								}
							}
						}
					}
				}
				#pragma endregion

				if (droppedEntityValid)
				{
					droppedEntity.SetParent(entity);
				}
			}
			isDragDrop = true;
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginPopupContextItem())
		{
			SetSelectedEntity(entity);

			if (ImGui::MenuItem("Add Empty"))
			{
				Entity empty = CreateEmpty();
				empty.SetParent(entity);
				SetSelectedEntity(empty);
			}

			if (ImGui::MenuItem("Add Sprite"))
			{
				Entity sprite = CreateSpriteEntity();
				sprite.SetParent(entity);
				SetSelectedEntity(sprite);
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Delete"))
			{
				if (entity == m_SelectionContext)
				{
					ClearSelectedEntity();
				}
				m_Context->m_DeletedEntities.push_back(entity);
			}

			ImGui::EndPopup();
		}

		if (opened && hasChildren)
		{
			for (auto itr = rc->Children.cbegin(); itr != rc->Children.cend(); itr++)
			{
				DrawEntityNode(*itr);
			}
		}

		if (opened)
		{
			ImGui::TreePop();
			if (isDragDrop)
			{

			}
		}
	}

	void SceneHierarchyPanel::DrawSceneContextMenu()
	{
		if (ImGui::MenuItem("Add Empty"))
		{
			SetSelectedEntity(CreateEmpty());
		}

		if (ImGui::MenuItem("Add Sprite"))
		{
			SetSelectedEntity(CreateSpriteEntity());
		}
	}

	Entity SceneHierarchyPanel::CreateEmpty()
	{
		return m_Context->CreateEntity("Entity");
	}

	Entity SceneHierarchyPanel::CreateSpriteEntity()
	{
		Entity spriteEntity = m_Context->CreateEntity("Sprite");
		spriteEntity.AddComponent<SpriteRendererComponent>();
		return spriteEntity;
	}
}
