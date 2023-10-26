#pragma once

#include <entt.hpp>
#include <typeinfo>

#include "Scene.h"
#include "Components.h"

namespace Omni
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			OM_CORE_ASSERT(!HasComponent<T>(), "[ENTITY ERROR]: Entity already has component!");

			// Cache Checks
			if (typeid(T) == typeid(SpriteRendererComponent))
			{
				m_Scene->m_IsSpriteComponentCacheDirty = true;
			}
			// Cache Checks

			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			OM_CORE_ASSERT(HasComponent<T>(), "[ENTITY ERROR]: Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			OM_CORE_ASSERT(HasComponent<T>(), "[ENTITY ERROR]: Entity does not have component!");

			// Cache Checks
			if (typeid(T) == typeid(SpriteRendererComponent))
			{
				m_Scene->m_IsSpriteComponentCacheDirty = true;
			}
			// Cache Checks

			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		void SetActive(bool isActive) { GetComponent<ActiveComponent>().Active = isActive; }
		bool IsActive() { return GetComponent<ActiveComponent>().Active; }

		void SetName(const std::string& name) { GetComponent<NameComponent>().Name = name; }
		const std::string& GetName() { return GetComponent<NameComponent>().Name; }

		// If the entity has no parent, the world transform is set, if it does have a parent, the local transform is set
		void SetTransform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
		{
			auto& tc = GetComponent<TransformComponent>();
			if (!HasParent())
			{
				tc.Translation = translation;
				tc.Rotation = rotation;
				tc.Scale = scale;
			}
			else
			{
				tc.LocalTranslation = translation;
				tc.LocalRotation = rotation;
				tc.LocalScale = scale;
				tc.isWorldDirty = true;
				SetDescendantsWorldTransformDirty();
			}
		}

		void SetWorldTransform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
		{
			auto& tc = GetComponent<TransformComponent>();
			tc.Translation = translation;
			tc.Rotation = rotation;
			tc.Scale = scale;
		}

		void SetLocalTransform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
		{
			auto& tc = GetComponent<TransformComponent>();
			tc.LocalTranslation = translation;
			tc.LocalRotation = rotation;
			tc.LocalScale = scale;
			tc.isWorldDirty = true;
			SetDescendantsWorldTransformDirty();
		}

		// If the entity has no parent, the world translation is set, if it does have a parent, the local translation is set
		void SetTranslation(glm::vec3 translation)
		{
			auto& tc = GetComponent<TransformComponent>();
			if (!HasParent())
			{
				tc.Translation = translation;
			}
			else
			{
				tc.LocalTranslation = translation;
				tc.isWorldDirty = true;
				SetDescendantsWorldTransformDirty();
			}
		}
		void SetWorldTranslation(glm::vec3 translation)
		{
			auto& tc = GetComponent<TransformComponent>();
			tc.Translation = translation;
		}
		void SetLocalTranslation(glm::vec3 translation)
		{
			auto& tc = GetComponent<TransformComponent>();
			tc.LocalTranslation = translation;
			tc.isWorldDirty = true;
			SetDescendantsWorldTransformDirty();
		}

		// If the entity has no parent, the world rotation is set, if it does have a parent, the local rotation is set
		void SetRotation(glm::vec3 rotation)
		{
			auto& tc = GetComponent<TransformComponent>();
			if (!HasParent())
			{
				tc.Rotation = rotation;
			}
			else
			{
				tc.LocalRotation = rotation;
				tc.isWorldDirty = true;
				SetDescendantsWorldTransformDirty();
			}
		}
		void SetWorldRotation(glm::vec3 rotation)
		{
			auto& tc = GetComponent<TransformComponent>();
			tc.Rotation = rotation;
		}
		void SetLocalRotation(glm::vec3 rotation)
		{
			auto& tc = GetComponent<TransformComponent>();
			tc.LocalRotation = rotation;
			tc.isWorldDirty = true;
			SetDescendantsWorldTransformDirty();
		}

		// If the entity has no parent, the world scale is set, if it does have a parent, the local scale is set
		void SetScale(glm::vec3 scale)
		{
			auto& tc = GetComponent<TransformComponent>();
			if (!HasParent())
			{
				tc.Scale = scale;
			}
			else
			{
				tc.LocalScale = scale;
				tc.isWorldDirty = true;
				SetDescendantsWorldTransformDirty();
			}
		}
		void SetWorldScale(glm::vec3 scale)
		{
			auto& tc = GetComponent<TransformComponent>();
			tc.Scale = scale;
		}
		void SetLocalScale(glm::vec3 scale)
		{
			auto& tc = GetComponent<TransformComponent>();
			tc.LocalScale = scale;
			tc.isWorldDirty = true;
			SetDescendantsWorldTransformDirty();
		}

		void SetParent(Entity parent)
		{
			RelationshipComponent* rc = &GetComponent<RelationshipComponent>();
			RelationshipComponent* parentrc = &parent.GetComponent<RelationshipComponent>();

			// If the new parent is equal 
			// to the old one, do nothing
			if (parent == rc->Parent)
			{
				return;
			}

			#pragma region Check if parent is a descendant
			bool foundThis = false;
			bool foundParent = false;
			bool needToSearch = true;
			for (auto itr = m_Scene->m_SceneOrder.cbegin(); itr != m_Scene->m_SceneOrder.cend(); itr++)
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

				Entity entity = { *itr, m_Scene };
				if (entity == *this)
				{
					foundThis = true;
				}
				else if (entity == parent)
				{
					foundParent = true;
				}
			}

			if (needToSearch)
			{
				if (parentrc->Parent != entt::null)
				{
					Entity curr = { parentrc->Parent, m_Scene };
					bool search = true;
					while (search)
					{
						if (curr == *this)
						{
							OM_CORE_ERROR("[ENTITY ERROR]: Cannot make a descendant a parent of its parent!");
							return;
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

			// Remove old parent
			if (rc->Parent != entt::null)
			{
				Entity oldParent = { rc->Parent, m_Scene };
				RelationshipComponent* oldParentrc = &oldParent.GetComponent<RelationshipComponent>();
				oldParentrc->Children.remove(*this);
				oldParentrc->IsChildrenCacheDirty = true;
				rc->Parent = entt::null;
			}

			std::list<Entity>::const_iterator itr;

			// Remove this entity from old position
			m_Scene->m_SceneOrder.remove(*this);

			// If parent has no children, 
			// then find the index of parent
			if (parentrc->Children.empty())
			{
				// Get index of this entity
				itr = std::find(m_Scene->m_SceneOrder.cbegin(), m_Scene->m_SceneOrder.cend(),
					parent);
			}
			// If parent has children, then find
			// the index of the lowest child
			else
			{
				Entity lowestChild = parentrc->Children.back();
				bool lowestChildFound = false;
				while (!lowestChildFound)
				{
					RelationshipComponent* lowestChildrc = &lowestChild.GetComponent<RelationshipComponent>();
					if (!lowestChildrc->Children.empty())
					{
						lowestChild = lowestChildrc->Children.back();
					}
					else
					{
						lowestChildFound = true;
					}
				}
				// Get index of the lowest child
				itr = std::find(m_Scene->m_SceneOrder.cbegin(), m_Scene->m_SceneOrder.cend(),
					lowestChild);
			}

			// If index of entity was found in the scene order
			if (itr != m_Scene->m_SceneOrder.cend())
			{
				// Insert after entity
				m_Scene->m_SceneOrder.insert(++itr, *this);

				// Set the dropped entity's parent to this entity
				// and add dropped entity to this entity's list of children 
				rc->Parent = parent;
				parentrc->Children.push_back(*this);

				// Check if sprite component cache needs to be updated
				if (HasComponent<SpriteRendererComponent>())
				{
					m_Scene->m_IsSpriteComponentCacheDirty = true;
				}

				if (!rc->Children.empty())
				{
					auto pos = std::find(m_Scene->m_SceneOrder.cbegin(), m_Scene->m_SceneOrder.cend(),
										*this);
					pos++;
					bool isLastOrphan = true;
					std::list<Entity>::const_iterator insertPos;
					// Find entity after this one with no parent
					for (auto itr = pos; itr != m_Scene->m_SceneOrder.cend(); itr++)
					{
						Entity entity = { *itr };
						if (!entity.HasParent())
						{
							insertPos = itr;
							isLastOrphan = false;
							break;
						}
						else
						{
							isLastOrphan = true;
						}
					}
					RemoveDescendantsFromOrder();

					if (!isLastOrphan)
					{
						InsertDescendantsInOrder(insertPos);
					}
					else
					{
						PushBackDescendantsInOrder();
					}
				}
			}
			else
			{
				OM_CORE_ERROR("[ENTITY ERROR]: Could not set the parent of \"{0}\"!", GetName());
			}
		}
		void RemoveParent()
		{
			if (!HasParent())
				return;

			RelationshipComponent* rc = &GetComponent<RelationshipComponent>();
			Entity parent = { rc->Parent, m_Scene };
			RelationshipComponent* parentrc = &parent.GetComponent<RelationshipComponent>();

			// Update order in scene
			{
				// Find highest parent

				// Start out with this entity's parent as the highest parent, 
				// and then iterate from there
				Entity highestParent = parent;
				bool highestParentFound = false;
				while (!highestParentFound)
				{
					// If this parent has a parent, 
					// then the parent's parent is now the highest parent
					if (highestParent.HasParent())
					{
						highestParent = highestParent.GetParent();
					}
					// If this parent has no parent, then it is the highest parent
					else
					{
						highestParentFound = true;
					}
				}

				// Find lowest child

				RelationshipComponent* highestParentrc = &highestParent.GetComponent<RelationshipComponent>();
				Entity lowestChild = highestParentrc->Children.back();
				bool lowestChildFound = false;
				while (!lowestChildFound)
				{
					// If this child has children, 
					// the last one is the new lowest child
					RelationshipComponent* rc = &lowestChild.GetComponent<RelationshipComponent>();
					if (!rc->Children.empty())
					{
						lowestChild = rc->Children.back();
					}
					// If this child has no children, 
					// then this one is the absolute lowest
					else
					{
						lowestChildFound = true;
					}
				}

				// Remove from old position
				m_Scene->m_SceneOrder.remove(*this);

				// Find index of lowest child
				auto itr = std::find(m_Scene->m_SceneOrder.cbegin(), m_Scene->m_SceneOrder.cend(), lowestChild);

				// If index of lowest child was found in the scene order list
				if (itr != m_Scene->m_SceneOrder.cend())
				{
					// Insert this entity after lowest child
					m_Scene->m_SceneOrder.insert(++itr, *this);

					// Check if sprite component cache needs to be updated
					if (HasComponent<SpriteRendererComponent>())
					{
						m_Scene->m_IsSpriteComponentCacheDirty = true;
					}
				}
				else
				{
					OM_CORE_ERROR("[ENTITY ERROR]: \"{0}\" Could not reorder entity!", __FUNCSIG__);
				}
			}

			// Remove this entity from its parent's list of children
			parentrc->Children.remove(*this);
			parentrc->IsChildrenCacheDirty = true;
			rc->Parent = entt::null;
		}
		Entity GetParent() { return { GetComponent<RelationshipComponent>().Parent, m_Scene }; }
		bool HasParent() { return (GetComponent<RelationshipComponent>().Parent != entt::null); }
		Scene* GetScene() { return m_Scene; }
		bool IsValid() { return m_EntityHandle != entt::null; }

		void SetUUID(UUID id) { GetComponent<UUIDComponent>().id = id; }
		UUID GetUUID() { return GetComponent<UUIDComponent>().id; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		// [Utility Function] Sets all children's world transforms to dirty
		void SetDescendantsWorldTransformDirty()
		{
			RelationshipComponent* rc = &GetComponent<RelationshipComponent>();
			for (auto entity : rc->Children)
			{
				entity.GetComponent<TransformComponent>().isWorldDirty = true;
				entity.SetDescendantsWorldTransformDirty();
			}
		}

	private:
		// [Utility Function] Removes all children from the scene order
		void RemoveDescendantsFromOrder()
		{
			RelationshipComponent* rc = &GetComponent<RelationshipComponent>();

			if (!rc->Children.empty())
			{
				for (auto itr = rc->Children.cbegin(); itr != rc->Children.cend(); itr++)
				{
					Entity child = { *itr };
					m_Scene->m_SceneOrder.remove(child);
					child.RemoveDescendantsFromOrder();
				}
			}
			else
			{
				m_Scene->m_SceneOrder.remove(*this);
			}
		}
		// [Utility Function] Inserts all children into the scene order before pos
		void InsertDescendantsInOrder(std::list<Entity>::const_iterator pos)
		{
			RelationshipComponent* rc = &GetComponent<RelationshipComponent>();
			if (!rc->Children.empty())
			{
				for (auto itr = rc->Children.cbegin(); itr != rc->Children.cend(); itr++)
				{
					Entity child = { *itr };
					if (!child.GetComponent<RelationshipComponent>().Children.empty())
					{
						m_Scene->m_SceneOrder.insert(pos, child);
					}
					child.InsertDescendantsInOrder(pos);

					// Cache Updates
					if (child.HasComponent<SpriteRendererComponent>())
					{
						m_Scene->m_IsSpriteComponentCacheDirty = true;
					}
					// Cache Updates
				}
			}
			else
			{
				m_Scene->m_SceneOrder.insert(pos, *this);

				// Cache Updates
				if (HasComponent<SpriteRendererComponent>())
				{
					m_Scene->m_IsSpriteComponentCacheDirty = true;
				}
				// Cache Updates
			}
		}
		// [Utility Function] Pushes back all children into the scene order
		void PushBackDescendantsInOrder()
		{
			RelationshipComponent* rc = &GetComponent<RelationshipComponent>();
			if (!rc->Children.empty())
			{
				for (auto itr = rc->Children.cbegin(); itr != rc->Children.cend(); itr++)
				{
					Entity child = { *itr };
					if (!child.GetComponent<RelationshipComponent>().Children.empty())
					{
						m_Scene->m_SceneOrder.push_back(child);
					}
					child.PushBackDescendantsInOrder();

					// Cache Updates
					if (child.HasComponent<SpriteRendererComponent>())
					{
						m_Scene->m_IsSpriteComponentCacheDirty = true;
					}
					// Cache Updates
				}
			}
			else
			{
				m_Scene->m_SceneOrder.push_back(*this);

				// Cache Updates
				if (HasComponent<SpriteRendererComponent>())
				{
					m_Scene->m_IsSpriteComponentCacheDirty = true;
				}
				// Cache Updates
			}
		}

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}
