#include "transform.h"
#include "entity.h"

namespace paragon::transform
{
	namespace
	{
		ds::Vector<math::Vector3> positions;
		ds::Vector<math::Vector4> rotations;
		ds::Vector<math::Vector3> scales;
	}


	Component CreateTransform(const InitInfo& info, game_entity::Entity entity)
	{
		assert(entity.IsValid());
		const id::IDType entityIndex{ id::Index(entity.GetID()) };

		if (positions.size() > entityIndex)
		{
			positions[entityIndex] = math::Vector3(info.position);
			rotations[entityIndex] = math::Vector4(info.rotation);
			scales[entityIndex] = math::Vector3(info.scale);
		}
		else
		{
			assert(positions.size() == entityIndex);
			positions.emplace_back(info.position);
			rotations.emplace_back(info.rotation);
			scales.emplace_back(info.scale);
		}

		return Component{ TransformID{ entity.GetID() } };
	}


	void RemoveTrasnform(Component component)
	{
		assert(component.IsValid());
	}


	math::Vector3 Component::GetPosition() const
	{
		assert(IsValid());
		return positions[id::Index(id)];
	}


	math::Vector4 Component::GetRotation() const
	{
		assert(IsValid());
		return rotations[id::Index(id)];
	}


	math::Vector3 Component::GetScale() const
	{
		assert(IsValid());
		return scales[id::Index(id)];
	}
}