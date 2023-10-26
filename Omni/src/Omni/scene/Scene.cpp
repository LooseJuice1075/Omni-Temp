#include "ompch.h"
#include "Scene.h"
#include "Entity.h"
#include "math/Math.h"
#include "Components.h"
#include "ScriptableEntity.h"
#include "renderer/Renderer.h"
#include "asset/AssetManager.h"

namespace Omni
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID id, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		m_SceneOrder.push_back(entity);
		entity.AddComponent<UUIDComponent>(id);
		entity.AddComponent<ActiveComponent>();
		auto& tag = entity.AddComponent<NameComponent>();
		tag.Name = name.empty() ? "Entity" : name;
		entity.AddComponent<RelationshipComponent>();
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		RelationshipComponent* rc = &entity.GetComponent<RelationshipComponent>();

		// Cache Updates
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			entity.RemoveComponent<SpriteRendererComponent>();
		}
		// Cache Updates

		if (rc->Parent != entt::null)
		{
			Entity parent = { rc->Parent, this };
			// If parent is marked for deletion, don't remove the entity from its children
			if (std::find(
				m_DeletedEntities.cbegin(),
				m_DeletedEntities.cend(),
				parent) == m_DeletedEntities.cend())
			{
				if (std::find(
					m_RecursivelyDeletedEntities.cbegin(),
					m_RecursivelyDeletedEntities.cend(),
					parent) == m_RecursivelyDeletedEntities.cend())
				{
					parent.GetComponent<RelationshipComponent>().Children.remove(entity);
				}
				else
				{
					m_RecursivelyDeletedEntities.push_back(entity);
				}
			}
			else
			{
				m_RecursivelyDeletedEntities.push_back(entity);
			}
		}

		for (auto child : rc->Children)
		{
			DestroyEntity(child);
		}

		m_SceneOrder.remove(entity);
		m_Registry.destroy(entity);
	}

	void Scene::CalculateEntityTransform(Entity entity)
	{
		if (entity.HasParent())
		{
			auto& tc = entity.GetComponent<TransformComponent>();
			auto& parentTC = entity.GetParent().GetComponent<TransformComponent>();

			tc.Translation = (glm::rotate(glm::quat(-glm::radians(parentTC.Rotation)), tc.LocalTranslation) * parentTC.Scale) + parentTC.Translation;
			tc.Rotation = tc.LocalRotation + parentTC.Rotation;
			tc.Scale = tc.LocalScale * parentTC.Scale;
			tc.isWorldDirty = false;
		}
	}

	void Scene::CalculateEntityLocalTransform(Entity entity)
	{
		if (entity.HasParent())
		{
			auto& tc = entity.GetComponent<TransformComponent>();
			auto& parentTC = entity.GetParent().GetComponent<TransformComponent>();

			tc.LocalTranslation = glm::rotate(glm::quat(glm::radians(parentTC.Rotation)), tc.Translation - parentTC.Translation) / parentTC.Scale;
			tc.LocalRotation = tc.Rotation - parentTC.Rotation;
			tc.LocalScale = tc.Scale / parentTC.Scale;
		}
	}

	void Scene::CalculateDescendantsTransform(Entity entity)
	{
		RelationshipComponent* rc = &entity.GetComponent<RelationshipComponent>();

		for (auto child : rc->Children)
		{
			CalculateEntityTransform(child);
			CalculateDescendantsTransform(child);
		}
	}

	void Scene::CalculateDescendantsLocalTransform(Entity entity)
	{
		RelationshipComponent* rc = &entity.GetComponent<RelationshipComponent>();

		for (auto child : rc->Children)
		{
			CalculateEntityLocalTransform(child);
			CalculateDescendantsLocalTransform(child);
		}
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Sort entity child list to reflect their order in the scene
		{
			for (Entity entity : m_SceneOrder)
			{
				RelationshipComponent* rc = &entity.GetComponent<RelationshipComponent>();
				if (rc->IsChildrenCacheDirty && !rc->Children.empty())
				{
					// Clear child list
					rc->Children.clear();

					// Re-add children in proper order
					for (Entity childEntity : m_SceneOrder)
					{
						if (childEntity.GetParent() == entity)
						{
							rc->Children.push_back(childEntity);
						}
					}

					rc->IsChildrenCacheDirty = false;
				}
			}
		}

		// TransformComponent
		{
			for (auto entity : m_SceneOrder)
			{
				auto& tc = entity.GetComponent<TransformComponent>();
				if (!entity.HasParent())
				{
					tc.LocalTranslation = tc.Translation;
					tc.LocalRotation = tc.Rotation;
					tc.LocalScale = tc.Scale;
				}
				else
				{
					if (tc.isWorldDirty)
					{
						CalculateEntityTransform(entity);
					}
				}
			}
		}

		// Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});
		}

		// CameraComponent
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			bool mainCameraFound = false;
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Camera.GetPosition() != transform.Translation)
					camera.Camera.SetPosition(transform.Translation);

				if (camera.Camera.GetRotation() != transform.Rotation)
					camera.Camera.SetRotation(transform.Rotation);

				if (camera.IsMainCamera && !mainCameraFound)
				{
					m_MainCamera = &camera;
					mainCameraFound = true;
				}
			}
			if (!mainCameraFound)
				m_MainCamera = nullptr;
		}

		if (m_MainCamera)
		{
			RenderCommand::SetViewport(0, 0, m_MainCamera->Camera.GetResolutionX(), m_MainCamera->Camera.GetResolutionY());
			m_MainCamera->Camera.BindFramebuffer();
			Renderer::BeginScene(m_MainCamera->Camera);

			// SpriteRendererComponent
			if (m_IsSpriteComponentCacheDirty)
			{
				OM_PROFILE_SCOPE("Dirty Sprite Cache");

				RenderCommand::EnableDepthTesting(false);

				// Reset cache
				m_SpriteComponentCache.clear();

				for (Entity entity : m_SceneOrder)
				{
					if (entity.HasComponent<SpriteRendererComponent>())
					{
						m_SpriteComponentCache.push_back(entity);
						SpriteRendererComponent* sprite = &entity.GetComponent<SpriteRendererComponent>();

						if (entity.IsActive() && sprite->Active)
						{
							TransformComponent* transform = &entity.GetComponent<TransformComponent>();

							if (sprite->HasTexture)
							{
								if (AssetManager::DoesAssetExist(sprite->TextureAssetID))
								{
									if (AssetManager::IsAssetLoaded(sprite->TextureAssetID))
									{
										Renderer::DrawSprite(
											transform->Translation, transform->Rotation, transform->Scale,
											AssetManager::GetTexture2D(sprite->TextureAssetID), sprite->Color,
											sprite->TilingFactor, sprite->Size,
											sprite->SpritePosition, (int)(uint32_t)entity);
									}
									else
									{
										Renderer::DrawSprite(
											transform->Translation, transform->Rotation, transform->Scale,
											AssetManager::LoadTexture2DAsync(sprite->TextureAssetID), sprite->Color,
											sprite->TilingFactor, sprite->Size,
											sprite->SpritePosition, (int)(uint32_t)entity);
									}
								}
							}
							else
							{
								Renderer::DrawSprite(
									transform->Translation,
									transform->Rotation,
									transform->Scale,
									sprite->Color,
									(int)(uint32_t)entity);
							}
						}
					}
				}

				m_IsSpriteComponentCacheDirty = false;
			}
			else
			{
				OM_PROFILE_SCOPE("Clean Sprite Cache");

				RenderCommand::EnableDepthTesting(false);

				for (Entity entity : m_SpriteComponentCache)
				{
					SpriteRendererComponent* sprite = &entity.GetComponent<SpriteRendererComponent>();

					if (entity.IsActive() && sprite->Active)
					{
						TransformComponent* transform = &entity.GetComponent<TransformComponent>();

						if (sprite->HasTexture)
						{
							if (AssetManager::DoesAssetExist(sprite->TextureAssetID))
							{
								if (AssetManager::IsAssetLoaded(sprite->TextureAssetID))
								{
									Renderer::DrawSprite(
										transform->Translation, transform->Rotation, transform->Scale,
										AssetManager::GetTexture2D(sprite->TextureAssetID), sprite->Color,
										sprite->TilingFactor, sprite->Size,
										sprite->SpritePosition, (int)(uint32_t)entity);
								}
								else
								{
									Renderer::DrawSprite(
										transform->Translation, transform->Rotation, transform->Scale,
										AssetManager::LoadTexture2DAsync(sprite->TextureAssetID), sprite->Color,
										sprite->TilingFactor, sprite->Size,
										sprite->SpritePosition, (int)(uint32_t)entity);
								}
							}
						}
						else
						{
							Renderer::DrawSprite(transform->Translation, transform->Rotation, transform->Scale, sprite->Color, (int)(uint32_t)entity);
						}
					}
				}
			}

			Renderer::EndScene();
			m_MainCamera->Camera.UnbindFramebuffer();
		}

		// Delete all the entities marked for deletion
		{
			for (auto entity : m_DeletedEntities)
			{
				DestroyEntity(entity);
			}

			m_RecursivelyDeletedEntities.clear();
			m_DeletedEntities.clear();
		}
	}

	void Scene::OnFixedUpdate()
	{
		// Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				nsc.Instance->OnFixedUpdate();
			});
		}
	}

	void Scene::RenderScene(Camera& camera)
	{
		Renderer::BeginScene(camera);

		// SpriteRendererComponent
		if (m_IsSpriteComponentCacheDirty)
		{
			OM_PROFILE_SCOPE("Dirty Sprite Cache");

			RenderCommand::EnableDepthTesting(false);

			// Reset cache
			m_SpriteComponentCache.clear();

			for (Entity entity : m_SceneOrder)
			{
				if (entity.HasComponent<SpriteRendererComponent>())
				{
					m_SpriteComponentCache.push_back(entity);
					SpriteRendererComponent* sprite = &entity.GetComponent<SpriteRendererComponent>();

					if (entity.IsActive() && sprite->Active)
					{
						TransformComponent* transform = &entity.GetComponent<TransformComponent>();

						if (sprite->HasTexture)
						{
							if (AssetManager::DoesAssetExist(sprite->TextureAssetID))
							{
								if (AssetManager::IsAssetLoaded(sprite->TextureAssetID))
								{
									Renderer::DrawSprite(
										transform->Translation, transform->Rotation, transform->Scale,
										AssetManager::GetTexture2D(sprite->TextureAssetID), sprite->Color,
										sprite->TilingFactor, sprite->Size,
										sprite->SpritePosition, (int)(uint32_t)entity);
								}
								else
								{
									Renderer::DrawSprite(
										transform->Translation, transform->Rotation, transform->Scale,
										AssetManager::LoadTexture2DAsync(sprite->TextureAssetID), sprite->Color,
										sprite->TilingFactor, sprite->Size,
										sprite->SpritePosition, (int)(uint32_t)entity);
								}
							}
						}
						else
						{
							Renderer::DrawSprite(
								transform->Translation,
								transform->Rotation,
								transform->Scale,
								sprite->Color,
								(int)(uint32_t)entity);
						}
					}
				}
			}

			m_IsSpriteComponentCacheDirty = false;
		}
		else
		{
			OM_PROFILE_SCOPE("Clean Sprite Cache");

			RenderCommand::EnableDepthTesting(false);

			for (Entity entity : m_SpriteComponentCache)
			{
				SpriteRendererComponent* sprite = &entity.GetComponent<SpriteRendererComponent>();

				if (entity.IsActive() && sprite->Active)
				{
					TransformComponent* transform = &entity.GetComponent<TransformComponent>();

					if (sprite->HasTexture)
					{
						if (AssetManager::DoesAssetExist(sprite->TextureAssetID))
						{
							if (AssetManager::IsAssetLoaded(sprite->TextureAssetID))
							{
								Renderer::DrawSprite(
									transform->Translation, transform->Rotation, transform->Scale,
									AssetManager::GetTexture2D(sprite->TextureAssetID), sprite->Color,
									sprite->TilingFactor, sprite->Size,
									sprite->SpritePosition, (int)(uint32_t)entity);
							}
							else
							{
								Renderer::DrawSprite(
									transform->Translation, transform->Rotation, transform->Scale,
									AssetManager::LoadTexture2DAsync(sprite->TextureAssetID), sprite->Color,
									sprite->TilingFactor, sprite->Size,
									sprite->SpritePosition,
									(int)(uint32_t)entity);
							}
						}
					}
					else
					{
						Renderer::DrawSprite(transform->Translation, transform->Rotation, transform->Scale, sprite->Color, (int)(uint32_t)entity);
					}
				}
			}
		}

		Renderer::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}
}
