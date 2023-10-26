#include "ompch.h"
#include "platform/OpenGL/OpenGLTexture.h"

namespace Omni
{
	namespace Utils {

		static GLenum OmniImageFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:  return GL_RGB;
			case ImageFormat::RGBA8: return GL_RGBA;
			}

			OM_CORE_ASSERT(false, "");
			return 0;
		}

		static GLenum OmniImageFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:  return GL_RGB8;
			case ImageFormat::RGBA8: return GL_RGBA8;
			}

			OM_CORE_ASSERT(false, "");
			return 0;
		}

		static GLenum OmniTextureFilterToGLenum(TextureFilteringMode filteringMode)
		{
			switch (filteringMode)
			{
			case TextureFilteringMode::Nearest: return GL_NEAREST;
			case TextureFilteringMode::Linear: return GL_LINEAR;
			}

			OM_CORE_ASSERT(false, "");
			return 0;
		}

		static GLenum OmniTextureFilterToGLenumMip(TextureFilteringMode filteringMode)
		{
			switch (filteringMode)
			{
			case TextureFilteringMode::Nearest: return GL_NEAREST_MIPMAP_NEAREST;
			case TextureFilteringMode::Linear: return GL_LINEAR_MIPMAP_LINEAR;
			}

			OM_CORE_ASSERT(false, "");
			return 0;
		}

		static GLenum OmniTextureWrapModeToGLenum(TextureWrapMode wrapMode)
		{
			switch (wrapMode)
			{
			case TextureWrapMode::Repeat: return GL_REPEAT;
			case TextureWrapMode::Clamp: return GL_CLAMP_TO_EDGE;
			}

			OM_CORE_ASSERT(false, "");
			return 0;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height)
	{
		OM_PROFILE_FUNCTION();

		m_InternalFormat = Utils::OmniImageFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::OmniImageFormatToGLDataFormat(m_Specification.Format);
		GLenum minFilter = Utils::OmniTextureFilterToGLenum(m_Specification.MinFilter);
		GLenum magFilter = Utils::OmniTextureFilterToGLenum(m_Specification.MagFilter);
		GLenum wrapMode = Utils::OmniTextureWrapModeToGLenum(m_Specification.WrapMode);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, minFilter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, magFilter);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, wrapMode);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, wrapMode);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		OM_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			OM_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		if (data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			OM_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			OM_CORE_ERROR("Could not load texture from path \"{0}\"!", path);
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(Texture2DData* data)
	{
		m_IsLoaded = true;

		m_Width = data->Width;
		m_Height = data->Height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (data->Channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (data->Channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		OM_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		GLenum minFilter, magFilter;
		GLenum wrapMode = Utils::OmniTextureWrapModeToGLenum(data->WrapMode);
		uint32_t mipLevel = 1;

		if (!data->GenerateMips)
		{
			minFilter = Utils::OmniTextureFilterToGLenum(data->MinFilter);
			magFilter = Utils::OmniTextureFilterToGLenum(data->MagFilter);
		}
		else
		{
			minFilter = Utils::OmniTextureFilterToGLenumMip(data->MinFilter);
			magFilter = Utils::OmniTextureFilterToGLenum(data->MagFilter);
			mipLevel = 3;
		}

		glCreateTextures(GL_TEXTURE_2D, mipLevel, &m_RendererID);
		glTextureStorage2D(m_RendererID, mipLevel, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, minFilter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, magFilter);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, wrapMode);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, wrapMode);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data->Memory);
		if (data->GenerateMips)
			glGenerateTextureMipmap(m_RendererID);

		stbi_image_free(data->Memory);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		OM_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		OM_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		OM_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		OM_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}
}
