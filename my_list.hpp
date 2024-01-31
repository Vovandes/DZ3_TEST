#pragma once

#include "custom_allocator.hpp"

//namespace vs {
//	template<typename Q>
//	struct Node {
//		Node* next;
//		Q data;
//
//		//template<typename...Args>
//		//Node(Args&&...args)
//		//	: next(nullptr)
//		//	, data(std::forward<Args>(args)...) {}
//	};
//}

namespace vs {
	template <typename Q, typename _A = std::allocator<Q>>
	class my_list {
	public:
	struct Node {
		Node* next;
		Q data;
	};
		template <typename Q>
		class my_list_const_iterator {
		public:
			using value_type = Q;
			using pointer = const Q*;
			using reference = const Q&;
			using iterator_category = std::forward_iterator_tag;

			using _self = my_list_const_iterator<Q>;
			Node* _node;

			my_list_const_iterator() : _node(nullptr) {};

			my_list_const_iterator(Node* pNode) : _node(pNode) {};

			reference operator*() const {
				return _node->data;
			}

			pointer operator->() const {
				return &(_node->data);
			}

			_self& operator++() {
				if (_node == nullptr) {
					return *this;
				}
				else {
					_node = _node->next;
				}
				return *this;
			}

			bool operator==(const _self& rhs) const {
				return _node == rhs._node;
			}

			bool operator!=(const _self& rhs) const {
				return _node != rhs._node;
			}
		};

		using Allocator = typename std::allocator_traits<_A>::template rebind_alloc<Q>;
		using QAllocatorTraits = std::allocator_traits<Allocator>;
		using NodeAllocator = typename QAllocatorTraits::template rebind_alloc<Node>;
		using NodeAllocatorTraits = std::allocator_traits<NodeAllocator>;

		my_list()
			: m_head(nullptr)
			, m_tail(nullptr)
			, m_alloc() {
			std::cout << "LOG: LL ctor..." << std::endl;
		};

		my_list(const my_list& object)
			: m_head(nullptr)
			, m_tail(nullptr)
			, m_alloc() {
			std::cout << "LOG: LL COPY ctor..." << std::endl;
			copyList(object);
		}

		template <typename TAlloc>
		my_list(const my_list<Q, TAlloc>& object)
			: m_head(nullptr)
			, m_tail(nullptr)
			, m_alloc() {
			std::cout << "LOG: LL COPY ctor (diff allocs)..." << std::endl;
			copyList(object);
		}

		template <typename TAlloc>
		void copyList(const my_list<Q, TAlloc>& object) {
			Node* curNode = object.cbegin()._node;
			while (curNode != nullptr) {
				this->push_back(curNode->data);
				curNode = curNode->next;
			}
		}

		my_list(my_list&& object) noexcept
			: m_head(object.m_head)
			, m_tail(object.m_tail)
			, m_alloc(object.m_alloc) {
			std::cout << "LOG: LL MOVE ctor..." << std::endl;
			object.m_head = nullptr;
			object.m_tail = nullptr;
		}

		template <typename TAlloc>
		my_list(my_list<Q, TAlloc>&& object)
			: m_head(nullptr)
			, m_tail(nullptr)
			, m_alloc() {
			std::cout << "LOG: LL MOVE ctor (diff allocs)..." << std::endl;
			if (object.cbegin()._node != nullptr) {
				this->m_head = m_alloc.allocate(1);
				m_alloc.construct(m_head, std::move(*object.cbegin()._node));
				auto p_dst = m_head;
				auto p_object = object.cbegin()._node;
				while (p_object->next != nullptr) {
					p_dst->next = m_alloc.allocate(1);
					p_dst = p_dst->next;
					p_object = p_object->next;
					m_alloc.construct(p_dst, std::move(*p_object));
				}
			}
		}

		~my_list() {
			std::cout << "LOG: LL dtor..." << std::endl;
			std::cout << "LOG: LL m_head (before destruct) = " << m_head << "..." << std::endl;
			//auto current = m_head;
			//while (current != nullptr) {
			//	auto p_rm = current;
			//	current = current->next;
			//	m_alloc.destroy(p_rm);
			//	m_alloc.deallocate(p_rm, 1);
			//}

			clear();
		}

		template <typename ...Args>
		void push_back(Args&&...args) {
			auto* new_node = NodeAllocatorTraits::allocate(m_alloc, 1);
			NodeAllocatorTraits::construct(m_alloc, &(new_node->data), std::forward<Args>(args)...);
			new_node->next = nullptr;

			if (m_head == nullptr) {
				m_head = new_node;
				m_tail = new_node;
			}
			else {
				m_tail->next = new_node;
				m_tail = new_node;
				m_tail->next = nullptr;
			}
		}

		void clear() {
			auto* current = m_head;
			while (current != nullptr) {
				auto* p_rm = current;
				current = current->next;
				NodeAllocatorTraits::destroy(m_alloc, p_rm);
				NodeAllocatorTraits::deallocate(m_alloc, p_rm, 1);
				
			}
		}

		my_list_const_iterator<Q> cbegin() const noexcept {
			return my_list_const_iterator<Q>(m_head);
		}

		my_list_const_iterator<Q> cend() noexcept {
			return my_list_const_iterator<Q>();
		}

	private:
		Node* m_head;// = nullptr;
		Node* m_tail;// = nullptr;

		NodeAllocator m_alloc;
	};
}