#pragma once

#include "ompch.h"

#include "core/Core.h"
#include "events/Event.h"

namespace Omni
{
	enum CursorMode
	{
		Normal,
		Hidden,
		LockedAndHidden,
	};

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Omni Engine",
			uint32_t width = 1280,
			uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		// Window attributes
		virtual void SetSize(uint32_t width, uint32_t height) = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual void SetMSAA(int samples) = 0;
		virtual int GetMSAA() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetFullscreen(bool fullscreen) = 0;
		virtual bool IsFullscreen() const = 0;

		virtual void SetCursorMode(CursorMode cursorMode) = 0;
		virtual CursorMode GetCursorMode() const = 0;

		virtual void SetCursorPosition(int x, int y) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
