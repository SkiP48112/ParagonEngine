#pragma once

#include "transform_component.h"
#include "..\components\components_common.h"


DEFINE_TYPED_ID(geENTITY_ID);


class geENTITY
{
public:
	constexpr explicit geENTITY(geENTITY_ID id) : id { id } 
	{ 
	}

	constexpr geENTITY() : id { ID_INVALID_ID }
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

	geTRANSFORM_COMPONENT Transform() const;

private:
	geENTITY_ID id;
};