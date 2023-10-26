#pragma once

#include "core/Core.h"
#include "Shader.h"

#include <string>
#include <stb/stb_image.h>

namespace Omni
{
	enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	enum class TextureFilteringMode
	{
		Nearest = 0,
		Linear = 1,
	};

	enum class TextureWrapMode
	{
		Repeat = 0,
		Clamp = 1,
	};

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;

		ImageFormat Format = ImageFormat::RGBA8;

		TextureFilteringMode MinFilter = TextureFilteringMode::Nearest;
		TextureFilteringMode MagFilter = TextureFilteringMode::Nearest;

		TextureWrapMode WrapMode = TextureWrapMode::Repeat;

		bool GenerateMips = true;
	};

	struct Texture2DData
	{
		stbi_uc* Memory;

		uint32_t Width = 1;
		uint32_t Height = 1;
		uint32_t Channels = 1;

		ImageFormat Format = ImageFormat::RGBA8;

		TextureFilteringMode MinFilter = TextureFilteringMode::Nearest;
		TextureFilteringMode MagFilter = TextureFilteringMode::Nearest;

		TextureWrapMode WrapMode = TextureWrapMode::Repeat;

		bool GenerateMips = true;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual const TextureSpecification& GetSpecification() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual const std::string& GetPath() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const TextureSpecification& specification);
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Generate(Texture2DData* data);
	};
}
