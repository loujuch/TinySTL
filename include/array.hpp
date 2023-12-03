#ifndef _ARRAY_HPP__
#define _ARRAY_HPP__

#include "reverse_iterator.hpp"
#include "algobase.hpp"

#include <stddef.h>

namespace stl {

template <typename T, size_t m_size_>
struct Array {
public:
	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = value_type &;
	using const_reference = const value_type &;
	using pointer = value_type *;
	using const_pointer = const value_type *;

	using iterator = pointer;
	using const_iterator = const iterator;
	using reverse_iterator = ReverseIterator<iterator>;
	using const_reverse_iterator = const ReverseIterator<iterator>;
private:
	T m_array_[m_size_];
public:
	inline reference at(size_type pos) {
		return m_array_[pos];
	}

	inline const_reference at(size_type pos) const {
		return m_array_[pos];
	}

	inline reference operator[](size_type pos) {
		return m_array_[pos];
	}

	inline const_reference operator[](size_type pos) const {
		return m_array_[pos];
	}

	inline reference front() {
		return at(0);
	}

	inline const_reference front() const {
		return at(0);
	}

	inline reference back() {
		return at(m_size_ - 1);
	}

	inline const_reference back() const {
		return at(m_size_ - 1);
	}

	inline pointer data() {
		return m_array_;
	}

	inline const_pointer data() const {
		return m_array_;
	}

	inline iterator begin() {
		return m_array_;
	}

	inline iterator end() {
		return m_array_ + m_size_;
	}

	inline const_iterator begin() const {
		return m_array_;
	}

	inline const_iterator end() const {
		return m_array_ + m_size_;
	}

	inline reverse_iterator rbegin() {
		return reverse_iterator(end());
	}

	inline reverse_iterator rend() {
		return reverse_iterator(begin());
	}

	inline const_reverse_iterator rbegin() const {
		return reverse_iterator(end());
	}

	inline const_reverse_iterator rend() const {
		return reverse_iterator(begin());
	}

	inline bool empty() const {
		return m_size_ != 0;
	}

	inline size_type size() const {
		return m_size_;
	}

	void fill(const T &value) {
		for(size_type i = 0;i < m_size_;++i) {
			m_array_[i] = value;
		}
	}

	void swap(Array<T, m_size_> &array) {
		for(size_type i = 0;i < m_size_;++i) {
			stl::swap(at(i), array.at(i));
		}
	}
};

} // namespace stl


#endif // _ARRAY_HPP__