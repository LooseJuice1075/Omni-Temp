#include "ompch.h"

#include "Camera.h"
#include "Renderer.h"
#include "core/Application.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Omni
{
	Camera::Camera(CameraType cameraType)
		: m_CameraType(cameraType), m_ViewMatrix(1.0f)
	{
		OM_PROFILE_FUNCTION();

		if (cameraType == CameraType::Orthographic)
		{
			m_FOV = 10.0f;
			m_NearClip = -1.0f;
			m_FarClip = 1000.0f;
		}
		else if (cameraType == CameraType::Perspective)
		{
			m_FOV = 60.0f;
			m_NearClip = 0.01f;
			m_FarClip = 1000.0f;
		}

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);
	}

	void Camera::SetProjection()
	{
		OM_PROFILE_FUNCTION();

		if (m_CameraType == CameraType::Orthographic)
		{
			float aspectX = (float)m_ResolutionX * 0.01f;
			float aspectY = (float)m_ResolutionY * 0.01f;

			m_ProjectionMatrix = glm::ortho(
				-(aspectX * 2) * m_FOV,
				(aspectX * 2) * m_FOV,
				-(aspectY * 2) * m_FOV,
				(aspectY * 2) * m_FOV,
				m_NearClip,
				m_FarClip
			);

			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
		else if (m_CameraType == CameraType::Perspective)
		{
			float aspectRatio = (float)m_ResolutionX / (float)m_ResolutionY;
			m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), aspectRatio, m_NearClip, m_FarClip);

			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
	}

	void Camera::SetCameraType(CameraType cameraType)
	{
		m_CameraType = cameraType;
	}

	glm::vec3 Camera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 Camera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 Camera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat Camera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-glm::radians(m_Pitch), -glm::radians(m_Yaw), glm::radians(m_Roll)));
	}

	void Camera::RecalculateViewMatrix()
	{
		OM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::toMat4(GetOrientation());
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
