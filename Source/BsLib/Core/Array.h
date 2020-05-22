#pragma once
#include "Types.h"
#include "Assertion.h"
#include <vector>

namespace Bs
{
	template<typename T>
	class Array : std::vector<T>
	{

		using Base = std::vector<T>;

	public:

		Array() = default;
		Array(u32 reservedCapacity)
			: Base()
		{
			this->reserve(reservedCapacity);
		}

		using ForwardIterator = T*;
		using ConstForwardIterator = const T*;
		using Reference = T&;
		using ConstReference = const T&;

		void Add(const T& element)
		{
			this->push_back(element);
		}

		void Add(T&& element)
		{
			this->push_back(std::move(element));
		}

		ForwardIterator Begin() { return this->begin(); }

		ForwardIterator End() { return Begin() + Size(); }

		const ConstForwardIterator Begin() const { return this->begin(); }

		const ConstForwardIterator End() const { return Begin() + Size(); }

		constexpr u32 Size() const { return this->size(); }

		Reference operator[](u32 index)
		{
			BS_ASSERT(index < Size);
			return Begin()[index];
		}

		ConstReference operator[](u32 index) const
		{
			BS_ASSERT(index < Size);
			return Begin()[index];
		}

		T* Data() { return this->data(); }

		const T* Data() const { return this->data(); }


	};
}
