#include "gs_entity.h"
#include "gs_transform.h"
#include "gs_script.h"


namespace
{
	dsVECTOR<gsTRANSFORM_COMPONENT> transforms;
	dsVECTOR<gsSCRIPT_COMPONENT> scripts;

	dsVECTOR<idGENERATION_TYPE> generations;
	dsDEQUE<gsENTITY_ID> freeIds;
}


gsENTITY gsCreateGameEntity(gsENTITY_INFO info)
{
	// Every game entity must have a transform component
	assert(info.transform); 
	if (!info.transform)
	{
		return gsENTITY();
	}

	gsENTITY_ID id;
	if (freeIds.size() > ID_MIN_DELETED_ELEMENTS)
	{
		id = freeIds.front();
		assert(!gsIsAlive(id));

		freeIds.pop_front();
		id = gsENTITY_ID(idNewGeneration(id));
		++generations[idGetIndex(id)];
	}
	else
	{
		id = gsENTITY_ID((idID_TYPE)generations.size());
		generations.push_back(0);

		// Resize components
		// NOTE: Don't want to use resize(), so the number of memory allocations stays low
		transforms.emplace_back();
		scripts.emplace_back();
	}

	const gsENTITY newEntity(id);
	const idID_TYPE index = idGetIndex(id);

	// Create transform component
	assert(!transforms[index].IsValid());
	transforms[index] = gsCreateTransform(*info.transform, newEntity);
	if (!transforms[index].IsValid())
	{
		return gsENTITY();
	}

	// Create script component
	if (info.script && info.script->scriptCreator)
	{
		assert(!scripts[index].IsValid());
		scripts[index] = gsCreateScript(*info.script, newEntity);
		assert(scripts[index].IsValid());
	}

	return newEntity;
}


void gsRemoveGameEntity(gsENTITY_ID id)
{
	const idID_TYPE index = idGetIndex(id);
	assert(gsIsAlive(id));

	if (scripts[index].IsValid())
	{
		gsRemoveScript(scripts[index]);
		scripts[index] = {};
	}

	gsRemoveTrasnform(transforms[index]);
	transforms[index] = {};

	freeIds.push_back(id);
}


bool gsIsAlive(gsENTITY_ID id)
{
	assert(idIsValid(id));
	const idID_TYPE index = idGetIndex(id);

	assert(index < generations.size());
	assert(generations[index] == idGetGeneration(id));

	return (generations[index] == idGetGeneration(id) && transforms[index].IsValid());
}


gsTRANSFORM_COMPONENT gsENTITY::GetTransform() const
{
	assert(gsIsAlive(id));

	const idID_TYPE index = idGetIndex(id);
	return transforms[index];
}


gsSCRIPT_COMPONENT gsENTITY::GetScript() const
{
	assert(gsIsAlive(id));

	const idID_TYPE index = idGetIndex(id);
	return scripts[index];
}