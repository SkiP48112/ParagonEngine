#include "ge_script.h"
#include "ge_entity.h"


namespace
{
	dsVECTOR<geSCRIPT_PTR> entityScripts;
	dsVECTOR<idID_TYPE>	idMapping;

	dsVECTOR<idGENERATION_TYPE> generations;
	dsVECTOR<geSCRIPT_ID> freeIds;

	// We need to create out own implementation of unordered_map
	using dsSCRIPT_REGISTERY = std::unordered_map<size_t, geSCRIPT_CREATOR>;
	dsSCRIPT_REGISTERY& Registery() 
	{
		// NOTE: we put static variable in a function because of
		//		the initializetion order in static data. This way, we can
		//		be certain that the data is initialized before accessing it.

		static dsSCRIPT_REGISTERY registery;
		return registery;
	}


	bool geIsScriptExists(geSCRIPT_ID id)
	{
		assert(idIsValid(id));
		const idID_TYPE index{ idGetIndex(id) };

		assert(index < generations.size() && idMapping[index] < entityScripts.size());
		assert(generations[index] == idGetGeneration(id));

		return (generations[index] == idGetGeneration(id)) &&
			entityScripts[idMapping[index]] &&
			entityScripts[idMapping[index]]->IsValid();
	}
}


U8 apiRegisterScript(size_t tag, geSCRIPT_CREATOR func)
{
	bool result{ Registery().insert(dsSCRIPT_REGISTERY::value_type(tag, func)).second };
	assert(result);

	return result;
}


geSCRIPT_COMPONENT geCreateScript(geSCRIPT_INIT_INFO info, geENTITY entity)
{
	assert(entity.IsValid());
	assert(info.scriptCreator);

	geSCRIPT_ID id{};
	if (freeIds.size() > ID_MIN_DELETED_ELEMENTS)
	{
		id = freeIds.front();
		freeIds.pop_back();

		id = geSCRIPT_ID{ idNewGeneration(id) };
		++generations[idGetIndex(id)];
	}
	else
	{
		id = geSCRIPT_ID{ (idID_TYPE)idMapping.size() };
		idMapping.emplace_back();
		generations.push_back(0);
	}

	assert(idIsValid(id));
	entityScripts.emplace_back(info.scriptCreator(entity));
	assert(entityScripts.back()->GetID() == entity.GetID());

	const idID_TYPE index{ (idID_TYPE)entityScripts.size() };
	idMapping[idGetIndex(id)] = index;

	return geSCRIPT_COMPONENT{ id };
}


void geRemoveScript(geSCRIPT_COMPONENT component)
{
	assert(component.IsValid() && geIsScriptExists(component.GetID()));
	
	const geSCRIPT_ID id{ component.GetID() };
	const idID_TYPE index{ idMapping[idGetIndex(id)] };
	const geSCRIPT_ID lastId{ entityScripts.back()->GetScript().GetID() };

	dsEraseUnordered(entityScripts, index);

	idMapping[idGetIndex(lastId)] = index;
	idMapping[idGetIndex(id)] = ID_INVALID_ID;
}