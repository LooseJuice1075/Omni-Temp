#pragma once

#include "Omni/core/Layer.h"

#include "Omni/events/Event.h"
#include "Omni/events/KeyEvent.h"
#include "Omni/events/MouseEvent.h"
#include "Omni/events/ApplicationEvent.h"

#include <imgui.h>

namespace Omni
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void EnableMouse(bool enable);

		uint32_t GetActiveWidgetID() const;
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;

		ImGuiConfigFlags m_ConfigFlags =
			ImGuiConfigFlags_NavEnableKeyboard | // Enable Keyboard Controls
			ImGuiConfigFlags_DockingEnable |	 // Enable Docking
			ImGuiConfigFlags_ViewportsEnable;	 // Enable Multi-Viewport / Platform Windows
	};
}
