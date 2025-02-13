#pragma once
#include "common_headers.h"

using idID_TYPE = U32;

namespace
{
	constexpr idID_TYPE ID_GENERATION_BITS{ 10 };
	constexpr idID_TYPE ID_INDEX_BITS{ sizeof(idID_TYPE) * 8 - ID_GENERATION_BITS };
	
	constexpr idID_TYPE ID_INDEX_MASK{ (idID_TYPE{1} << ID_INDEX_BITS) - 1 };
	constexpr idID_TYPE ID_GENERATION_MASK{ (idID_TYPE{1} << ID_GENERATION_BITS) - 1 };
}

constexpr idID_TYPE idINVALID_ID{ idID_TYPE(-1) };
constexpr U32 idMIN_DELETED_ELEMENTS{ 1024 };

using idGENERATION_TYPE = std::conditional_t<ID_GENERATION_BITS <= 16, std::conditional_t<ID_GENERATION_BITS <= 8, U8, U16>, U32>;

static_assert(sizeof(idGENERATION_TYPE) * 8 >= ID_GENERATION_BITS);
static_assert((sizeof(idID_TYPE) - sizeof(idGENERATION_TYPE)) > 0);


constexpr bool idIsValid(idID_TYPE id)
{
	return id != idINVALID_ID;
}


constexpr idID_TYPE idGetIndex(idID_TYPE id)
{
	idID_TYPE index{ id & ID_INDEX_MASK };
	assert(index != ID_INDEX_MASK);

	return index;
}


constexpr idID_TYPE idGetGeneration(idID_TYPE id)
{
	return (id >> ID_INDEX_BITS) & ID_GENERATION_MASK;
}


constexpr idID_TYPE idNewGeneration(idID_TYPE id)
{
	const idID_TYPE generation{ idGetGeneration(id) + 1 };
	assert(generation < (((U64)1 << ID_GENERATION_BITS) - 1));
	return idGetIndex(id) | (generation << ID_INDEX_BITS);
}


#if _DEBUG
	namespace
	{
		struct idID_BASE
		{
			constexpr explicit idID_BASE(idID_TYPE id) : id{ id } {}
			constexpr operator idID_TYPE() const { return id; }

		private:
			idID_TYPE id;
		};
	}

#define DEFINE_TYPED_ID(name)							\
	struct name final : idID_BASE						\
	{													\
		constexpr explicit name(idID_TYPE id)			\
			: idID_BASE { id } {}						\
		constexpr name() : idID_BASE { 0 } {}			\
	};
#else
#define DEFINE_TYPED_ID(name) using name = idID_TYPE;
#endif