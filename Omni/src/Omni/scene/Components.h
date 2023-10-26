#pragma once

#include "renderer/Camera.h"
#include "renderer/Texture.h"
#include "asset/AssetManager.h"
#include "Entity.h"

#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Omni
{
	struct UUIDComponent
	{
		UUID id;

		UUIDComponent() = default;
		UUIDComponent(const UUIDComponent&) = default;
	};

	struct ActiveComponent
	{
		bool Active = true;

		ActiveComponent() = default;
		ActiveComponent(const ActiveComponent&) = default;
		ActiveComponent(const bool& active)
			: Active(active) {}
	};

	struct NameComponent
	{
		std::string Name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name)
			: Name(name) {}
	};

	struct RelationshipComponent
	{
		entt::entity Parent{ entt::null };
		std::list<Entity> Children;

		bool IsChildrenCacheDirty = true;

		RelationshipComponent() = default;
	};

	// TODO : Adjust world scale when an object becomes a child so that it does not change size

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::vec3 LocalTranslation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 LocalRotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 LocalScale = { 1.0f, 1.0f, 1.0f };

		bool isWorldDirty = true;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 ToMat4() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		glm::mat4 LocalToMat4() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(LocalRotation));

			return glm::translate(glm::mat4(1.0f), LocalTranslation)
				* rotation
				* glm::scale(glm::mat4(1.0f), LocalScale);
		}
	};

	struct SpriteRendererComponent
	{
		bool Active = true;

		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		bool HasTexture = false;
		UUID TextureAssetID;
		float TilingFactor = 1.0f;
		glm::vec2 Size{ 0.0f, 0.0f };
		glm::vec2 SpritePosition = { 0.0f, 0.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
	};

	struct CameraComponent
	{
		bool Active = true;

		Omni::Camera Camera = Omni::Camera(Omni::CameraType::Orthographic);

		bool IsMainCamera = true; // TODO : Think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	// Forward declaration
	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	//-----------------------------

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents =
		ComponentGroup<
		TransformComponent,
		SpriteRendererComponent,
		CameraComponent,
		NativeScriptComponent
		>;
}
