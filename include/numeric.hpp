#ifndef _NUMERIC_HPP__
#define _NUMERIC_HPP__

#include "functional.hpp"
#include "iterator.hpp"

namespace stl {

template <typename InputIterator, typename T, typename BOP = plus<T>>
T accumulate(InputIterator first, InputIterator last, T init, BOP bop = BOP()) {
	for(;first != last;++first) {
		init = std::move(bop(init, *first));
	}
	return std::move(init);
}

template <typename InputIterator, typename OutputIterator, typename T, typename BOP = minus<T>>
static OutputIterator __adjacent_difference(InputIterator first, InputIterator last,
	OutputIterator d_first, T *, BOP op = BOP()) {
	if(first == last) {
		return d_first;
	}
	auto p = first;
	*d_first++ = *p++;
	for(;p != last;++p, ++first, ++d_first) {
		*d_first = op(*p, *first);
	}
	return d_first;
}

template <typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
	OutputIterator d_first) {
	return __adjacent_difference(first, last, d_first, value_type(first));
}

template <typename InputIterator, typename OutputIterator, typename BOP>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
	OutputIterator d_first, BOP op) {
	return __adjacent_difference(first, last, d_first, value_type(first), op);
}

template <typename InputIterator, typename OutputIterator, typename T, typename BOP = plus<T>>
static OutputIterator __partial_sum(InputIterator first, InputIterator last,
	OutputIterator d_first, T *, BOP op = BOP()) {
	if(first == last) {
		return d_first;
	}
	T tmp = *first++;
	*d_first++ = tmp;
	for(;first != last;++first, ++d_first) {
		tmp = op(tmp, *first);
		*d_first = tmp;
	}
	return d_first;
}

template <typename InputIterator, typename OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last,
	OutputIterator d_first) {
	return __partial_sum(first, last, d_first, value_type(first));
}

template <typename InputIterator, typename OutputIterator, typename BOP>
OutputIterator partial_sum(InputIterator first, InputIterator last,
	OutputIterator d_first, BOP op) {
	return __partial_sum(first, last, d_first, value_type(first), op);
}

template <typename InputIterator1, typename InputIterator2, typename T,
	typename BOP1 = plus<T>, typename BOP2 = multiplies<T>
>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init,
	BOP1 op1 = BOP1(), BOP2 op2 = BOP2()) {
	for(;first1 != last1;++first1, ++first2) {
		init = op1(init, op2(*first1, *first2));
	}
	return init;
}

template <typename InputIterator, typename T>
void iota(InputIterator first, InputIterator last, T value) {
	for(;first != last;++first) {
		*first = value;
		++value;
	}
}

} // namespace stl

#endif // _NUMERIC_HPP__