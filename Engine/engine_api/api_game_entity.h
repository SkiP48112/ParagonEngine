#pragma once

#include "api_transform_component.h"
#include "..\game_entities\ge_common.h"
#include "api_script_component.h"


DEFINE_TYPED_ID(geENTITY_ID);


class geENTITY
{
public:
	constexpr explicit geENTITY(geENTITY_ID id) 
		: id { id } 
	{ 
	}

	constexpr geENTITY() 
		: id { ID_INVALID_ID }
	{ 
	}
	
	constexpr geENTITY_ID GetID() const 
	{ 
		return id; 
	}

	constexpr bool IsValid() const 
	{ 
		return idIsValid(id);  
	}

	geTRANSFORM_COMPONENT GetTransform() const;
	geSCRIPT_COMPONENT GetScript() const;

private:
	geENTITY_ID id;
};


class geENTITY_SCRIPT : public geENTITY
{
public:
	~geENTITY_SCRIPT() = default;

	virtual void BeginPlay()
	{
	}

	virtual void Update(float) 
	{
	}

protected:
	constexpr explicit geENTITY_SCRIPT(geENTITY entity)
		:geENTITY{ entity.GetID() }
	{
	}
};


using geSCRIPT_PTR = std::unique_ptr<geENTITY_SCRIPT>;
using geSCRIPT_CREATOR = geSCRIPT_PTR(*)(geENTITY entity);
using dsSTRING_HASH = std::hash<std::string>;


U8 apiRegisterScript(size_t, geSCRIPT_CREATOR);


template<class geSCRIPT_CLASS>
geSCRIPT_PTR apiCreateScript(geENTITY entity)
{
	assert(entity.IsValid());
	return std::make_unique<geSCRIPT_CLASS>(entity);

}


#define REGISTER_SCRIPT(TYPE)										\
		class TYPE;													\
		namespace {													\
		const U8 _reg##TYPE											\
		{ apiRegisterScript( dsSTRING_HASH()(#TYPE),				\
			&apiCreateScript<TYPE>) };								\
		}