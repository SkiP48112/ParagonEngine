#pragma once

#include "..\game_systems\gs_common.h"


DEFINE_TYPED_ID(gsTRANSFORM_ID);


class gsTRANSFORM_COMPONENT final
{
public:
	constexpr explicit gsTRANSFORM_COMPONENT(gsTRANSFORM_ID id) 
		: id{ id }
	{
	}

	constexpr gsTRANSFORM_COMPONENT() 
		: id{ ID_INVALID_ID }
	{
	}

	constexpr gsTRANSFORM_ID GetID() const
	{
		return id;
	}

	constexpr bool IsValid() const
	{
		return idIsValid(id);
	}


	mVECTOR3 GetPosition() const;
	mVECTOR4 GetRotation() const;
	mVECTOR3 GetScale() const;

private:
	gsTRANSFORM_ID id;
};