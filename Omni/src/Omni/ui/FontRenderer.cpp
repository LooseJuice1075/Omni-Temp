#include "ompch.h"
#include "FontRenderer.h"
#include "renderer/Renderer.h"
#include "core/Omni_Utils.h"

namespace Omni
{
	//void FontRenderer::InitFontObject(FontObject* font, std::string text, std::string fontPath)
	//{
	//	font->m_Text = text;
	//	font->m_BxString = bx::StringView(font->m_Text.c_str(), strlen(font->m_Text.c_str()));

	//	// Init the text rendering system.
	//	font->m_FontManager = NULL;
	//	font->m_TextBufferManager = NULL;
	//	font->m_Font = new TrueTypeHandle();
	//	*font->m_Font = BGFX_INVALID_HANDLE;
	//	font->m_FontSdf = BGFX_INVALID_HANDLE;
	//	font->m_FontScaled = BGFX_INVALID_HANDLE;
	//	font->m_ScrollableBuffer = BGFX_INVALID_HANDLE;

	//	InitializeFont(font, fontPath);

	//	font->m_LineCount = font->m_Metrics.getLineCount(font->m_BxString);

	//	font->m_VisibleLineCount = 20.0f;

	//	font->m_TextBegin = 0;
	//	font->m_TextEnd = 0;
	//	font->m_Metrics.getSubText(font->m_BxString, 0, (uint32_t)font->m_VisibleLineCount, font->m_TextBegin, font->m_TextEnd);

	//	InitializeTextBuffer(font);

	//	ApplyTextBufferAttributes(font);

	//	font->m_TextBufferManager->appendText(font->m_ScrollableBuffer, font->m_FontScaled, font->m_TextBegin, font->m_TextEnd);
	//}

	//TrueTypeHandle FontRenderer::loadTtf(FontObject* font, std::string filePath)
	//{
	//	std::string fullPath = "fonts/" + filePath;
	//	uint32_t size;
	//	void* data = loadFileBinary(fullPath, &size);

	//	if (data != NULL)
	//	{
	//		TrueTypeHandle handle = font->m_FontManager->createTtf((uint8_t*)data, size);
	//		return handle;
	//	}

	//	TrueTypeHandle invalid = BGFX_INVALID_HANDLE;
	//	OM_CORE_ERROR("[FONT ERROR]: Font \"{0}\" could not be loaded!", fullPath);
	//	return invalid;
	//}

	//void FontRenderer::InitializeFont(FontObject* font, std::string fontPath)
	//{
	//	if (font->m_Font->idx != bgfx::kInvalidHandle)
	//	{
	//		font->m_FontManager->destroyTtf(*font->m_Font);
	//	}

	//	if (font->m_ScrollableBuffer.idx != bgfx::kInvalidHandle)
	//	{
	//		font->m_TextBufferManager->destroyTextBuffer(font->m_ScrollableBuffer);
	//	}

	//	if (font->m_FontScaled.idx != bgfx::kInvalidHandle)
	//	{
	//		font->m_FontManager->destroyFont(font->m_FontScaled);
	//	}
	//	if (font->m_FontSdf.idx != bgfx::kInvalidHandle)
	//	{
	//		font->m_FontManager->destroyFont(font->m_FontSdf);
	//	}

	//	delete font->m_TextBufferManager;
	//	delete font->m_FontManager;

	//	font->m_FontManager = new FontManager(512);
	//	font->m_TextBufferManager = new TextBufferManager(font->m_FontManager);

	//	font->m_Font = &loadTtf(font, fontPath);

	//	font->m_FontSdf = font->m_FontManager->createFontByPixelSize(*font->m_Font, 0, 48, font->m_FontType, 6 + 2, 6 + 2);

	//	font->m_FontScaled = font->m_FontManager->createScaledFontToPixelSize(font->m_FontSdf, (uint32_t)font->m_TextSize);

	//	font->m_Metrics = TextLineMetrics(font->m_FontManager->getFontInfo(font->m_FontScaled));
	//}

	//void FontRenderer::InitializeTextBuffer(FontObject* font)
	//{
	//	font->m_ScrollableBuffer = font->m_TextBufferManager->createTextBuffer(font->m_FontType, BufferType::Transient);
	//}

	//void FontRenderer::ApplyTextBufferAttributes(FontObject* font)
	//{
	//	font->m_TextBufferManager->setTextColor(font->m_ScrollableBuffer, rgbaToHex(font->m_TextColor));
	//	font->m_TextBufferManager->setOutlineColor(font->m_ScrollableBuffer, (rgbaToHex(font->m_OutlineColor)));
	//	font->m_TextBufferManager->setOutlineWidth(font->m_ScrollableBuffer, font->m_OutlineWidth);
	//	font->m_TextBufferManager->setDropShadowColor(font->m_ScrollableBuffer, (rgbaToHex(font->m_DropShadowColor)));
	//	font->m_TextBufferManager->setDropShadowOffset(font->m_ScrollableBuffer, font->m_DropShadowOffsetX, font->m_DropShadowOffsetY);
	//	font->m_TextBufferManager->setDropShadowSoftener(font->m_ScrollableBuffer, font->m_DropShadowSoftener);
	//}

	//void FontRenderer::ComputeLayout(FontObject* font)
	//{
	//	bool recomputeVisibleText = false;

	//	if (recomputeVisibleText)
	//	{
	//		font->m_TextBufferManager->clearTextBuffer(font->m_ScrollableBuffer);

	//		ApplyTextBufferAttributes(font);

	//		font->m_Metrics.getSubText(font->m_BxString, (uint32_t)font->m_TextScroll, (uint32_t)(font->m_TextScroll + font->m_VisibleLineCount), font->m_TextBegin, font->m_TextEnd);
	//		font->m_TextBufferManager->appendText(font->m_ScrollableBuffer, font->m_FontScaled, font->m_TextBegin, font->m_TextEnd);
	//	}

	//	float centering = 0.0f;
	//	if (bgfx::getRendererType() == bgfx::RendererType::Direct3D9) {
	//		centering = -0.5f;
	//	}
	//}

	//void FontRenderer::RenderFont(FontObject* font,uint32_t viewID, glm::vec3 offset)
	//{
	//	float textMtx[16];
	//	// 180 is added to correct text rotation
	//	bx::mtxSRT(textMtx, font->m_TextScale, font->m_TextScale, 1.0f,
	//		glm::radians(font->m_Rotation.x + 180.0f), glm::radians(font->m_Rotation.y), glm::radians(font->m_Rotation.z),
	//		font->m_Transform.x + offset.x, font->m_Transform.y + offset.y, font->m_Transform.z + offset.z);

	//	bgfx::setTransform(textMtx);

	//	font->m_TextBufferManager->submitTextBuffer(font->m_ScrollableBuffer, viewID);
	//}

	//void FontObject::Destroy()
	//{
	//}

}
