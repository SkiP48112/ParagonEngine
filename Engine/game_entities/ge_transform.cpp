#include "ge_transform.h"
#include "ge_entity.h"


namespace
{
	dsVECTOR<mVECTOR3> positions;
	dsVECTOR<mVECTOR4> rotations;
	dsVECTOR<mVECTOR3> scales;
}


geTRANSFORM_COMPONENT geCreateTransform(const geTRANSFORM_INIT_INFO& info, geENTITY entity)
{
	assert(entity.IsValid());
	const idID_TYPE entityIndex{ idGetIndex(entity.GetID()) };

	if (positions.size() > entityIndex)
	{
		positions[entityIndex] = mVECTOR3(info.position);
		rotations[entityIndex] = mVECTOR4(info.rotation);
		scales[entityIndex] = mVECTOR3(info.scale);
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


mVECTOR3 geTRANSFORM_COMPONENT::GetPosition() const
{
	assert(IsValid());
	return positions[idGetIndex(id)];
}


mVECTOR4 geTRANSFORM_COMPONENT::GetRotation() const
{
	assert(IsValid());
	return rotations[idGetIndex(id)];
}


mVECTOR3 geTRANSFORM_COMPONENT::GetScale() const
{
	assert(IsValid());
	return scales[idGetIndex(id)];
}