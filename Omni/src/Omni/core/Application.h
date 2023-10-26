#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "events/Event.h"
#include "Omni/events/ApplicationEvent.h"
#include "core/Timestep.h"

#include "ImGui/ImGuiLayer.h"

namespace Omni
{
	class Application
	{
	public:
		Application(const std::string& name = "Omni App");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }

		bool m_CapUpdateInterval = false;
		/// <summary>
		/// The target application framerate.
		/// </summary>
		double m_UpdateInterval = 60.0f;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		/// <summary>
		/// The number of milliseconds between each FixedUpdate step.
		/// </summary>
		double m_FixedUpdateInterval = 0.01666666667f;
	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}
