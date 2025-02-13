#include "transform.h"
#include "entity.h"

namespace
{
	dsVECTOR<mathVECTOR_3> positions;
	dsVECTOR<mathVECTOR_4> rotations;
	dsVECTOR<mathVECTOR_3> scales;
}


geTRANSFORM_COMPONENT geCreateTransform(const geTRANSFORM_INIT_INFO& info, geENTITY entity)
{
	assert(entity.IsValid());
	const idID_TYPE entityIndex{ idGetIndex(entity.GetID()) };

	if (positions.size() > entityIndex)
	{
		positions[entityIndex] = mathVECTOR_3(info.position);
		rotations[entityIndex] = mathVECTOR_4(info.rotation);
		scales[entityIndex] = mathVECTOR_3(info.scale);
	}
	else
	{
		assert(positions.size() == entityIndex);
		positions.emplace_back(info.position);
		rotations.emplace_back(info.rotation);
		scales.emplace_back(info.scale);
	}

	return geTRANSFORM_COMPONENT{ geTRANSFORM_ID{ entity.GetID() } };
}


void geRemoveTrasnform(geTRANSFORM_COMPONENT component)
{
	assert(component.IsValid());
}


mathVECTOR_3 geTRANSFORM_COMPONENT::GetPosition() const
{
	assert(IsValid());
	return positions[idGetIndex(id)];
}


mathVECTOR_4 geTRANSFORM_COMPONENT::GetRotation() const
{
	assert(IsValid());
	return rotations[idGetIndex(id)];
}


mathVECTOR_3 geTRANSFORM_COMPONENT::GetScale() const
{
	assert(IsValid());
	return scales[idGetIndex(id)];
}