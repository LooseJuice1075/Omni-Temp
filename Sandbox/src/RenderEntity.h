#pragma once

#include <glm.hpp>
#include <Omni/core/Core.h>
#include <Omni/renderer/Renderer.h>
#include <Omni/renderer/Shader.h>
#include <Omni/renderer/Texture.h>
#include <Omni/renderer/Buffer.h>

enum SpriteType
{
	SOLID,
	TEXTURED
};

struct Sprite
{
public:
	int m_Layer;
	std::string m_Name;
	SpriteType m_Type;
	//Omni::Ref<Omni::Texture2D> m_Texture;
	glm::vec4 m_Color;
	glm::vec2 m_SpriteSize;
	int m_SpriteIndex;

	glm::vec3 m_Transform;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	//void Create(int layer, std::string name, Omni::Ref<Omni::Texture2D> texture, glm::vec2 spriteSize, int spriteIndex);
	void Create(int layer, std::string name, glm::vec2 spriteSize, int spriteIndex);
	void Draw(unsigned int viewID, uint32_t texFlags, uint64_t renderState);
	void Destroy();

	bool operator > (const Sprite& sprite) const
	{
		return (m_Layer > sprite.m_Layer);
	}
};
