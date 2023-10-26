#pragma once

#include <core/Core.h>
#include <renderer/Texture.h>

namespace Omni
{
	static class Icons
	{
	public:
		Icons()
		{
			m_DirectoryIcon = Texture2D::Create("resources/asset browser icons/folder_icon.png");
			m_FileIcon = Texture2D::Create("resources/asset browser icons/file_icon.png");
			m_TextureIcon = Texture2D::Create("resources/asset browser icons/texture_icon.png");

			m_BackArrowIcon = Texture2D::Create("resources/asset browser icons/back_arrow_icon.png");

			m_CameraIcon = Texture2D::Create("resources/camera icons/camera_icon.png");
			m_MissingCameraIcon = Texture2D::Create("resources/camera icons/missing_camera_icon.png");
		}

		inline static Ref<Texture2D> m_DirectoryIcon;
		inline static Ref<Texture2D> m_FileIcon;
		inline static Ref<Texture2D> m_TextureIcon;

		inline static Ref<Texture2D> m_BackArrowIcon;

		inline static Ref<Texture2D> m_CameraIcon;
		inline static Ref<Texture2D> m_MissingCameraIcon;
	};
}
