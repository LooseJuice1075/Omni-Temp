#include "CameraController.h"

#include "Omni.h"

namespace Omni
{
	CameraController::CameraController(CameraType cameraType)
	{
		m_MoveSpeed = 12.0f;
		m_CursorPrevPos = { 0.0f, 0.0f };
		m_OrthoPanningSensitivity = 0.04f;
		m_RotationSpeed = 20.0f;
		m_InDragRotateMode = false;

		m_Camera = Camera(cameraType);
	}

	void CameraController::OnUpdate(Timestep ts)
	{
		float time = ts;
		float fov = GetFOV();
		glm::vec3 position = GetPosition();

		// TODO : Create perspective camera controls

		//---------Orthographic-Controls---------
		if (GetCameraType() == CameraType::Orthographic)
		{
			if (Input::IsKeyPressed(OM_KEY_W))
			{
				position.y += (m_MoveSpeed * fov) * time;
			}
			else if (Input::IsKeyPressed(OM_KEY_S))
			{
				position.y -= (m_MoveSpeed * fov) * time;
			}

			if (Input::IsKeyPressed(OM_KEY_D))
			{
				position.x += (m_MoveSpeed * fov) * time;
			}
			else if (Input::IsKeyPressed(OM_KEY_A))
			{
				position.x -= (m_MoveSpeed * fov) * time;
			}

			if (Input::IsMouseButtonPressed(OM_MOUSE_BUTTON_MIDDLE)
				|| Input::IsMouseButtonPressed(OM_MOUSE_BUTTON_RIGHT))
			{
				position.x -= ((Input::GetMouseX() - m_CursorPrevPos.x) * fov) * m_OrthoPanningSensitivity;
				position.y += ((Input::GetMouseY() - m_CursorPrevPos.y) * fov) * m_OrthoPanningSensitivity;
			}
		}
		else if (GetCameraType() == CameraType::Perspective)
		{
			if (Input::IsKeyPressed(OM_KEY_W))
			{
				position += (m_MoveSpeed* GetForwardDirection()) * time;
			}
			else if (Input::IsKeyPressed(OM_KEY_S))
			{
				position -= (m_MoveSpeed * GetForwardDirection()) * time;
			}

			if (Input::IsKeyPressed(OM_KEY_D))
			{
				position += (m_MoveSpeed * GetRightDirection()) * time;
			}
			else if (Input::IsKeyPressed(OM_KEY_A))
			{
				position -= (m_MoveSpeed * GetRightDirection()) * time;
			}

			if (Input::IsMouseButtonPressed(OM_MOUSE_BUTTON_MIDDLE)
				|| Input::IsMouseButtonPressed(OM_MOUSE_BUTTON_RIGHT))
			{
				glm::vec2 delta = { (Input::GetMouseX() - m_CursorPrevPos.x) * 0.003f, (Input::GetMouseY() - m_CursorPrevPos.y) * 0.003f };

				float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
				float yaw = GetYaw();
				float pitch = GetPitch();
				SetRotation(GetPitch() + (delta.y * m_RotationSpeed), GetYaw() + (yawSign * delta.x * m_RotationSpeed), GetRoll());
			}
		}

		SetPosition(position);
		//---------Orthographic-Controls---------

		m_CursorPrevPos.x = Input::GetMouseX();
		m_CursorPrevPos.y = Input::GetMouseY();
	}

	bool CameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (GetCameraType() == CameraType::Perspective)
		{
			if (e.GetMouseButton() == OM_MOUSE_BUTTON_MIDDLE ||
				e.GetMouseButton() == OM_MOUSE_BUTTON_RIGHT)
			{
				Application::Get().GetWindow().SetCursorMode(CursorMode::LockedAndHidden);
				m_InDragRotateMode = true;
			}
		}

		return false;
	}

	bool CameraController::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (e.GetMouseButton() == OM_MOUSE_BUTTON_MIDDLE ||
			e.GetMouseButton() == OM_MOUSE_BUTTON_RIGHT)
		{
			Application::Get().GetWindow().SetCursorMode(CursorMode::Normal);
			m_InDragRotateMode = false;
		}

		return false;
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		if (GetCameraType() == CameraType::Orthographic)
		{
			float fov = GetFOV();
			SetFOV(fov - (e.GetYOffset() * 0.1f) * fov);
		}

		return false;
	}
}
