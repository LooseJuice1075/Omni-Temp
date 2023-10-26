#include "ompch.h"

#include "Entity.h"

namespace Omni
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
}
