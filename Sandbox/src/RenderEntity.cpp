#include "RenderEntity.h"
#include "core/Core.h"

//void Sprite::Create(int layer, std::string name, Omni::Ref<Omni::Texture2D> texture, glm::vec2 spriteSize, int spriteIndex)
//{
//	m_Layer = layer;
//	m_Name = name;
//	m_Type = SpriteType::TEXTURED;
//	m_Texture = texture;
//	m_SpriteSize = spriteSize;
//	m_SpriteIndex = spriteIndex;
//	m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
//	m_SpriteSize = spriteSize;
//	m_Transform = { 0.0f, 0.0f, 0.0f };
//	m_Rotation = { 0.0f, 0.0f, 0.0f };
//	m_Scale = { 1.0f, 1.0f, 1.0f };
//}

void Sprite::Create(int layer, std::string name, glm::vec2 spriteSize, int spriteIndex)
{
	m_Layer = layer;
	m_Name = name;
	m_Type = SpriteType::SOLID;
	m_SpriteSize = spriteSize;
	m_SpriteIndex = spriteIndex;
	m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_SpriteSize = spriteSize;
	m_Transform = { 0.0f, 0.0f, 0.0f };
	m_Rotation = { 0.0f, 0.0f, 0.0f };
	m_Scale = { 1.0f, 1.0f, 1.0f };
}

void Sprite::Draw(unsigned int viewID, uint32_t texFlags, uint64_t renderState)
{
	if (m_Type == SpriteType::TEXTURED)
	{
		//Omni::Renderer::DrawSprite(viewID, texFlags, m_Texture, m_SpriteSize, m_SpriteIndex, m_Color,
		//	m_Transform, m_Rotation, m_Scale, renderState);
	}
	else if (m_Type == SpriteType::SOLID)
	{
		//Omni::Renderer::DrawSpriteSolid(viewID, m_Color, m_Transform, m_Rotation, m_Scale, renderState);
	}
}

void Sprite::Destroy()
{
}
