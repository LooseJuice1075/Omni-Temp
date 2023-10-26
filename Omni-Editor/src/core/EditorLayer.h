#pragma once

#include "Omni.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "camera/CameraController.h"
#include "panels/SceneHierarchyPanel.h"
#include "panels/PropertiesPanel.h"
#include "panels/AssetBrowserPanel.h"

namespace Omni
{
	class EditorLayer : public Layer
	{
	public:

		EditorLayer();
		~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		void OnFixedUpdate() override;
		virtual void OnImGuiRender() override;

		void OnEvent(Omni::Event& e) override;

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);

		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

		bool OnWindowResized(WindowResizeEvent& e);

		void NewProject();
		bool OpenProject();
		void OpenProject(const std::filesystem::path& path);
		void SaveProject();

		void NewScene();
		void SaveSceneAs();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);

	private:

		Application& app = Application::Get();
		int m_MSAASamples = 0;
		bool m_VsyncEnabled = false;
		bool m_FullscreenEnabled = false;
		uint64_t m_FrameCount = 0;
		uint64_t m_FixedUpdateCount = 0;
		uint64_t m_AssetManagerRefreshInterval = 60;

		int currentFPS = 0;
		double msPF = 0.0f;
		float fpsUpdateInterval = 1.0f;

		float m_FontSize = 20;

		// Panels

		bool m_GameWindowOpen = false;
		bool m_StatsWindowOpen = true;

		// Panels

		Ref<Texture2D> m_Textures[4];

		// Viewport

		Ref<Framebuffer> m_ViewportFrameBuffer;
		bool m_ViewportFocused = false, m_ViewportHovered = false, m_ViewportWindowOpen = true;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		ImGuiWindowFlags m_ViewportWindowFlags =
			ImGuiWindowFlags_NoScrollbar|
			ImGuiWindowFlags_NoScrollWithMouse|
			ImGuiWindowFlags_NoCollapse;

		CameraController m_EditorCamera;
		float m_CameraMoveSpeed = 32.0f;
		float m_CameraRotationSpeed = 20.0f;
		float m_CameraFOV = 0.2f;
		float m_CameraNearClip = 0.01f, m_CameraFarClip = 1000.0f;

		Entity m_HoveredEntity;

		int m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		bool m_UsingGizmo = false;
		float m_TranslateSnap = 0.5f;
		float m_ScaleSnap = 0.5f;
		float m_RotateSnap = 45.0f;

		// Viewport

		// Game Window

		ImGuiWindowFlags m_GameWindowFlags =
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoCollapse;

		glm::vec2 m_GameWindowSize = { 0.0f, 0.0f };

		// Game Window

		Ref<Scene> m_ActiveScene;
	};
}
