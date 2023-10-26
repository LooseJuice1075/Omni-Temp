#pragma once

#include "core/Core.h"
#include "core/Timestep.h"
#include "events/Event.h"

namespace Omni
{
	class OMNI_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		/// <summary>
		/// Consistently updates 60 times every second.
		/// </summary>
		virtual void OnFixedUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}
