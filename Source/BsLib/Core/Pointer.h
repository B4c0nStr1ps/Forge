#pragma once
#include <memory>

namespace Bs
{
	template<typename T, typename Deleter = std::default_delete<T>>
	using UniquePtr = std::unique_ptr<T, Deleter>;
}
