#ifndef _UNINITIALIZED_HPP__
#define _UNINITIALIZED_HPP__

#include "allocator.hpp"
#include "iterator.hpp"

namespace stl {

template <typename InputIterator, typename ForwardIterator, typename Alloc = Allocator<typename iterator_traits<InputIterator>::value_type>>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, Alloc allocator = Alloc()) {
	auto p = first;
	auto q = result;
	for(;p != last;++p, ++q) {
		allocator.construct(q, *p);
	}
	return result;
}

template <typename InputIterator, typename ForwardIterator, typename Alloc = Allocator<typename iterator_traits<InputIterator>::value_type>>
ForwardIterator uninitialized_move(InputIterator first, InputIterator last, ForwardIterator result, Alloc allocator = Alloc()) {
	auto p = first;
	auto q = result;
	for(;p != last;++p, ++q) {
		allocator.construct(q, std::move(*p));
	}
	return result;
}

template <typename InputIterator, typename Alloc = Allocator<typename iterator_traits<InputIterator>::value_type>>
void uninitialized_fill(InputIterator first, InputIterator last, Alloc allocator = Alloc()) {
	auto p = first;
	for(;p != last;++p) {
		allocator.construct(p);
	}
}

template <typename InputIterator, typename Size, typename Alloc = Allocator<typename iterator_traits<InputIterator>::value_type>>
void uninitialized_fill_n(InputIterator first, Size size, Alloc allocator = Alloc()) {
	auto p = first;
	for(Size s = 0;s < size;++s, ++p) {
		allocator.construct(p);
	}
}

template <typename InputIterator, typename T, typename Alloc = Allocator<typename iterator_traits<InputIterator>::value_type>>
void uninitialized_fill(const T &x, InputIterator first, InputIterator last, Alloc allocator = Alloc()) {
	auto p = first;
	for(;p != last;++p) {
		allocator.construct(p, x);
	}
}

template <typename InputIterator, typename Size, typename T, typename Alloc = Allocator<typename iterator_traits<InputIterator>::value_type>>
void uninitialized_fill_n(const T &x, InputIterator first, Size size, Alloc allocator = Alloc()) {
	auto p = first;
	for(Size s = 0;s < size;++s, ++p) {
		allocator.construct(p, x);
	}
}

template <typename InputIterator, typename Alloc = Allocator<typename iterator_traits<InputIterator>::value_type>>
void initialized_destory(InputIterator first, InputIterator last, Alloc allocator = Alloc()) {
	auto p = first;
	for(;p != last;++p) {
		allocator.destory(p);
	}
}

template <typename InputIterator, typename Size, typename Alloc = Allocator<typename iterator_traits<InputIterator>::value_type>>
void initialized_destory_n(InputIterator first, Size size, Alloc allocator = Alloc()) {
	auto p = first;
	for(Size s = 0;s < size;++s, ++p) {
		allocator.destory(p);
	}
}

} // namespace stl

#endif // _UNINITIALIZED_HPP__