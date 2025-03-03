#include "gs_script.h"


namespace
{
	dsVECTOR<gsSCRIPT_PTR> scripts;
	dsVECTOR<idID_TYPE> idMapping;

	dsVECTOR<idGENERATION_TYPE> generations;
	dsDEQUE<gsSCRIPT_ID> freeIds;

	// We need to create out own implementation of unordered_map
	using dsSCRIPT_REGISTRY = std::unordered_map<size_t, gsSCRIPT_CREATOR>;
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


	bool gsIsScriptExists(gsSCRIPT_ID id)
	{
		assert(idIsValid(id));
		const idID_TYPE index = idGetIndex(id);

		assert(index < generations.size() && idMapping[index] < scripts.size());
		assert(generations[index] == idGetGeneration(id));

		return (generations[index] == idGetGeneration(id)) &&
			scripts[idMapping[index]] &&
			scripts[idMapping[index]]->IsValid();
	}
}


U8 apiRegisterScript(size_t tag, gsSCRIPT_CREATOR func)
{
	bool result = Registry().insert(dsSCRIPT_REGISTRY::value_type(tag, func)).second;
	assert(result);

	return result;
}


gsSCRIPT_CREATOR apiGetScriptCreator(size_t tag)
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


gsSCRIPT_COMPONENT gsCreateScript(gsSCRIPT_INIT_INFO info, gsENTITY entity)
{
	assert(entity.IsValid());
	assert(info.scriptCreator);

	gsSCRIPT_ID id;
	if (freeIds.size() > ID_MIN_DELETED_ELEMENTS)
	{
		id = freeIds.front();
		assert(!gsIsScriptExists(id));

		freeIds.pop_front();

		id = gsSCRIPT_ID(idNewGeneration(id));
		++generations[idGetIndex(id)];
	}
	else
	{
		id = gsSCRIPT_ID((idID_TYPE)idMapping.size());
		idMapping.emplace_back();
		generations.push_back(0);
	}

	assert(idIsValid(id));
	const idID_TYPE index = (idID_TYPE)scripts.size();
	scripts.emplace_back(info.scriptCreator(entity));
	assert(scripts.back()->GetID() == entity.GetID());
	
	idMapping[idGetIndex(id)] = index;

	return gsSCRIPT_COMPONENT{ id };
}


void gsRemoveScript(gsSCRIPT_COMPONENT component)
{
	assert(component.IsValid() && gsIsScriptExists(component.GetID()));
	
	const gsSCRIPT_ID id = component.GetID();
	const idID_TYPE index = idMapping[idGetIndex(id)];
	const gsSCRIPT_ID lastId = scripts.back()->GetScript().GetID();

	dsEraseUnordered(scripts, index);

	idMapping[idGetIndex(lastId)] = index;
	idMapping[idGetIndex(id)] = ID_INVALID_ID;
}

void gsUpdateScripts(float dt)
{
	for (auto& ptr : scripts)
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

		for (U32 i = 0; i < size; i++)
		{
			names.SetAt(i, A2BSTR_EX(ScriptNames()[i].c_str()), false);
		}

		return names.Detach();
	}

#endif