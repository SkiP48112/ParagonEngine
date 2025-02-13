#pragma once

#include "..\components\components_common.h"
#include "transform_component.h"

namespace paragon::game_entity
{
	DEFINE_TYPED_ID(EntityID);


	class Entity
	{
	public:
		constexpr explicit Entity(EntityID id) : id { id } 
		{ }

		constexpr Entity() : id { id::INVALID_ID }
		{ }
		
		constexpr EntityID GetID() const 
		{ 
			return id; 
		}

		constexpr bool IsValid() const 
		{ 
			return id::IsValid(id);  
		}

		transform::Component Transform() const;

	private:
		EntityID id;
	};
}