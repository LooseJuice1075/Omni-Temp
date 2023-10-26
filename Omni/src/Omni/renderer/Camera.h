#pragma once

#include "core/Core.h"
#include "Framebuffer.h"

#include <glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Omni
{
	enum class CameraType
	{
		Orthographic = 0,
		Perspective  = 1,
	};

	class Camera
	{
	public:

		Camera() = default;
		Camera(CameraType cameraType);

		void SetProjection();

		CameraType GetCameraType() const { return m_CameraType; }
		void SetCameraType(CameraType cameraType);

		float GetFOV() const { return m_FOV; }
		void SetFOV(float fov) { m_FOV = fov; SetProjection(); }

		float GetNearClip() const { return m_NearClip; }
		void SetNearClip(float nearClip) { m_NearClip = nearClip; SetProjection(); }

		float GetFarClip() const { return m_FarClip; }
		void SetFarClip(float farClip) { m_FarClip = farClip; SetProjection(); }

		uint32_t GetResolutionX() const { return m_ResolutionX; }
		uint32_t GetResolutionY() const { return m_ResolutionY; }
		void SetResolutionX(uint32_t resolutionX) { m_ResolutionX = resolutionX; SetProjection(); }
		void SetResolutionY(uint32_t resolutionY) { m_ResolutionY = resolutionY; SetProjection(); }
		void SetResolution(uint32_t resolutionX, uint32_t resolutionY)
		{
			m_ResolutionX = resolutionX;
			m_ResolutionY = resolutionY;
			SetProjection();
		}

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
		float GetRoll() const { return m_Roll; }
		glm::vec3 GetRotation() { return { m_Pitch, m_Yaw, m_Roll }; }
		void SetRotation(const glm::vec3& rotation)
		{
			m_Pitch = rotation.x;
			m_Yaw = rotation.y;
			m_Roll = rotation.z;
			RecalculateViewMatrix();
		}
		void SetRotation(float pitch, float yaw, float roll)
		{
			m_Pitch = pitch;
			m_Yaw = yaw;
			m_Roll = roll;
			RecalculateViewMatrix();
		}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		glm::quat GetOrientation() const;

		Ref<Framebuffer> GetFramebuffer() { return m_Framebuffer; }
		void BindFramebuffer() { m_Framebuffer->Bind(); }
		void UnbindFramebuffer() { m_Framebuffer->Unbind(); }

	private:

		void RecalculateViewMatrix();

	private:

		CameraType m_CameraType;

		float m_FOV;

		float m_NearClip = 0.01f;
		float m_FarClip = 1000.0f;

		uint32_t m_ResolutionX = 1280;
		uint32_t m_ResolutionY = 720;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;
		float m_Roll = 0.0f;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		Ref<Framebuffer> m_Framebuffer;
	};
}
