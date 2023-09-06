#ifndef _ITERATOR_HPP__
#define _ITERATOR_HPP__

#include <stddef.h>

namespace stl {

// 迭代器类型
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag :public input_iterator_tag {};
struct bidirectional_iterator_tag :public forward_iterator_tag {};
struct random_access_iterator_tag :public bidirectional_iterator_tag {};

// 迭代器基类
template <typename Category,
	typename T,
	typename Distance = ::ptrdiff_t,
	typename Pointer = T *,
	typename Reference = T &>
class Iterator {
public:
	using iterator_category = Category;
	using value_type = T;
	using difference_type = Distance;
	using pointer = Pointer;
	using reference = Reference;
}; // class Iterator

// 萃取迭代器类型
template <typename I>
struct iterator_traits {
	using iterator_category = typename I::iterator_category;
	using value_type = typename I::value_type;
	using difference_type = typename I::difference_type;
	using pointer = typename I::pointer;
	using reference = typename I::reference;
}; // struct iterator_traits

// 萃取指针类型
template <typename T>
struct iterator_traits<T *> {
	using iterator_category = random_access_iterator_tag;
	using value_type = T;
	using difference_type = ::ptrdiff_t;
	using pointer = T *;
	using reference = T &;
}; // struct iterator_traits<T *>

// 萃取常量指针类型
template <typename T>
struct iterator_traits<const T *> {
	using iterator_category = random_access_iterator_tag;
	using value_type = T;
	using difference_type = ::ptrdiff_t;
	using pointer = const T *;
	using reference = const T &;
}; // struct iterator_traits<const T *>

// 获取迭代器类型
template <typename I>
inline typename iterator_traits<I>::iterator_category
iterator_category(const I &) {
	return typename iterator_traits<I>::iterator_category();
}

// 获取值类型
template <typename I>
inline typename iterator_traits<I>::value_type *
value_type(const I &) {
	return static_cast<typename iterator_traits<I>::value_type *>(nullptr);
}

// 获取距离类型
template <typename I>
inline typename iterator_traits<I>::difference_type *
difference_type(const I &) {
	return static_cast<typename iterator_traits<I>::difference_type *>(nullptr);
}

// 获取两迭代器间距离
template <typename InputIterator>
static inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag) {
	typename iterator_traits<InputIterator>::difference_type n = 0;
	while(last != first) {
		++n;
		++first;
	}
	return n;
}

// 获取两迭代器间距离
template <typename RandomAccessIterator>
static inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
	return last - first;
}

// 获取两迭代器间距离
template <typename InputIterator>
static inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
	if(first == last) {
		return 0;
	}
	return __distance(first, last, iterator_category(first));
}

// 将迭代器前进/后退n步
template <typename InputIterator, typename Distance>
static inline void __advance(InputIterator &i, Distance n, InputIterator) {
	if(n >= 0) {
		while(n--) {
			++i;
		}
	} else {
	}
}

// 将迭代器前进/后退n步
template <typename BidirectionalIterator, typename Distance>
static inline void __advance(BidirectionalIterator &i, Distance n, bidirectional_iterator_tag) {
	if(n >= 0) {
		while(n--) {
			++i;
		}
	} else {
		while(n++) {
			--i;
		}
	}
}

// 将迭代器前进/后退n步
template <typename RandomAccessIterator, typename Distance>
static inline void __advance(RandomAccessIterator &i, Distance n, random_access_iterator_tag) {
	i += n;
}

// 将迭代器前进/后退n步
template <typename InputIterator, typename Distance>
inline void advance(InputIterator &i, Distance n) {
	__advance(i, n, iterator_category(i));
}

} // namespace stl

#endif // _ITERATOR_HPP__