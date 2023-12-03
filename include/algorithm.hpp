#ifndef _ALGORITHM_HPP__
#define _ALGORITHM_HPP__

#include "iterator.hpp"
#include "functional.hpp"
#include "algobase.hpp"
#include "algo.hpp"

namespace stl {

template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator d_first) {
	using T = typename iterator_traits<InputIterator1>::value_type;
	return set_difference(first1, last1, first2, last2, d_first, less<T>());
}

template <typename InputIterator1, typename InputIterator2,
	typename OutputIterator, typename Compare>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator d_first, Compare comp) {
	while(first1 != last1 && first2 != last2) {
		if(comp(*first1, *first2)) {
			*d_first++ = *first1;
			++first1;
		} else if(comp(*first2, *first1)) {
			++first2;
		} else {
			++first1;
			++first2;
		}
	}
	return stl::copy(first1, last1, d_first);
}

template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator d_first) {
	using T = typename iterator_traits<InputIterator1>::value_type;
	return set_intersection(first1, last1, first2, last2, d_first, less<T>());
}

template <typename InputIterator1, typename InputIterator2,
	typename OutputIterator, typename Compare>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator d_first, Compare comp) {
	while(first1 != last1 && first2 != last2) {
		if(comp(*first1, *first2)) {
			++first1;
		} else if(comp(*first2, *first1)) {
			++first2;
		} else {
			*d_first++ = *first1;
			++first1;
			++first2;
		}
	}
	return d_first;
}

template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator d_first) {
	using T = typename iterator_traits<InputIterator1>::value_type;
	return set_symmetric_difference(first1, last1, first2, last2, d_first, less<T>());
}

template <typename InputIterator1, typename InputIterator2,
	typename OutputIterator, typename Compare>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator d_first, Compare comp) {
	while(first1 != last1 && first2 != last2) {
		if(comp(*first1, *first2)) {
			*d_first++ = *first1;
			++first1;
		} else if(comp(*first2, *first1)) {
			*d_first++ = *first2;
			++first2;
		} else {
			++first1;
			++first2;
		}
	}
	return stl::copy(first1, last1, stl::copy(first2, last2, d_first));
}

template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator d_first) {
	using T = typename iterator_traits<InputIterator1>::value_type;
	return set_union(first1, last1, first2, last2, d_first, less<T>());
}

template <typename InputIterator1, typename InputIterator2,
	typename OutputIterator, typename Compare>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator d_first, Compare comp) {
	while(first1 != last1 && first2 != last2) {
		if(comp(*first1, *first2)) {
			*d_first++ = *first1++;
		} else if(comp(*first2, *first1)) {
			*d_first++ = *first2++;
		} else {
			*d_first++ = *first1++;
			++first2;
		}
	}
	return stl::copy(first1, last1, stl::copy(first2, last2, d_first));
}

template <typename InputIterator1, typename InputIterator2>
bool include(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2) {
	using T = typename iterator_traits<InputIterator1>::value_type;
	return include(first1, last1, first2, last2, less<T>());
}

template <typename InputIterator1, typename InputIterator2, typename Compare>
bool include(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, Compare comp) {
	while(first1 != last1 && first2 != last2) {
		if(comp(*first1, *first2)) {
			++first1;
		} else if(comp(*first2, *first1)) {
			return false;
		} else {
			++first1;
			++first2;
		}
	}
	return first2 == last2;
}

} // namespace stl

#endif // _ALGORITHM_HPP__