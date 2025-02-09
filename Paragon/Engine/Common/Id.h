#pragma once
#include "common_headers.h"

namespace paragon::id
{
	using IDType = U32;

	namespace internal
	{
		constexpr U32 GENERATION_BITS{ 8 };
		constexpr U32 INDEX_BITS{ sizeof(IDType) * 8 - GENERATION_BITS };
		
		constexpr IDType INDEX_MASK{ (IDType{1} << INDEX_BITS) - 1 };
		constexpr IDType GENERATION_MASK{ (IDType{1} << GENERATION_BITS) - 1 };
	}

	constexpr IDType INVALID_ID{ IDType(-1) };
	constexpr U32 MIN_DELETED_ELEMENTS{ 1024 };

	using GenerationType = std::conditional_t<internal::GENERATION_BITS <= 16, std::conditional_t<internal::GENERATION_BITS <= 8, U8, U16>, U32>;
	
	static_assert(sizeof(GenerationType) * 8 >= internal::GENERATION_BITS);
	static_assert((sizeof(IDType) - sizeof(GenerationType)) > 0);


	constexpr bool IsValid(IDType id)
	{
		return id != INVALID_ID;
	}


	constexpr IDType Index(IDType id)
	{
		IDType index{ id & internal::INDEX_MASK };
		assert(index != internal::INDEX_MASK);

		return index;
	}


	constexpr IDType Generation(IDType id)
	{
		return (id >> internal::INDEX_BITS) & internal::GENERATION_MASK;
	}


	constexpr IDType NewGeneration(IDType id)
	{
		const IDType generation{ id::Generation(id) + 1 };
		assert(generation < (((U64)1 << internal::GENERATION_BITS) - 1));
		return Index(id) | (generation << internal::INDEX_BITS);
	}


#if _DEBUG
	namespace internal
	{
		struct IDBase
		{
			constexpr explicit IDBase(IDType id) : _id{ id } {}
			constexpr operator IDType() const { return _id; }

		private:
			IDType _id;
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