#include "ompch.h"

#include "Application.h"
#include "core/Log.h"
#include "asset/AssetManager.h"
#include "renderer/Renderer.h"
#include <GLFW/glfw3.h>

#include "events/Input.h"

namespace Omni
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		OM_PROFILE_FUNCTION();

		OM_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		AssetManager::Init();
		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		OM_PROFILE_FUNCTION();

		Renderer::Shutdown();
		AssetManager::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		OM_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		OM_PROFILE_FUNCTION();
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		OM_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopOverlay(Layer* overlay)
	{
		OM_PROFILE_FUNCTION();
		m_LayerStack.PopOverlay(overlay);
		overlay->OnDetach();
	}

	void Application::OnEvent(Event& e)
	{
		OM_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		OM_PROFILE_FUNCTION();

		double currentTime = glfwGetTime();
		double accumulator = 0.0f;

		while (m_Running)
		{
			OM_PROFILE_SCOPE("RunLoop");

			double time = (float)glfwGetTime();

			// Calculate fixed update timestep

			double frameTime = time - currentTime;
			if (frameTime > 0.25f)
				frameTime = 0.25f;
			currentTime = time;

			accumulator += frameTime;

			while (accumulator >= m_FixedUpdateInterval)
			{
				OM_PROFILE_SCOPE("LayerStack FixedUpdates");
				for (Layer* layer : m_LayerStack)
					layer->OnFixedUpdate();
				accumulator -= m_FixedUpdateInterval;
			}

			// TODO : Decouple from glfw

			// Calculate timestep

			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (m_CapUpdateInterval)
			{
				// 10 is added to compensate for the apparent lag.
				while (glfwGetTime() < m_LastFrameTime + 1.0f / (m_UpdateInterval + 10.0f))
				{
					std::this_thread::yield();
				}
			}

			if (!m_Minimized)
			{
				{
					OM_PROFILE_SCOPE("LayerStack Updates");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					OM_PROFILE_SCOPE("LayerStack OnImGuiRender");
					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}
