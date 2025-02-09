#pragma once

#include "..\components\components_common.h"

namespace paragon::transform
{
	DEFINE_TYPED_ID(TransformID);


	class Component final
	{
	public:
		constexpr explicit Component(TransformID id) : _id{ id }
		{
		}

		constexpr Component() : _id{ id::INVALID_ID }
		{
		}

		constexpr TransformID GetID() const
		{
			return _id;
		}

		constexpr bool IsValid() const
		{
			return id::IsValid(_id);
		}


		math::Vector3 GetPosition() const;
		math::Vector4 GetRotation() const;
		math::Vector3 GetScale() const;

	private:
		TransformID _id;
	};
}