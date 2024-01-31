#pragma once

#include <iostream>

#include <map>
#include <array>

#define __PRETTY_FUNCTION__  __FUNCSIG__

namespace vs {
	template<typename Q, size_t container_count_max>
	class custom_allocator {
	public:
		// Using:
		using value_type = Q;
		using pointer = Q*;
		using const_pointer = const Q*;
		using reference = Q&;
		using const_reference = const Q&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		template<typename U, size_t container_count_max_U = container_count_max>
		struct rebind {
			using other = vs::custom_allocator<U, container_count_max_U>;
		};

		custom_allocator() 
			: m_elem_count{ 0 }
			, m_elem_pool {	container_count_max	}
		{}

		~custom_allocator() {}

		template <typename U, size_t container_count_max_U = container_count_max>
		explicit custom_allocator(const custom_allocator<U, container_count_max_U>& object) noexcept
			: m_elem_count{ object.get_elem_count() }
			, m_elem_pool{ object.get_pool()}
		{
			std::cout << "Copy CTOR call..." << std::endl;
		}

		Q* allocate(size_t n) {
			m_counter += n;
			std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
			if (m_elem_count + n > m_elem_pool) {
				throw std::bad_alloc();
			}
			if (!m_inner_ptr) {
				////
				m_inner_ptr = reinterpret_cast<Q*>(malloc(container_count_max * sizeof(Q)));
			}
			if (!m_inner_ptr) {
				throw std::bad_alloc();
			}
			Q* tmp = m_inner_ptr + m_counter;
			if (m_counter > 1) {
				m_elem_count += n;
			}
			return tmp;
		}

		void deallocate(Q* ptr, size_t n) {
			std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
			if (!ptr) {
				return;
			}
			if (m_counter > 1) {
				m_elem_count -= n;
			}
			m_counter -= n;
			if (m_elem_count != 0) {
				return;
			}
			free(m_inner_ptr);
			m_inner_ptr = nullptr;
		}

		//template<typename U, typename ...Args>
		//void construct(U* ptr, Args &&...args) {
		//	std::cout << __PRETTY_FUNCTION__ << std::endl;
		//	new(ptr) U(std::forward<Args>(args)...);
		//};

		//void destroy(Q* ptr) {
		//	std::cout << __PRETTY_FUNCTION__ << std::endl;
		//	ptr->~Q();
		//}

		size_t get_elem_count() const { return m_elem_count; }
		size_t get_pool() const { return m_elem_pool; }

		using propagate_on_container_copy_assignment = std::true_type;
		using propagate_on_container_move_assignment = std::true_type;
		using propagate_on_container_swap = std::true_type;
	private:
		Q* m_inner_ptr{ nullptr };
		size_t  m_elem_count;
		size_t  m_elem_pool;
		static size_t m_counter;
	};
}

template<typename Q, size_t container_count_max>
size_t vs::custom_allocator<Q, container_count_max>::m_counter = 0;

template <class T, size_t container_count_max, class U>
constexpr bool operator== (const vs::custom_allocator<T, container_count_max>& lhs, const vs::custom_allocator<U, container_count_max>& rhs) noexcept
{
	return true;
}

template <class T, size_t container_count_max, class U>
constexpr bool operator!= (const vs::custom_allocator<T, container_count_max>& lhs, const vs::custom_allocator<U, container_count_max>& rhs) noexcept
{
	return false;
}

namespace vs {
	const int Factorial(int value);

	///////////////////////////////
	template<size_t n>
	class Factoriall {
	public:
		static const size_t value = Factoriall<n - 1>::value * n;
	};

	template<>
	class Factoriall<0> {
	public:
		static const size_t value = 1;
	};

	template<size_t container_size>
	class FactorialContainer {
	public:
		FactorialContainer()
			: m_fact_container{std::array<size_t, container_size>()}
		{}
	private:
		std::array<size_t, container_size> m_fact_container;
	};
	///////////////////////////////

	void PrintContainerMap(std::map<int, int, std::less<int>, vs::custom_allocator<std::pair<const int, int>, 10>>& dict);
}
