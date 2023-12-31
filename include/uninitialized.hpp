#ifndef _UNINITIALIZED_HPP__
#define _UNINITIALIZED_HPP__

#include "allocator.hpp"
#include "iterator.hpp"

namespace stl {

namespace {

template <typename T>
constexpr T *pointer_traits(T *p) {
	return p;
}

template <typename T>
const T *pointer_traits(const T *p) {
	return p;
}

template <typename I>
constexpr typename I::pointer pointer_traits(I p) {
	return pointer_traits(p.base());
}

} // namespace

template <typename InputIterator, typename ForwardIterator, typename Alloc>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
	ForwardIterator result, Alloc allocator) {
	auto p = first;
	auto q = result;
	for(;p != last;++p, ++q) {
		allocator.construct(pointer_traits(q), *p);
	}
	return result;
}

template <typename InputIterator, typename ForwardIterator, typename Alloc>
ForwardIterator uninitialized_move(InputIterator first, InputIterator last,
	ForwardIterator result, Alloc allocator) {
	auto p = first;
	auto q = result;
	for(;p != last;++p, ++q) {
		allocator.construct(pointer_traits(q), std::move(*p));
	}
	return result;
}

template <typename InputIterator, typename Alloc>
void uninitialized_fill(InputIterator first, InputIterator last, Alloc allocator) {
	auto p = first;
	for(;p != last;++p) {
		allocator.construct(pointer_traits(p));
	}
}

template <typename InputIterator, typename Size, typename Alloc>
void uninitialized_fill_n(InputIterator first, Size size, Alloc allocator) {
	auto p = first;
	for(Size s = 0;s < size;++s, ++p) {
		allocator.construct(pointer_traits(p));
	}
}

template <typename InputIterator, typename T, typename Alloc>
void uninitialized_fill(const T &x, InputIterator first, InputIterator last, Alloc allocator) {
	auto p = first;
	for(;p != last;++p) {
		allocator.construct(pointer_traits(p), x);
	}
}

template <typename InputIterator, typename Size, typename T, typename Alloc>
void uninitialized_fill_n(const T &x, InputIterator first, Size size, Alloc allocator) {
	auto p = first;
	for(Size s = 0;s < size;++s, ++p) {
		allocator.construct(pointer_traits(p), x);
	}
}

template <typename InputIterator, typename Alloc>
void initialized_destory(InputIterator first, InputIterator last, Alloc allocator) {
	auto p = first;
	for(;p != last;++p) {
		allocator.destory(pointer_traits(p));
	}
}

template <typename InputIterator, typename Size, typename Alloc>
void initialized_destory_n(InputIterator first, Size size, Alloc allocator) {
	auto p = first;
	for(Size s = 0;s < size;++s, ++p) {
		allocator.destory(pointer_traits(p));
	}
}

} // namespace stl

#endif // _UNINITIALIZED_HPP__