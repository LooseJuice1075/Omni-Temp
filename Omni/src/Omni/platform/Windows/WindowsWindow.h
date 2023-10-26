#pragma once

#include "core/Window.h"
#include "renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Omni
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		// Window attributes
		inline void SetSize(uint32_t width, uint32_t height) override;
		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

		void SetMSAA(int samples) override;
		int GetMSAA() const override;

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		void SetFullscreen(bool fullscreen) override;
		bool IsFullscreen() const override;

		void SetCursorMode(CursorMode cursorMode) override;
		CursorMode GetCursorMode() const override;

		void SetCursorPosition(int x, int y) override;

		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			uint32_t WindowedWidth, WindowedHeight;
			int XPos, YPos;
			int WindowedXPos, WindowedYPos;
			int MSAASamples;
			bool VSync;
			bool Fullscreen;
			CursorMode CursorMode;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
