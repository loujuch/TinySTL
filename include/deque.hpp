#ifndef _DEQUE_HPP__
#define _DEQUE_HPP__

#include "iterator.hpp"
#include "allocator.hpp"

namespace stl {

template <typename T, size_t BufferSize>
class DequeIterator :public Iterator<random_access_iterator_tag, T> {
	using map_pointer = pointer *;
	using self = DequeIterator<T>;
private:
	pointer m_first_;
	pointer m_last_;
	pointer m_cur_;

	map_pointer m_map_;
public:
	DequeIterator(pointer cur = nullptr, map_pointer map_ptr = nullptr) :
		m_cur_(cur), m_map_(map_ptr) {
		if(m_map_ != nullptr) {
			m_first_ = *m_map_;
			m_last_ = m_first_ + BufferSize;
		}
	}
}; // class DequeIterator

template <typename T, size_t BufferSize, typename ALLOC = Allocator<T>>
class Deque {
public:
	using value_type = T;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using reference = value_type &;
	using const_reference = const value_type &;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	using iterator = DequeIterator;
	using const_iterator = const DequeIterator;

	using map_type = pointer *;
	using map_pointer = map_type *;
private:
	ALLOC m_buffer_allocator_;
	ALLOC::rebind<map_type>::other m_map_allocator_;

	iterator m_start_;
	iterator m_last_;

	map_pointer m_map_;

	size_type m_length_;
public:
	Deque() {
	}

	Deque(size_type n) {
	}

	Deque(size_type n, const_reference x) {
	}

	Deque(iterator first, iterator last) {
	}

	Deque(const Deque<T, ALLOC> &other) {
	}

	Deque(Deque<T, ALLOC> &&other) {
	}

	~Deque() {
	}

	// Deque<T, ALLOC> &operator=(const Deque<T, ALLOC> &other) {
	// }

	// Deque<T, ALLOC> &operator=(Deque<T, ALLOC> &&other) {
	// }

	// ALLOC get_allocator() const {
	// }

	// reference at(size_type n) {
	// }

	// const_reference at(size_type n) const {
	// }

	// reference operator[](size_type n) {
	// }

	// const_reference operator[](size_type n) const {
	// }

	// reference front(size_type n) {
	// }

	// const_reference front(size_type n) const {
	// }

	// reference back(size_type n) {
	// }

	// const_reference back(size_type n) const {
	// }

	iterator begin() {
		return m_start_;
	}

	const_iterator begin() const {
		return m_start_;
	}

	iterator end() {
		return m_last_;
	}

	const_iterator end() const {
		return m_last_;
	}

	// bool empty() const {
	// }

	// size_type size() const {
	// }

	// void shrink_to_fit() {
	// }

	// void clear() {
	// }
}; // class Deque 

} // namespace stl

#endif // _DEQUE_HPP__