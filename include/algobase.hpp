#ifndef _ALGOBASE_HPP__
#define _ALGOBASE_HPP__

#include "iterator.hpp"
#include "functional.hpp"
#include "utility.hpp"
#include "type_traits.hpp"

#include <string.h>

namespace stl {

template <typename InputIterator1, typename InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	using T = typename iterator_traits<InputIterator1>::value_type;
	return equal(first1, last1, first2, equal_to<T>());
}

template <typename InputIterator1, typename InputIterator2, typename BP>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BP op) {
	for(;first1 != last1;++first1, ++first2) {
		if(!op(*first1, *first2)) {
			return false;
		}
	}
	return true;
}

template <typename InputIterator, typename T>
void fill(InputIterator first, InputIterator last, T value) {
	for(;first != last;++first) {
		*first = value;
	}
}

template <typename InputIterator, typename Size, typename T>
InputIterator fill_n(InputIterator first, Size size, T value) {
	while(size--) {
		*first = value;
		++first;
	}
	return first;
}

template <typename ForwardIterator1, typename ForwardIterator2>
void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
	if(a == b) {
		return;
	}
	using T = typename iterator_traits<ForwardIterator1>::value_type;
	T tmp = move(*a);
	*a = move(*b);
	*b = move(tmp);
}

template <typename ForwardIterator1, typename ForwardIterator2>
bool lexicographical_compare(ForwardIterator1 first1, ForwardIterator1 last1,
	ForwardIterator2 first2, ForwardIterator2 last2) {
	using T = typename iterator_traits<ForwardIterator1>::value_type;
	return lexicographical_compare(first1, last1, first2, last2, less<T>());
}

template <typename ForwardIterator1, typename ForwardIterator2, typename Compare>
bool lexicographical_compare(ForwardIterator1 first1, ForwardIterator1 last1,
	ForwardIterator2 first2, ForwardIterator2 last2, Compare comp) {
	for(;first1 != last1 && first2 != last2;++first1, ++first2) {
		if(comp(*first1, *first2)) {
			return true;
		}
		if(comp(*first2, *first1)) {
			return false;
		}
	}
	return first1 == last1 && first2 != last2;
}

template <typename T, typename Compare = less<T>>
const T &max(const T &a, const T &b, Compare comp = Compare()) {
	return comp(a, b) ? b : a;
}

template <typename T, typename Compare = less<T>>
const T &min(const T &a, const T &b, Compare comp = Compare()) {
	return comp(a, b) ? a : b;
}

template <typename T>
void swap(T &a, T &b) {
	T tmp = move(a);
	a = move(b);
	b = move(tmp);
}

template <typename InputIterator1, typename InputIterator2>
stl::Pair<InputIterator1, InputIterator2> mismatch(
	InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2) {
	using T = typename iterator_traits<InputIterator1>::value_type;
	return mismatch(first1, last1, first2, equal_to<T>());
}

template <typename InputIterator1, typename InputIterator2, typename Compare>
stl::Pair<InputIterator1, InputIterator2> mismatch(
	InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, Compare comp) {
	for(;first1 != last1 && comp(*first1, *first2);++first1, ++first2);
	return stl::Pair<InputIterator1, InputIterator2>(first1, first2);
}

namespace {

static void memmove(char *res, const char *first, size_t n) {
	::memcpy(res, first, n);
}

template <typename T>
static T *__copy_t(const T *first, const T *last, T *result, true_type) {
	memmove((char *)result, (char *)first, sizeof(T) * (last - first));
	return result + (last - first);
}

template <typename T>
static T *__copy_t(const T *first, const T *last, T *result, false_type) {
	return __copy_d(first, last, result, distance_type(first));
}

template <typename InputIterator1, typename InputIterator2>
static InputIterator2 __copy(InputIterator1 first, InputIterator1 last,
	InputIterator2 result, stl::input_iterator_tag) {
	for(;first != last;++result, ++first) {
		*result = *first;
	}
	return result;
}

template <typename InputIterator1, typename InputIterator2, typename Distance>
static InputIterator2 __copy_d(InputIterator1 first, InputIterator1 last,
	InputIterator2 result, Distance *) {
	for(Distance i = last - first;i >= 0;--i, ++result, ++first) {
		*result = *first;
	}
	return result;
}

template <typename InputIterator1, typename InputIterator2>
static InputIterator2 __copy(InputIterator1 first, InputIterator1 last,
	InputIterator2 result, stl::random_access_iterator_tag) {
	return __copy_d(first, last, result, distance_type(first));
}

template <typename InputIterator1, typename InputIterator2>
struct __copy_dispatch {
	InputIterator2 operator()(InputIterator1 first, InputIterator1 last,
		InputIterator2 result) {
		return __copy(first, last, result, iterator_category(first));
	}
};

template <typename T>
struct __copy_dispatch<T *, T *> {
	T *operator()(T *first, T *last, T *result) {
		return __copy_t(first, last, result,
			typename stl::type_traits<T>::has_trivial_assignment_operator());
	}
};

template <typename T>
struct __copy_dispatch<const T *, T *> {
	T *operator()(const T *first, const T *last, T *result) {
		return __copy_t(first, last, result,
			typename stl::type_traits<T>::has_trivial_assignment_operator());
	}
};

} // namespace

template <typename InputIterator1, typename InputIterator2>
InputIterator2 copy(InputIterator1 first, InputIterator1 last, InputIterator2 result) {
	return __copy_dispatch<InputIterator1, InputIterator2>()(first, last, result);
}

char *copy(const char *first, const char *last, char *result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

wchar_t *copy(const wchar_t *first, const wchar_t *last, wchar_t *result) {
	memmove((char *)result, (char *)first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}

} // namespace stl

#endif // _ALGOBASE_HPP__