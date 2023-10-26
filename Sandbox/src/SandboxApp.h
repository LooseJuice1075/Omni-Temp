#pragma once

#include "Omni.h"
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace Omni
{
	class SandboxLayer : public Layer
	{
	public:
		SandboxLayer();
		~SandboxLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		void OnFixedUpdate() override;
		virtual void OnImGuiRender() override;

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		void OnEvent(Event& e) override;

	private:

		Application& app = Application::Get();
		int msaaSamples = 4;
		bool vsyncOn = true;
		bool fullscreen = false;

		Ref<Framebuffer> m_Framebuffer;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		ShaderLibrary m_ShaderLibrary;

		Camera m_Camera;
		std::string m_CamModeLabel;
		float m_CameraMoveSpeed = 12.0f;
		float m_CameraRotSpeed = 40.0f;
		float m_CameraZoomSpeed = 100.0f;

		int frameCounter = 0;
		int currentFPS = 0;
		double msPF = 0.0f;
		float fpsUpdateInterval = 1.0f;

		bool physicsDebug = false;

		int m_PrevViewportW;
		int m_PrevViewportH;

		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_CheckerboardTexture;

		glm::mat4 m_Transform;

		glm::vec2 m_SpritePosition = { 0.0f, 3.0f };

		glm::vec2 m_SpriteRowsColumns = { 20.0f, 20.0f };

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 2.0f };
		glm::vec3 m_CameraRotation = { 0.0f, 0.0f, 0.0f };
		float m_CameraFOV = 0.1f;
	};
}
