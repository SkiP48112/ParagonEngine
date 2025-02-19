#pragma once

#include "..\game_entities\ge_common.h"


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


	mVECTOR3 GetPosition() const;
	mVECTOR4 GetRotation() const;
	mVECTOR3 GetScale() const;

private:
	geTRANSFORM_ID id;
};