#pragma once
#include <vector>

namespace Bs
{
	template<typename T>
	class Array : std::vector<T>
	{
	public:
		void Add(const T& element)
		{
			this->push_back(element);
		}

		void Add(T&& element)
		{
			this->push_back(std::move(element));
		}
	};
}
