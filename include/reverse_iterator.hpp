#ifndef _REVERSE_ITERATOR_HPP__
#define _REVERSE_ITERATOR_HPP__

#include "iterator.hpp"

namespace stl {

template <typename I>
class ReverseIterator {
public:
	using iterator_category = typename iterator_traits<I>::iterator_category;
	using value_type = typename iterator_traits<I>::value_type;
	using difference_type = typename iterator_traits<I>::difference_type;
	using pointer = typename iterator_traits<I>::pointer;
	using reference = typename iterator_traits<I>::reference;

	using iterator_type = I;
	using self = ReverseIterator<I>;
private:
	I m_iterator_;
public:
	explicit ReverseIterator(I i = I()) :m_iterator_(i) {
	}

	ReverseIterator(const self &ri) :m_iterator_(ri.m_iterator_) {
	}

	self &operator=(const self &ri) {
		m_iterator_ = ri.m_iterator_;
	}

	bool operator==(const self &ri) {
		return m_iterator_ == ri.m_iterator_;
	}

	bool operator!=(const self &ri) {
		return !operator==(ri);
	}

	iterator_type base() const {
		return m_iterator_;
	}

	reference operator*() {
		auto tmp = m_iterator_;
		return *(--tmp);
	}

	pointer operator->() {
		return &(operator*());
	}

	self &operator++() {
		--m_iterator_;
		return *this;
	}

	self operator++(int) {
		auto p = *this;
		--m_iterator_;
		return p;
	}

	self &operator--() {
		++m_iterator_;
		return *this;
	}

	self operator--(int) {
		auto p = *this;
		++m_iterator_;
		return p;
	}

	self &operator+=(difference_type n) {
		m_iterator_ -= n;
		return *this;
	}

	self operator+(difference_type n) {
		return self(m_iterator_ - n);
	}

	self &operator-=(difference_type n) {
		m_iterator_ += n;
		return *this;
	}

	self operator-(difference_type n) {
		return self(m_iterator_ + n);
	}
}; // class ReverseIterator

} // namespace stl

#endif // _REVERSE_ITERATOR_HPP__