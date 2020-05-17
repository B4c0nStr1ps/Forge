#pragma once
#include "Types.h"
#include "Assertion.h"

namespace Bs
{
	template<typename T, u32 SizeType>
	class FixedArray
	{
	public:

		using ForwardIterator = T*;
		using ConstForwardIterator = const T*;
		using Reference = T&;
		using ConstReference = const T&;

		FixedArray() = default;
		FixedArray(const FixedArray& other)
		{
			BS_ASSERT(Size() == other.Size());
			std::copy(other.Begin(), other.End(), Begin());
		}
		FixedArray(FixedArray&& other)
		{
			BS_ASSERT(Size() == other.Size());
			std::uninitialized_move(other.Begin(), other.End(), Begin());			
		}
		FixedArray(std::initializer_list<T> initializedList)
		{
			BS_ASSERT(Size() == initializedList.size());
			std::uninitialized_move(initializedList.begin(), initializedList.end(), Begin());
		}

		FixedArray& operator=(std::initializer_list<T> initializedList) {
			BS_ASSERT(Size() == initializedList.size());
			std::uninitialized_move(initializedList.begin(), initializedList.end(), Begin());
			return *this;
		}

		ForwardIterator Begin(){ return m_data; }

		ForwardIterator End() {return Begin() + Size; }

		const ConstForwardIterator Begin() const { return m_data; }

		const ConstForwardIterator End() const {return Begin() + Size; }

		constexpr u32 Size() const { return SizeType; }

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

		T* Data() { return m_data; }

		const T* Data() const { return m_data; }

	private:
		T m_data[SizeType];
	};
}
