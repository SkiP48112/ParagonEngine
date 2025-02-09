#pragma once
#include "components_common.h"

namespace paragon
{
#define INIT_INFO(component) namespace component {struct InitInfo;}
	INIT_INFO(transform);
#undef INIT_INFO

	namespace game_entity
	{
		struct EntityInfo
		{
			transform::InitInfo* transform{ nullptr };
		};


		Entity CreateGameEntity(const EntityInfo info);
		void RemoveGameEntity(Entity entity);
		bool IsAlive(Entity entity);
	}
}