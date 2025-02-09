#pragma once
#include "components_common.h"

namespace paragon::transform
{
	struct InitInfo
	{
		F32 position[3]{};
		F32 rotation[4]{};
		F32 scale[3]{1.0f, 1.0f, 1.0f};
	};


	Component CreateTransform(const InitInfo& info, game_entity::Entity entity);
	void RemoveTrasnform(Component component);
}