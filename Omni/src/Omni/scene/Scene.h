#pragma once

#include "core/Timestep.h"
#include "core/UUID.h"
#include "renderer/Camera.h"

#include <entt.hpp>

namespace Omni
{
	class Entity;
	class CameraComponent;

	class Scene
	{
	public:

		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID id, const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		void CalculateEntityTransform(Entity entity);
		void CalculateEntityLocalTransform(Entity entity);
		void CalculateDescendantsTransform(Entity entity);
		void CalculateDescendantsLocalTransform(Entity entity);

		CameraComponent* GetMainCamera() { return m_MainCamera; }

		void OnUpdate(Timestep ts);
		void OnFixedUpdate();
		void OnViewportResize(uint32_t width, uint32_t height);

		// Made for the editor camera
		void RenderScene(Camera& camera);

	private:

		entt::registry m_Registry;
		std::list<Entity> m_SceneOrder;
		// All of the entities marked for deletion
		std::list<Entity> m_DeletedEntities;
		std::list<Entity> m_RecursivelyDeletedEntities;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		CameraComponent* m_MainCamera = nullptr;

		//------Component-Caches------

		bool m_IsSpriteComponentCacheDirty = true;
		std::list<Entity> m_SpriteComponentCache;

		//------Component-Caches------

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
		friend class PropertiesPanel;
	};
}
