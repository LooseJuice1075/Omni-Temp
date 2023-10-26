#pragma once

#include "Entity.h"
#include "core/Timestep.h"

namespace Omni
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnFixedUpdate() {}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}
