#pragma once

#include "api_transform_component.h"
#include "api_script_component.h"
#include "..\game_systems\gs_common.h"


DEFINE_TYPED_ID(gsENTITY_ID);


class gsENTITY
{
public:
	constexpr explicit gsENTITY(gsENTITY_ID id) 
		: id { id } 
	{ 
	}

	constexpr gsENTITY() 
		: id { ID_INVALID_ID }
	{ 
	}
	
	constexpr gsENTITY_ID GetID() const 
	{ 
		return id; 
	}

	constexpr bool IsValid() const 
	{ 
		return idIsValid(id);  
	}

	gsTRANSFORM_COMPONENT GetTransform() const;
	gsSCRIPT_COMPONENT GetScript() const;

private:
	gsENTITY_ID id;
};


class gsENTITY_SCRIPT : public gsENTITY
{
public:
	~gsENTITY_SCRIPT() = default;

	virtual void BeginPlay()
	{
	}

	virtual void Update(float) 
	{
	}

protected:
	constexpr explicit gsENTITY_SCRIPT(gsENTITY entity)
		:gsENTITY{ entity.GetID() }
	{
	}
};


using gsSCRIPT_PTR = std::unique_ptr<gsENTITY_SCRIPT>;
using gsSCRIPT_CREATOR = gsSCRIPT_PTR(*)(gsENTITY entity);
using dsSTRING_HASH = std::hash<std::string>;


U8 apiRegisterScript(size_t, gsSCRIPT_CREATOR);

#ifdef USE_WITH_EDITOR
extern "C" __declspec(dllexport)
#endif
gsSCRIPT_CREATOR apiGetScriptCreator(size_t tag);


template<class gsSCRIPT_CLASS>
gsSCRIPT_PTR apiCreateScript(gsENTITY entity)
{
	assert(entity.IsValid());
	return std::make_unique<gsSCRIPT_CLASS>(entity);
}

#ifdef USE_WITH_EDITOR
U8 apiAddScriptName(const char* name);

#define REGISTER_SCRIPT(TYPE)																			\
		namespace																							\
		{																										\
			const U8 _reg_##TYPE																			\
			{																									\
				apiRegisterScript( dsSTRING_HASH()(#TYPE), &apiCreateScript<TYPE>)		\
			};																									\
			const U8 _name_##TYPE																		\
			{																									\
				apiAddScriptName(#TYPE)																	\
			};																									\
		}

#else
#define REGISTER_SCRIPT(TYPE)																			\
		namespace																							\
		{																										\
			const U8 _reg_##TYPE																			\
			{																									\
				apiRegisterScript( dsSTRING_HASH()(#TYPE), &apiCreateScript<TYPE>)		\
			};																									\
		}
#endif