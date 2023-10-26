#pragma once

#include "core/Timestep.h"
#include "renderer/Camera.h"
#include "events/Event.h"
#include "events/MouseEvent.h"

namespace Omni
{
	class CameraController
	{
	public:

		CameraController() = default;
		CameraController(CameraType cameraType);

		void OnUpdate(Timestep ts);

		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

		bool IsInDragRotateMode() const { return m_InDragRotateMode; }

		Camera GetCamera() const { return m_Camera; }

		CameraType GetCameraType() const { return m_Camera.GetCameraType(); }
		void SetCameraType(CameraType cameraType) { m_Camera.SetCameraType(cameraType); }

		float GetMoveSpeed() const { return m_MoveSpeed; }
		void SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; }

		float GetOrthoPanningSensitivity() const { return m_OrthoPanningSensitivity; }
		void SetOrthoPanningSensitivity(float orthoPanningSensitivity) { m_OrthoPanningSensitivity = orthoPanningSensitivity; }

		float GetRotationSpeed() const { return m_RotationSpeed; }
		void SetRotationSpeed(float rotationSpeed) { m_RotationSpeed = rotationSpeed; }

		float GetFOV() const { return m_Camera.GetFOV(); }
		void SetFOV(float fov) { m_Camera.SetFOV(fov); }

		float GetNearClip() const { return m_Camera.GetNearClip(); }
		void SetNearClip(float nearClip) { m_Camera.SetNearClip(nearClip); }

		float GetFarClip() const { return m_Camera.GetFarClip(); }
		void SetFarClip(float farClip) { m_Camera.SetFarClip(farClip); }

		uint32_t GetResolutionX() const { return m_Camera.GetResolutionX(); }
		uint32_t GetResolutionY() const { return m_Camera.GetResolutionX(); }
		void SetResolution(uint32_t resolutionX, uint32_t resolutionY)
		{ m_Camera.SetResolution(resolutionX, resolutionY); }

		const glm::vec3& GetPosition() const { return m_Camera.GetPosition(); }
		void SetPosition(const glm::vec3& position) { m_Camera.SetPosition(position); }

		float GetPitch() const { return m_Camera.GetPitch(); }
		float GetYaw() const { return m_Camera.GetYaw(); }
		float GetRoll() const { return m_Camera.GetRoll(); }
		void SetRotation(const glm::vec3& rotation) { m_Camera.SetRotation(rotation); }
		void SetRotation(float pitch, float yaw, float roll) { m_Camera.SetRotation(pitch, yaw, roll); }

		const glm::mat4& GetProjectionMatrix() const { return m_Camera.GetProjectionMatrix(); }
		const glm::mat4& GetViewMatrix() const { return m_Camera.GetViewMatrix(); }
		const glm::mat4& GetViewProjectionMatrix() const { return m_Camera.GetViewProjectionMatrix(); }

		glm::vec3 GetUpDirection() const { return m_Camera.GetUpDirection(); }
		glm::vec3 GetRightDirection() const { return m_Camera.GetRightDirection(); }
		glm::vec3 GetForwardDirection() const { return m_Camera.GetForwardDirection(); }
		glm::quat GetOrientation() const { return m_Camera.GetOrientation(); }

	private:

		float m_MoveSpeed;
		float m_OrthoPanningSensitivity;
		float m_RotationSpeed;
		glm::vec2 m_CursorPrevPos;
		bool m_InDragRotateMode;

		Camera m_Camera;
	};
}
