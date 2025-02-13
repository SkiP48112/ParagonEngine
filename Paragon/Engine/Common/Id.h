#pragma once
#include "common_headers.h"

namespace paragon::id
{
	using IDType = U32;

	namespace
	{
		constexpr IDType GENERATION_BITS{ 10 };
		constexpr IDType INDEX_BITS{ sizeof(IDType) * 8 - GENERATION_BITS };
		
		constexpr IDType INDEX_MASK{ (IDType{1} << INDEX_BITS) - 1 };
		constexpr IDType GENERATION_MASK{ (IDType{1} << GENERATION_BITS) - 1 };
	}

	constexpr IDType INVALID_ID{ IDType(-1) };
	constexpr U32 MIN_DELETED_ELEMENTS{ 1024 };

	using GenerationType = std::conditional_t<GENERATION_BITS <= 16, std::conditional_t<GENERATION_BITS <= 8, U8, U16>, U32>;
	
	static_assert(sizeof(GenerationType) * 8 >= GENERATION_BITS);
	static_assert((sizeof(IDType) - sizeof(GenerationType)) > 0);


	constexpr bool IsValid(IDType id)
	{
		return id != INVALID_ID;
	}


	constexpr IDType Index(IDType id)
	{
		IDType index{ id & INDEX_MASK };
		assert(index != INDEX_MASK);

		return index;
	}


	constexpr IDType Generation(IDType id)
	{
		return (id >> INDEX_BITS) & GENERATION_MASK;
	}


	constexpr IDType NewGeneration(IDType id)
	{
		const IDType generation{ id::Generation(id) + 1 };
		assert(generation < (((U64)1 << GENERATION_BITS) - 1));
		return Index(id) | (generation << INDEX_BITS);
	}


#if _DEBUG
	namespace internal
	{
		struct IDBase
		{
			constexpr explicit IDBase(IDType id) : id{ id } {}
			constexpr operator IDType() const { return id; }

		private:
			IDType id;
		};
	}
#define DEFINE_TYPED_ID(name)							\
	struct name final : id::internal::IDBase		\
	{												\
		constexpr explicit name(id::IDType id)		\
			: IDBase { id } {}						\
		constexpr name() : IDBase { 0 } {}			\
	};
#else
#define DEFINE_TYPED_ID(name) using name = id::IDType;
#endif
}