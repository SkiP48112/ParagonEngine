#pragma once

#include "..\components\components_common.h"

DEFINE_TYPED_ID(geTRANSFORM_ID);


class geTRANSFORM_COMPONENT final
{
public:
	constexpr explicit geTRANSFORM_COMPONENT(geTRANSFORM_ID id) : id{ id }
	{
	}

	constexpr geTRANSFORM_COMPONENT() : id{ ID_INVALID_ID }
	{
	}

	constexpr geTRANSFORM_ID GetID() const
	{
		return id;
	}

	constexpr bool IsValid() const
	{
		return idIsValid(id);
	}


	mathVECTOR_3 GetPosition() const;
	mathVECTOR_4 GetRotation() const;
	mathVECTOR_3 GetScale() const;

private:
	geTRANSFORM_ID id;
};