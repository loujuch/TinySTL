#ifndef _ALGORITHM_HPP__
#define _ALGORITHM_HPP__

#include "iterator.hpp"
#include "functional.hpp"

namespace stl {

// 将指向的洞运往尾部
template <typename RandomIterator, typename Compare>
static void __adiust_heap(RandomIterator first, RandomIterator last, Compare comp,
	RandomIterator hole) {
	if(first == last) {
		return;
	}
	using difference_type = typename iterator_traits<RandomIterator>::difference_type;

	difference_type size = distance(first, last);
	difference_type i = distance(first, hole);

	while(i < size) {
		difference_type left = (i << 1) + 1;
		difference_type right = left + 1;
		if(right >= size) {
			std::swap(*(first + i), *(last - 1));
			break;
		}
		difference_type j = comp(*(first + left), *(first + right)) ? right : left;
		std::swap(*(first + i), *(first + j));
		i = j;
	}
}

template <typename RandomIterator>
bool is_heap(RandomIterator first, RandomIterator last) {
	using T = typename iterator_traits<RandomIterator>::value_type;
	return is_heap(first, last, less<T>());
}

template <typename RandomIterator, typename Compare>
bool is_heap(RandomIterator first, RandomIterator last, Compare comp) {
	if(first == last) {
		return true;
	}
	using difference_type = typename iterator_traits<RandomIterator>::difference_type;

	difference_type size = distance(first, last);

	for(difference_type i = 1;i < size;++i) {
		difference_type p = (i - 1) >> 1;
		if(comp(*(first + p), *(first + i))) {
			return false;
		}
	}
	return true;
}

template <typename RandomIterator>
RandomIterator is_heap_until(RandomIterator first, RandomIterator last) {
	using T = typename iterator_traits<RandomIterator>::value_type;
	return is_heap_until(first, last, less<T>());
}

template <typename RandomIterator, typename Compare>
RandomIterator is_heap_until(RandomIterator first, RandomIterator last, Compare comp) {
	if(first == last) {
		return first;
	}
	using difference_type = typename iterator_traits<RandomIterator>::difference_type;

	difference_type size = distance(first, last);
	difference_type i = 1;

	for(;i < size;++i) {
		difference_type p = (i - 1) >> 1;
		if(comp(*(first + p), *(first + i))) {
			break;
		}
	}
	return first + i;
}

template <typename RandomIterator>
void make_heap(RandomIterator first, RandomIterator last) {
	using T = typename iterator_traits<RandomIterator>::value_type;
	make_heap(first, last, less<T>());
}

template <typename RandomIterator, typename Compare>
void make_heap(RandomIterator first, RandomIterator last, Compare comp) {
	using difference_type = typename iterator_traits<RandomIterator>::difference_type;

	difference_type size = distance(first, last);
	difference_type i = (size - 1) >> 1;

	for(;i >= 0;--i) {
		difference_type ii = i;
		while(true) {
			difference_type left = (i << 1) + 1;
			difference_type right = left + 1;
			difference_type j;
			if(left >= size) {
				break;
			} else if(right == size) {
				j = left;
			} else {
				j = comp(*(first + left), *(first + right)) ? right : left;
			}
			if(comp(*(first + i), *(first + j))) {
				std::swap(*(first + i), *(first + j));
				i = j;
			} else {
				break;
			}
		}
	}
}

template <typename RandomIterator>
void push_heap(RandomIterator first, RandomIterator last) {
	using T = typename iterator_traits<RandomIterator>::value_type;
	push_heap(first, last, less<T>());
}

template <typename RandomIterator, typename Compare>
void push_heap(RandomIterator first, RandomIterator last, Compare comp) {
	if(first == last) {
		return;
	}
	using difference_type = typename iterator_traits<RandomIterator>::difference_type;

	difference_type i = distance(first, last) - 1;

	while(i != 0) {
		difference_type p = (i - 1) >> 1;
		if(comp(*(first + p), *(first + i))) {
			std::swap(*(first + p), *(first + i));
		} else {
			break;
		}
		i = p;
	}
}

template <typename RandomIterator>
void pop_heap(RandomIterator first, RandomIterator last) {
	using T = typename iterator_traits<RandomIterator>::value_type;
	pop_heap(first, last, less<T>());
}

template <typename RandomIterator, typename Compare>
void pop_heap(RandomIterator first, RandomIterator last, Compare comp) {
	__adiust_heap(first, last, comp, first);
}

template <typename RandomIterator>
void sort_heap(RandomIterator first, RandomIterator last) {
	using T = typename iterator_traits<RandomIterator>::value_type;
	sort_heap(first, last, less<T>());
}

template <typename RandomIterator, typename Compare>
void sort_heap(RandomIterator first, RandomIterator last, Compare comp) {
	while(first != last) {
		pop_heap(first, last--);
	}
}

} // namespace stl

#endif // _ALGORITHM_HPP__