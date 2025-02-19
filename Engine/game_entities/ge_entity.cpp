#include "ge_entity.h"
#include "ge_transform.h"


namespace
{
	dsVECTOR<geTRANSFORM_COMPONENT> transforms;
	dsVECTOR<idGENERATION_TYPE> generations;
	dsDEQUE<geENTITY_ID> freeIds;
}


geENTITY geCreateGameEntity(const geENTITY_INFO info)
{
	// Every game entity must have a transform component
	assert(info.transform); 
	if (!info.transform)
	{
		return geENTITY{};
	}

	geENTITY_ID id;
	if (freeIds.size() > ID_MIN_DELETED_ELEMENTS)
	{
		id = freeIds.front();
		assert(!geIsAlive(geENTITY{ id }));

		freeIds.pop_front();
		id = geENTITY_ID{ idNewGeneration(id) };
		++generations[idGetIndex(id)];
	}
	else
	{
		id = geENTITY_ID{(idID_TYPE)generations.size()};
		generations.push_back(0);

		// Resize components
		// NOTE: Don't want to use resize(), so the number of memory allocations stays low
		transforms.emplace_back();
	}

	const geENTITY newEntity{ id };
	const idID_TYPE index{ idGetIndex(id) };

	// Create transform component
	assert(!transforms[index].IsValid());
	transforms[index] = geCreateTransform(*info.transform, newEntity);
	if (!transforms[index].IsValid())
	{
		return {};
	}

	return newEntity;
}


void geRemoveGameEntity(geENTITY entity)
{
	const geENTITY_ID id{ entity.GetID() };
	const idID_TYPE index{ idGetIndex(id) };

	assert(geIsAlive(entity));
	if (!geIsAlive(entity))
	{
		return;
	}

	geRemoveTrasnform(transforms[index]);
	transforms[index] = {};

	freeIds.push_back(id);
}


bool geIsAlive(geENTITY entity)
{
	assert(entity.IsValid());
	
	const geENTITY_ID id{ entity.GetID() };
	const idID_TYPE index{ idGetIndex(id) };

	assert(index < generations.size());
	assert(generations[index] == idGetGeneration(id));

	return (generations[index] == idGetGeneration(id) && transforms[index].IsValid());
}


geTRANSFORM_COMPONENT geENTITY::Transform() const
{
	assert(geIsAlive(*this));

	const idID_TYPE index{  idGetIndex(id) };
	return transforms[index];
}