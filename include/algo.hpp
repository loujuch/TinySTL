#ifndef _ALGO_HPP__
#define _ALGO_HPP__

#include "functional.hpp"
#include "iterator.hpp"
#include "algobase.hpp"

namespace stl {

template <typename ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
	using T = typename iterator_traits<ForwardIterator>::value_type;
	return adjacent_find(first, last, equal_to<T>());
}

template <typename ForwardIterator, typename BP>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BP op) {
	ForwardIterator second = first;
	++second;
	for(;second != last; first = second, ++second) {
		if(op(*first, *second)) {
			return first;
		}
	}
	return last;
}

template <typename ForwardIterator, typename T>
typename iterator_traits<ForwardIterator>::difference_type
count(ForwardIterator first, ForwardIterator last, T value) {
	typename iterator_traits<ForwardIterator>::difference_type n = 0;
	for(;first != last;++first) {
		n += (*first == value);
	}
	return n;
}

template <typename ForwardIterator, typename BP>
typename iterator_traits<ForwardIterator>::difference_type
count_if(ForwardIterator first, ForwardIterator last, BP op) {
	typename iterator_traits<ForwardIterator>::difference_type n = 0;
	for(;first != last;++first) {
		if(op(*first)) {
			++n;
		}
	}
	return n;
}

template <typename InputIterator, typename T>
InputIterator find(InputIterator first, InputIterator last, T value) {
	for(;first != last && !(*first == value);++first);
	return first;
}

template <typename InputIterator, typename BP>
InputIterator find_if(InputIterator first, InputIterator last, BP op) {
	for(;first != last && !op(*first);++first);
	return first;
}

template <typename InputIterator1, typename InputIterator2>
InputIterator1 find_end(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2) {
	using T = typename iterator_traits<InputIterator1>::value_type;
	return find_end(first1, last1, first2, last2, equal_to<T>());
}

// TODO: 实现逆序版
template <typename InputIterator1, typename InputIterator2, typename Compare>
InputIterator1 find_end(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, Compare comp) {
	InputIterator1 tmp = last1;
	InputIterator1 q;
	InputIterator2 p;
	for(;first1 != last1;++first1) {
		for(p = first2, q = first1;p != last2 && q != last1 && comp(*p, *q);++p, ++q);
		if(p == last2) {
			tmp = first1;
		}
		if(q == last1) {
			break;
		}
	}
	return tmp;
}

template <typename InputIterator1, typename InputIterator2>
InputIterator1 find_first_of(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2) {
	using T = typename iterator_traits<InputIterator1>::value_type;
	return find_first_of(first1, last1, first2, last2, equal_to<T>());
}

template <typename InputIterator1, typename InputIterator2, typename Compare>
InputIterator1 find_first_of(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, Compare comp) {
	for(;first1 != last1;++first1) {
		for(auto p = first2;p != last2;++p) {
			if(comp(*first1, *p)) {
				return first1;
			}
		}
	}
	return last1;
}

template <typename InputIterator, typename Func>
Func for_each(InputIterator first, InputIterator last, Func f) {
	for(;first != last;++first) {
		f(*first);
	}
	return f;
}

template <typename ForwardIterator, typename Func>
void generate(ForwardIterator first, ForwardIterator last, Func gen) {
	for(;first != last;++first) {
		*first = gen();
	}
}

template <typename ForwardIterator, typename Size, typename Func>
void generate_n(ForwardIterator first, Size size, Func gen) {
	for(;size > 0;--size, ++first) {
		*first = gen();
	}
}

template <typename ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
	using T = typename iterator_traits<ForwardIterator>::value_type;
	return max_element(first, last, less<T>());
}

template <typename ForwardIterator, typename Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp) {
	if(first == last) {
		return last;
	}
	ForwardIterator result = first++;
	for(;first != last;++first) {
		if(comp(*result, *first)) {
			result = first;
		}
	}
	return result;
}

template <typename ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
	using T = typename iterator_traits<ForwardIterator>::value_type;
	return min_element(first, last, less<T>());
}

template <typename ForwardIterator, typename Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp) {
	if(first == last) {
		return last;
	}
	ForwardIterator result = first++;
	for(;first != last;++first) {
		if(comp(*first, *result)) {
			result = first;
		}
	}
	return result;
}

template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	using T = typename iterator_traits<OutputIterator>::value_type;
	return merge(first1, last1, first2, last2, result, less<T>());
}


template <typename InputIterator1, typename InputIterator2,
	typename OutputIterator, typename Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2,
	OutputIterator result, Compare comp) {
	while(first1 != last1 && first2 != last2) {
		if(comp(*first1, *first2)) {
			*result++ = *first1++;
		} else {
			*result++ = *first2++;
		}
	}
	return copy(first1, last1, copy(first2, last2, result));
}


} // namespace stl

#endif // _ALGO_HPP__