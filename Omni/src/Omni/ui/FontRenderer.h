#pragma once
#include <glm.hpp>

namespace Omni
{
	//struct FontObject
	//{
	//public:
	//	friend class FontRenderer;

	//	std::string m_Text = "";
	//	uint32_t m_FontType = FONT_TYPE_DISTANCE;
	//	//TrueTypeHandle* m_Font;

	//	glm::vec3 m_Transform = { 0.0f, 0.0f, 0.0f };
	//	glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	//	float m_TextScale = 1.0f;
	//	float m_TextSize = 14.0f;

	//	glm::vec4 m_TextColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	//	glm::vec4 m_OutlineColor = {0.0f, 0.0f, 0.0f, 1.0f};
	//	float m_OutlineWidth = 1.0f;
	//	glm::vec4 m_DropShadowColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	//	float m_DropShadowOffsetX = 0.0f;
	//	float m_DropShadowOffsetY = 0.0f;
	//	float m_DropShadowSoftener = 0.0f;

	//	float m_TextScroll = 0.0f;

	//	//TextLineMetrics m_Metrics = TextLineMetrics(FontInfo());
	//	uint32_t m_LineCount;
	//	float m_VisibleLineCount;

	//	void Destroy();

	//private:
	//	//bx::StringView m_BxString = bx::StringView();
	//	//FontHandle m_FontSdf;
	//	//FontHandle m_FontScaled;

	//	//FontManager* m_FontManager;
	//	//TextBufferManager* m_TextBufferManager;

	//	//TextBufferHandle m_ScrollableBuffer;

	//	const char* m_TextBegin;
	//	const char* m_TextEnd;
	//};

	//class FontRenderer
	//{
	//public:

	//	static void InitFontObject(FontObject* font, std::string text, std::string fontPath);

	//	/// <summary>
	//	/// Computes text layout and metrics, call before rendering
	//	/// </summary>
	//	static void ComputeLayout(FontObject* font);
	//	static void RenderFont(FontObject* font, uint32_t viewID, glm::vec3 offset = { 0.0f, 0.0f, 0.0f });

	//private:
	//	//inline static FontManager* m_FontManager;
	//	//inline static TextBufferManager* m_TextBufferManager;

	//	//static TrueTypeHandle loadTtf(FontObject* font, std::string filePath);
	//	static void InitializeFont(FontObject* font, std::string fontPath);
	//	static void InitializeTextBuffer(FontObject* font);
	//	static void ApplyTextBufferAttributes(FontObject* font);
	//};
}
