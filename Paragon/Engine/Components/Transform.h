#pragma once
#include "ComponentsCommon.h"

namespace paragon::transform
{
	DEFINE_TYPED_ID(transform_id);
	

	struct init_info
	{
		F32 position[3]{};
		F32 rotation[4]{};
		F32 scale[3]{1.0f, 1.0f, 1.0f};
	};


	transform_id create_transform(const init_info& info, game_entity::entity_id entity_id);
	void remove_transform(transform_id id);
}