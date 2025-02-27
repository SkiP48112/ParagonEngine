#include "ge_script.h"
#include "ge_entity.h"


namespace
{
	dsVECTOR<geSCRIPT_PTR> entityScripts;
	dsVECTOR<idID_TYPE>	idMapping;

	dsVECTOR<idGENERATION_TYPE> generations;
	dsVECTOR<geSCRIPT_ID> freeIds;

	// We need to create out own implementation of unordered_map
	using dsSCRIPT_REGISTRY = std::unordered_map<size_t, geSCRIPT_CREATOR>;
	dsSCRIPT_REGISTRY& Registry() 
	{
		// NOTE: we put static variable in a function because of
		//		the initializetion order in static data. This way, we can
		//		be certain that the data is initialized before accessing it.

		static dsSCRIPT_REGISTRY registry;
		return registry;
	}


	#ifdef USE_WITH_EDITOR
		dsVECTOR<std::string>& ScriptNames()
		{
			// NOTE: we put static variable in a function because of
			//		the initializetion order in static data. This way, we can
			//		be certain that the data is initialized before accessing it.

			static dsVECTOR<std::string> names;
			return names;
		}
	#endif


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
	bool result{ Registry().insert(dsSCRIPT_REGISTRY::value_type(tag, func)).second };
	assert(result);

	return result;
}


geSCRIPT_CREATOR apiGetScriptCreator(size_t tag)
{
	auto script = Registry().find(tag);
	assert(script != Registry().end() && script->first == tag);

	return script->second;
}


#ifdef USE_WITH_EDITOR
	U8 apiAddScriptName(const char* name)
	{
		ScriptNames().emplace_back(name);
		return true;
	}
#endif


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
	const idID_TYPE index{ (idID_TYPE)entityScripts.size() };
	entityScripts.emplace_back(info.scriptCreator(entity));
	assert(entityScripts.back()->GetID() == entity.GetID());
	
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

void geUpdateScripts(float dt)
{
	for (auto& ptr : entityScripts)
	{
		ptr->Update(dt);
	}
}


#ifdef USE_WITH_EDITOR
	#include <atlsafe.h>

	extern "C" __declspec(dllexport)
	LPSAFEARRAY apiGetScriptNames()
	{
		const U32 size{ (U32)ScriptNames().size() };
		CComSafeArray<BSTR> names(size);

		for (int i = 0; i < size; i++)
		{
			names.SetAt(i, A2BSTR_EX(ScriptNames()[i].c_str()), false);
		}

		return names.Detach();
	}

#endif