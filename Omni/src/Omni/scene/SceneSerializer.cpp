#include "ompch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "asset/AssetManager.h"

#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Omni
{
	#pragma region Operators
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}
	#pragma endregion

	Omni::SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		// ActiveComponent
		{
			out << YAML::Key << "Active" << YAML::Value << entity.IsActive();
		}

		// NameComponent
		{
			out << YAML::Key << "Name" << YAML::Value << entity.GetName();
		}

		// RelationshipComponent
		{
			RelationshipComponent* rc = &entity.GetComponent<RelationshipComponent>();

			out << YAML::Key << "RelationshipComponent";
			out << YAML::BeginMap;

			if (entity.HasParent())
			{
				out << YAML::Key << "Parent" << YAML::Value << entity.GetParent().GetUUID();
			}
			else
			{
				out << YAML::Key << "Parent" << YAML::Value << "NULL";
			}

			out << YAML::Key << "Children";
			out << YAML::BeginMap;

			for (auto itr = rc->Children.cbegin(); itr != rc->Children.cend(); itr++)
			{
				Entity child = { *itr };
				out << YAML::Key << "Entity" << YAML::Value << child.GetUUID();
			}

			out << YAML::EndMap;

			out << YAML::EndMap;
		}

		// TransformComponent
		{
			TransformComponent* tc = &entity.GetComponent<TransformComponent>();

			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Transform" << YAML::Value << tc->Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc->Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc->Scale;

			out << YAML::Key << "LocalTransform" << YAML::Value << tc->LocalTranslation;
			out << YAML::Key << "LocalRotation" << YAML::Value << tc->LocalRotation;
			out << YAML::Key << "LocalScale" << YAML::Value << tc->LocalScale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			SpriteRendererComponent* sprc = &entity.GetComponent<SpriteRendererComponent>();

			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Active" << YAML::Value << sprc->Active;
			out << YAML::Key << "Color" << YAML::Value << sprc->Color;
			if (sprc->HasTexture)
			{
				out << YAML::Key << "Texture" << YAML::Value << sprc->TextureAssetID;
			}
			else
			{
				out << YAML::Key << "Texture" << YAML::Value << "";
			}
			out << YAML::Key << "TilingFactor" << YAML::Value << sprc->TilingFactor;
			out << YAML::Key << "Size" << YAML::Value << sprc->Size;
			out << YAML::Key << "SpritePosition" << YAML::Value << sprc->SpritePosition;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			CameraComponent* cc = &entity.GetComponent<CameraComponent>();

			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Active" << YAML::Value << cc->Active;

			// Camera class
			out << YAML::Key << "FOV" << YAML::Value << cc->Camera.GetFOV();
			out << YAML::Key << "NearClip" << YAML::Value << cc->Camera.GetNearClip();
			out << YAML::Key << "FarClip" << YAML::Value << cc->Camera.GetFarClip();
			out << YAML::Key << "ResolutionX" << YAML::Value << cc->Camera.GetResolutionX();
			out << YAML::Key << "ResolutionY" << YAML::Value << cc->Camera.GetResolutionY();
			if (cc->Camera.GetCameraType() == CameraType::Orthographic)
			{
				out << YAML::Key << "Type" << YAML::Value << "Orthographic";
			}
			else if (cc->Camera.GetCameraType() == CameraType::Perspective)
			{
				out << YAML::Key << "Type" << YAML::Value << "Perspective";
			}
			// Camera class

			out << YAML::Key << "IsMainCamera" << YAML::Value << cc->IsMainCamera;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc->FixedAspectRatio;

			out << YAML::EndMap;
		}

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "New Scene";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		for (auto itr = m_Scene->m_SceneOrder.cbegin(); itr != m_Scene->m_SceneOrder.cend(); itr++)
		{
			Entity entity = {*itr};
			if (!entity.IsValid())
				return;

			SerializeEntity(out, entity);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		if (fout.is_open())
		{
			fout << out.c_str();
			fout.close();
		}
		else
		{
			OM_CORE_ERROR("[SceneSerializer::Serialize] Could not save scene to \"{0}\"!", filepath);
		}
	}

	void SceneSerializer::SerializeBinary(const std::string& filepath)
	{
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t id = entity["Entity"].as<uint64_t>();
				bool active = entity["Active"].as<bool>();
				std::string name = entity["Name"].as<std::string>();

				Entity decodedEntity = m_Scene->CreateEntityWithUUID(id, name);
				decodedEntity.SetActive(active);

				// TransformComponent
				{
					auto transformComponent = entity["TransformComponent"];
					if (transformComponent)
					{
						TransformComponent* tc = &decodedEntity.GetComponent<TransformComponent>();
						tc->Translation = transformComponent["Transform"].as<glm::vec3>();
						tc->Rotation = transformComponent["Rotation"].as<glm::vec3>();
						tc->Scale = transformComponent["Scale"].as<glm::vec3>();

						tc->LocalTranslation = transformComponent["LocalTransform"].as<glm::vec3>();
						tc->LocalRotation = transformComponent["LocalRotation"].as<glm::vec3>();
						tc->LocalScale = transformComponent["LocalScale"].as<glm::vec3>();
					}
				}

				// RelationshipComponent
				{
					auto relationshipComponent = entity["RelationshipComponent"];
					if (relationshipComponent)
					{
						RelationshipCache rc;
						rc.entity = decodedEntity;
						if (relationshipComponent["Parent"].as<std::string>() != "NULL")
						{
							rc.HasParent = true;
							rc.Parent = relationshipComponent["Parent"].as<uint64_t>();
						}
						else
						{
							rc.HasParent = false;
						}

						auto children = relationshipComponent["Children"];
						for (auto itr = children.begin(); itr != children.end(); itr++)
						{
							rc.Children.push_back(itr->second.as<uint64_t>());
						}
						
						m_RelationshipCaches.push_back(rc);
					}
				}

				// SpriteRendererComponent
				{
					auto spriteComponent = entity["SpriteRendererComponent"];
					if (spriteComponent)
					{
						SpriteRendererComponent* sprc = &decodedEntity.AddComponent<SpriteRendererComponent>();
						sprc->Active = spriteComponent["Active"].as<bool>();
						sprc->Color = spriteComponent["Color"].as<glm::vec4>();
						std::string texturePath = spriteComponent["Texture"].as<std::string>();
						if (texturePath != "")
						{
							uint64_t textureID = spriteComponent["Texture"].as<uint64_t>();
							if (AssetManager::DoesAssetExist(textureID))
							{
								sprc->TextureAssetID = textureID;
								sprc->HasTexture = true;
								if (!AssetManager::IsAssetLoaded(textureID))
								{
									AssetManager::LoadTexture2DAsync(textureID);
								}
							}
							else
							{
								OM_CORE_ERROR("[SceneSerializer::Deserialize] Could not find texture with Asset ID \"{0}\"!", textureID);
							}
						}
						sprc->TilingFactor = spriteComponent["TilingFactor"].as<float>();
						sprc->Size = spriteComponent["Size"].as<glm::vec2>();
						sprc->SpritePosition = spriteComponent["SpritePosition"].as<glm::vec2>();
					}
					
				}

				// CameraComponent
				{
					auto cameraComponent = entity["CameraComponent"];
					if (cameraComponent)
					{
						CameraComponent* cc = &decodedEntity.AddComponent<CameraComponent>();
						cc->Active = cameraComponent["Active"].as<bool>();
						cc->Camera.SetFOV(cameraComponent["FOV"].as<float>());
						cc->Camera.SetNearClip(cameraComponent["NearClip"].as<float>());
						cc->Camera.SetFarClip(cameraComponent["FarClip"].as<float>());
						cc->Camera.SetResolutionX(cameraComponent["ResolutionX"].as<uint32_t>());
						cc->Camera.SetResolutionY(cameraComponent["ResolutionY"].as<uint32_t>());
						if (cameraComponent["Type"].as<std::string>() == "Orthographic")
						{
							cc->Camera.SetCameraType(CameraType::Orthographic);
						}
						else if (cameraComponent["Type"].as<std::string>() == "Perspective")
						{
							cc->Camera.SetCameraType(CameraType::Perspective);
						}
						cc->IsMainCamera = cameraComponent["IsMainCamera"].as<bool>();
						cc->FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
					}
				}
			}
		}

		// Fulfill Relationship Caches
		for (auto rsc : m_RelationshipCaches)
		{
			RelationshipComponent* rc = &rsc.entity.GetComponent<RelationshipComponent>();
			if (rsc.HasParent)
			{
				// Find parent with matching uuid
				bool parentFound = false;
				for (auto entity : m_Scene->m_SceneOrder)
				{
					if (entity.GetUUID() == rsc.Parent)
					{
						parentFound = true;
						rc->Parent = entity;
						break;
					}
				}
				if (!parentFound)
					OM_CORE_ERROR("[DESERIALIZATION ERROR]: Could not locate parent of entity \"{0}\"!", rsc.entity.GetName());
			}

			for (auto child : rsc.Children)
			{
				// Find chilren with matching uuid
				for (auto entity : m_Scene->m_SceneOrder)
				{
					if (entity.GetUUID() == child)
					{
						rc->Children.push_back(entity);
					}
				}
			}
		}

		m_RelationshipCaches.clear();

		return true;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filepath)
	{
		return false;
	}
}
