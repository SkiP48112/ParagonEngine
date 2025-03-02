#pragma once

#include "..\game_systems\gs_common.h"


DEFINE_TYPED_ID(gsSCRIPT_ID);


class gsSCRIPT_COMPONENT final
{
public:
	constexpr explicit gsSCRIPT_COMPONENT(gsSCRIPT_ID id) 
		: id{ id }
	{
	}

	constexpr gsSCRIPT_COMPONENT() 
		: id{ ID_INVALID_ID }
	{
	}

	constexpr gsSCRIPT_ID GetID() const
	{
		return id;
	}

	constexpr bool IsValid() const
	{
		return idIsValid(id);
	}

private:
	gsSCRIPT_ID id;
};