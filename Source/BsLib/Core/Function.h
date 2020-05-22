#pragma once

#include "TypeTraits.h"
#include "Pointer.h"

namespace Bs
{
	template<class>
	class Function;

	template<typename R, typename... Args>
	class Function<R(Args...)>
	{
		struct Concept;

		template<typename F, bool Small>
		struct Model;

		static constexpr size_t k_smallSize = sizeof(void*) * 4;

		static constexpr Concept s_empty{ [](void*) noexcept {} };

		const Concept* m_concept = &s_empty;

		std::aligned_storage_t<k_smallSize> m_model;

	public:

		template<typename F>
		Function(F&& f)
		{
			constexpr bool isSmall = sizeof(Model<std::decay_t<F>, true>) <= k_smallSize;
			new (&m_model) Model<std::decay_t<F>, isSmall>(std::forward<F>(f));
			m_concept = &Model<std::decay_t<F>, isSmall>::s_vtable;
		}

		~Function()
		{
			m_concept->_dtor(&m_model);
		}

		Function(Function&& other) noexcept
			: m_concept(other.m_concept)
		{
			m_concept->_move(&other.m_model, &m_model);
		}

		Function& operator=(Function&& other) noexcept
		{
			if (this == &other) return *this;
			m_concept->_dtor(&m_model);
			m_concept = other.m_concept;
			m_concept->_move(&other.m_model, &m_model);
			return *this;
		}

		R operator()(Args... args)
		{
			return m_concept->_invoke(&m_model, std::forward<Args>(args)...);
		}
	};

	template<typename R, typename... Args>
	struct Function<R(Args...)>::Concept {
		void (*_dtor)(void*) noexcept;
		void (*_move)(void*, void*) noexcept;
		R(*_invoke)(void*, Args&&...);
	};

	template<typename R, typename... Args>
	template<typename F>
	struct Function<R(Args...)>::Model<F, true>
	{
		template<typename G>
		Model(G&& f) : m_callable(f) {}

		static void _dtor(void* self) noexcept { static_cast<Model*>(self)->~Model(); }
		static void _move(void* self, void* other) noexcept
		{
			new (other) Model(std::move(*static_cast<Model*>(self)));
		}
		static void _invoke(void* self, Args&&... args)
		{
			return std::invoke(static_cast<Model*>(self)->m_callable, std::forward<Args>(args)...);
		}

		static constexpr Concept s_vtable = { _dtor, _move, _invoke };

		F m_callable;
	};

	template <typename R, typename... Args>
	template <typename F>
	struct Function<R(Args...)>::Model<F, false>
	{
		template<typename G>
		Model(G&& f)
			: m_callable(std::make_unique<F>(std::forward<F>(f)))
		{}

		static void _dtor(void* self) noexcept { static_cast<Model*>(self)->~Model(); }
		static void _move(void* self, void* other) noexcept {
			new (other) Model(std::move(*static_cast<Model*>(self)));
		}
		static R _invoke(void* self, Args&&... args) {
			return std::invoke(*static_cast<Model*>(self)->m_callable, std::forward<Args>(args)...);
		}

		static constexpr Concept s_vtable = { _dtor, _move, _invoke };

		UniquePtr<F> m_callable;
	};
}
