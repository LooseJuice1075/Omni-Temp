#pragma once

#include "core/Core.h"
#include "Scene.h"
#include "Entity.h"

namespace Omni
{
	struct RelationshipCache
	{
		Entity entity;

		bool HasParent = false;
		UUID Parent;
		std::list<UUID> Children;
	};

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeBinary(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeBinary(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;
		std::list<RelationshipCache> m_RelationshipCaches;
	};
}
