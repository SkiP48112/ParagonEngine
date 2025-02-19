#pragma once

#include "..\game_entities\ge_common.h"


DEFINE_TYPED_ID(geSCRIPT_ID);


class geSCRIPT_COMPONENT final
{
public:
	constexpr explicit geSCRIPT_COMPONENT(geSCRIPT_ID id) : id{ id }
	{
	}

	constexpr geSCRIPT_COMPONENT() : id{ ID_INVALID_ID }
	{
	}

	constexpr geSCRIPT_ID GetID() const
	{
		return id;
	}

	constexpr bool IsValid() const
	{
		return idIsValid(id);
	}

private:
	geSCRIPT_ID id;
};