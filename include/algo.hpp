#ifndef _ALGO_HPP__
#define _ALGO_HPP__

#include "functional.hpp"
#include "iterator.hpp"
#include "algobase.hpp"

namespace stl {

template <typename RandomIterator, typename Compare>
static void __adjust_heap(RandomIterator first, RandomIterator last,
	Compare comp, RandomIterator hole) {
	auto size = distance(first, last);
	auto i = distance(first, hole);

	while(true) {
		auto left = (i << 1) + 1;
		auto right = left + 1;
		auto j = left;
		if(left >= size) {
			break;
		} else if(right == size) {
			j = left;
		} else {
			j = comp(*(first + left), *(first + right)) ? right : left;
		}
		if(comp(*(first + i), *(first + j))) {
			iter_swap(first + i, first + j);
			i = j;
		} else {
			break;
		}
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
	auto size = distance(first, last);
	auto i = (size - 1) >> 1;

	for(;i >= 0;--i) {
		__adjust_heap(first, last, comp, first + i);
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
	iter_swap(first, last - 1);
	__adjust_heap(first, last - 1, comp, first);
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
InputIterator find(InputIterator first, InputIterator last, const T &value) {
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

template <typename BidirectionalIterator, typename Pred>
BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Pred pred) {
	while(first != last) {
		while(first != last && pred(*first)) {
			++first;
		}
		if(!(first != last)) {
			break;
		}
		--last;
		while(first != last && !pred(*last)) {
			--last;
		}
		if(first != last) {
			iter_swap(first++, last);
		}
	}
	return first;
}

template <typename ForwardIterator, typename OutputIterator, typename Pred>
ForwardIterator remove_copy_if(ForwardIterator first, ForwardIterator last,
	OutputIterator result, Pred pred) {
	for(;first != last;++first) {
		if(!pred(*first)) {
			*result++ = *first;
		}
	}
	return result;
}

template <typename ForwardIterator, typename Pred>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Pred pred) {
	first = find_if(first, last, pred);
	if(first == last) {
		return last;
	}
	ForwardIterator tmp = first;
	for(++tmp;tmp != last;++tmp) {
		if(!pred(*tmp)) {
			*first++ = std::move(*tmp);
		}
	}
	return first;
}

template <typename ForwardIterator, typename OutputIterator, typename T>
ForwardIterator remove_copy(ForwardIterator first, ForwardIterator last,
	OutputIterator result, const T &value) {
	for(;first != last;++first) {
		if(*first != value) {
			*result++ = *first;
		}
	}
	return result;
}

template <typename ForwardIterator, typename T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T &value) {
	first = find(first, last, value);
	if(first == last) {
		return last;
	}
	ForwardIterator tmp = first;
	for(++tmp;tmp != last;++tmp) {
		if(*tmp != value) {
			*first++ = std::move(*tmp);
		}
	}
	return first;
}

template <typename ForwardIterator, typename T>
void replace(ForwardIterator first, ForwardIterator last,
	const T &old_value, const T &new_value) {
	for(;first != last;++first) {
		if(*first == old_value) {
			*first = new_value;
		}
	}
}

template <typename ForwardIterator, typename OutputIterator, typename T>
OutputIterator replace_copy(ForwardIterator first, ForwardIterator last, OutputIterator result,
	const T &old_value, const T &new_value) {
	for(;first != last;++first) {
		*result++ = *first == old_value ? new_value : *first;
	}
	return result;
}

template <typename ForwardIterator, typename Pred, typename T>
void replace_if(ForwardIterator first, ForwardIterator last,
	Pred pred, const T &new_value) {
	for(;first != last;++first) {
		if(pred(*first)) {
			*first = new_value;
		}
	}
}

template <typename ForwardIterator, typename OutputIterator, typename Pred, typename T>
OutputIterator replace_copy_if(ForwardIterator first, ForwardIterator last, OutputIterator result,
	Pred pred, const T &new_value) {
	for(;first != last;++first) {
		*result++ = pred(*first) ? new_value : *first;
	}
	return result;
}

template <typename BidirectionalIterator>
static void __reserve(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag) {
	while(first != last && first != (--last)) {
		iter_swap(first++, last);
	}
}

template <typename RandomAccessIterator>
static void __reserve(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
	auto p = distance(first, last) >> 1;
	while(p--) {
		iter_swap(first++, --last);
	}
}

template <typename BidirectionalIterator>
void reverse(BidirectionalIterator first, BidirectionalIterator last) {
	__reserve(first, last, iterator_category(first));
}

template <typename BidirectionalIterator, typename OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last,
	OutputIterator result) {
	while(first != last) {
		*result++ = *(--last);
	}
	return result;
}

template <typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2) {
	for(;first1 != last1;++first1, ++first2) {
		iter_swap(first1, first2);
	}
	return first2;
}

template <typename ForwardIterator>
static void __rotate(ForwardIterator first, ForwardIterator middle,
	ForwardIterator last, forward_iterator_tag) {
	ForwardIterator i = middle;
	while(true) {
		iter_swap(first++, i++);
		if(first == middle) {
			if(i == last) {
				break;
			}
			middle = i;
		} else if(i == last) {
			i = middle;
		}
	}
}

template <typename BidirectionalIterator>
static void __rotate(BidirectionalIterator first, BidirectionalIterator middle,
	BidirectionalIterator last, bidirectional_iterator_tag) {
	__reserve(first, middle, bidirectional_iterator_tag());
	__reserve(middle, last, bidirectional_iterator_tag());
	__reserve(first, last, bidirectional_iterator_tag());
}

template <typename RandomAccessIterator>
static void __rotate(RandomAccessIterator first, RandomAccessIterator middle,
	RandomAccessIterator last, random_access_iterator_tag) {
	__reserve(first, middle, random_access_iterator_tag());
	__reserve(middle, last, random_access_iterator_tag());
	__reserve(first, last, random_access_iterator_tag());
}

template <typename ForwardIterator>
void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
	if(first == middle || middle == last) {
		return;
	}
	return __rotate(first, middle, last, iterator_category(first));
}

template <typename ForwardIterator, typename OutputIterator>
OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle,
	ForwardIterator last, OutputIterator result) {
	return copy(first, middle, copy(middle, last, result));
}

template <typename InputIterator1, typename InputIterator2, typename Compare>
InputIterator1 search(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, Compare comp) {
	auto len1 = distance(first1, last1);
	auto len2 = distance(first2, last2);

	if(len2 <= 0) {
		return first1;
	}

	bool _is_same = false;
	for(;first1 != last1;++first1, --len1) {
		if(len2 > len1) {
			return last1;
		}
		bool _is_same = true;
		for(auto p = first2, q = first1;_is_same && p != last2;++p, ++q) {
			_is_same = comp(*p, *q);
		}
		if(_is_same) {
			break;
		}
	}
	return first1;
}

template <typename InputIterator1, typename InputIterator2>
InputIterator1 search(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2) {
	using T = typename iterator_traits<InputIterator1>::value_type;
	return search(first1, last1, first2, last2, equal_to<T>());
}

template <typename InputIterator, typename Size, typename T, typename Compare>
InputIterator search_n(InputIterator first, InputIterator last, Size size, const T &value, Compare comp) {
	if(size <= 0) {
		std::cout << __FILE__ << ':' << __LINE__ << std::endl;
		return first;
	}

	auto len = distance(first, last);

	for(;first != last;) {
		if(size > len) {
			return last;
		}
		bool _is_same = true;
		auto tmp = size;
		auto p = first;
		while(_is_same && tmp--) {
			_is_same = comp(*p++, value);
		}
		if(_is_same) {
			break;
		} else {
			len -= size - tmp;
			first = p;
		}
	}
	return first;
}

template <typename InputIterator, typename Size, typename T>
InputIterator search_n(InputIterator first, InputIterator last, Size size, const T &value) {
	return search_n(first, last, size, value, equal_to<T>());
}

template <typename InputIterator, typename OutputIterator, typename UP>
OutputIterator tranform(InputIterator first, InputIterator last, OutputIterator result, UP up) {
	while(first != last) {
		*result++ = up(*first++);
	}
	return result;
}

template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BP>
OutputIterator tranform(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, OutputIterator result, BP bp) {
	while(first1 != last1) {
		*result++ = bp(*first1++, *first2++);
	}
	return result;
}

template <typename InputIterator, typename OutputIterator, typename Compare>
static OutputIterator __unique_copy(InputIterator first, InputIterator last,
	OutputIterator result, Compare comp, forward_iterator_tag) {
	*result = *first;
	while(++first != last) {
		if(!comp(*result, *first)) {
			*(++result) = *first;
		}
	}
	return ++result;
}

template <typename InputIterator, typename OutputIterator, typename Compare>
static OutputIterator __unique_copy(InputIterator first, InputIterator last,
	OutputIterator result, Compare comp, output_iterator_tag) {
	using T = typename iterator_traits<OutputIterator>::value_type;
	T tmp = *first;
	while(++first != last) {
		if(!comp(tmp, *first)) {
			tmp = *first;
			*(++result) = tmp;
		}
	}
	return ++result;
}

template <typename InputIterator, typename OutputIterator, typename Compare>
OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result, Compare comp) {
	if(first == last) {
		return result;
	}
	return __unique_copy(first, last, result, comp, iterator_category(result));
}

template <typename InputIterator, typename OutputIterator>
OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result) {
	using T = typename iterator_traits<InputIterator>::value_type;
	return unique_copy(first, last, result, equal_to<T>());
}

template <typename InputIterator, typename Compare>
InputIterator unique(InputIterator first, InputIterator last, Compare comp) {
	first = adjacent_find(first, last, comp);
	return unique_copy(first, last, first, comp);
}

template <typename InputIterator>
InputIterator unique(InputIterator first, InputIterator last) {
	using T = typename iterator_traits<InputIterator>::value_type;
	return unique(first, last, equal_to<T>());
}

template <typename InputIterator, typename T, typename Compare = less<T>>
InputIterator lower_bound(InputIterator first, InputIterator last,
	const T &value, Compare comp = Compare()) {
	auto len = distance(first, last);
	auto half = len >> 1;

	InputIterator middle;

	while(len > 0) {
		half = len >> 1;
		middle = first;
		advance(middle, half);

		if(comp(*middle, value)) {
			len = len - half - 1;
			first = ++middle;
		} else {
			len = half;
		}
	}
	return first;
}

template <typename InputIterator, typename T, typename Compare = less<T>>
InputIterator upper_bound(InputIterator first, InputIterator last,
	const T &value, Compare comp = Compare()) {
	auto len = distance(first, last);
	auto half = len >> 1;

	InputIterator middle;

	while(len > 0) {
		half = len >> 1;
		middle = first;
		advance(middle, half);

		if(comp(value, *middle)) {
			len = half;
		} else {
			len = len - half - 1;
			first = ++middle;
		}
	}
	return first;
}

template <typename InputIterator, typename T, typename Compare = less<T>>
bool binary_search(InputIterator first, InputIterator last,
	const T &value, Compare comp = Compare()) {
	InputIterator tmp = lower_bound(first, last, value, comp);
	return tmp != last && (!comp(value, *tmp));
}

template <typename BidirectionalIterator, typename Compare>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp) {
	if(first == last) {
		return false;
	}

	auto i = first;
	++i;
	if(i == last) {
		return false;
	}

	i = last;
	--i;

	while(true) {
		auto ii = i;
		--i;

		if(comp(*i, *ii)) {
			auto j = last;
			while(!comp(*i, *(--j)));
			iter_swap(i, j);
			reverse(ii, last);
			return true;
		}

		if(i == first) {
			reverse(first, last);
			return false;
		}
	}
}

template <typename BidirectionalIterator>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last) {
	using T = typename iterator_traits<BidirectionalIterator>::value_type;
	return next_permutation(first, last, less<T>());
}

template <typename BidirectionalIterator, typename Compare>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp) {
	if(first == last) {
		return false;
	}

	auto i = first;
	++i;
	if(i == last) {
		return false;
	}

	i = last;
	--i;

	while(true) {
		auto ii = i;
		--i;

		if(comp(*ii, *i)) {
			auto j = last;
			while(!comp(*(--j), *i));
			iter_swap(i, j);
			reverse(ii, last);
			return true;
		}

		if(i == first) {
			reverse(first, last);
			return false;
		}
	}
}

template <typename BidirectionalIterator>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last) {
	using T = typename iterator_traits<BidirectionalIterator>::value_type;
	return prev_permutation(first, last, less<T>());
}

template <typename RandomAccessIterator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last) {
	if(first == last) {
		return;
	}

	for(auto p = first + 1;p != last;++p) {
		iter_swap(p, first + (rand() % ((p - first) + 1)));
	}
}

template <typename RandomAccessIterator, typename Random>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last, Random random_gen) {
	if(first == last) {
		return;
	}

	for(auto p = first + 1;p != last;++p) {
		iter_swap(p, first + (random_gen() % ((p - first) + 1)));
	}
}

template <typename RandomAccessIterator, typename Compare>
void partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
	RandomAccessIterator last, Compare comp) {
	make_heap(first, middle, comp);
	for(auto i = middle;i < last;++i) {
		if(comp(*i, *first)) {
			iter_swap(i, first);
			__adjust_heap(first, middle, comp, first);
		}
	}
	sort_heap(first, middle, comp);
}

template <typename RandomAccessIterator>
void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last) {
	using T = typename iterator_traits<RandomAccessIterator>::value_type;
	return partial_sort(first, middle, last, less<T>());
}

template <typename InputIterator, typename RandomAccessIterator, typename Compare>
RandomAccessIterator partial_sort_copy(InputIterator first1, InputIterator last1,
	RandomAccessIterator first2, RandomAccessIterator last2, Compare comp) {
	auto p = first2;
	for(;p != last2 && first1 != last1;++p, ++first1) {
		*p = *first1;
	}

	make_heap(first2, p, comp);
	for(;first1 != last1;++first1) {
		if(comp(*first1, *first2)) {
			*first2 = *first1;
			__adjust_heap(first2, p, comp, first2);
		}
	}
	sort_heap(first2, p, comp);
	return p;
}

template <typename InputIterator, typename RandomAccessIterator>
RandomAccessIterator partial_sort_copy(InputIterator first1, InputIterator last1,
	RandomAccessIterator first2, RandomAccessIterator last2) {
	using T = typename iterator_traits<RandomAccessIterator>::value_type;
	return partial_sort_copy(first1, last1, first2, last2, less<T>());
}

template <typename BidirectionalIterator1, typename BidirectionalIterator2>
BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
	BidirectionalIterator2 result) {
	while(first != last--) {
		*(--result) = *last;
	}
	return result;
}

template <typename RandomAccessIterator, typename T, typename Compare>
static void __unguarded_linear_insert(RandomAccessIterator last, T &&value, Compare comp) {
	auto next = last;
	--next;

	while(comp(value, *next)) {
		*last = std::move(*next);
		last = next--;
	}
	*last = std::move(value);
}

template <typename RandomAccessIterator, typename Compare>
static void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	if(first == last) {
		return;
	}
	using T = typename iterator_traits<RandomAccessIterator>::value_type;
	for(auto p = first + 1;p != last;++p) {
		T tmp = std::move(*p);
		if(comp(tmp, *first)) {
			for(auto q = p;q != first;--q) {
				*q = std::move(*(q - 1));
			}
			*first = std::move(tmp);
		} else {
			__unguarded_linear_insert(p, std::move(tmp), comp);
		}
	}
}

template <typename T, typename Compare>
static const T &__median(const T &a, const T &b, const T &c, Compare comp) {
	if(comp(a, b)) {
		if(comp(b, c)) {
			return b;
		} else if(comp(a, c)) {
			return c;
		} else {
			return a;
		}
	} else {
		if(comp(a, c)) {
			return a;
		} else if(comp(b, c)) {
			return c;
		} else {
			return b;
		}
	}
}

template <typename RandomAccessIterator, typename T, typename Compare>
static RandomAccessIterator __unguarded_partition(RandomAccessIterator first, RandomAccessIterator last,
	T value, Compare comp) {
	while(true) {
		while(comp(*first, value)) {
			++first;
		}
		--last;
		while(comp(value, *last)) {
			--last;
		}
		if(last - first <= 0) {
			break;
		}
		iter_swap(first, last);
		++first;
	}
	return first;
}

template <typename Size>
static Size __log(Size n) {
	Size k = 0;
	while(n) {
		++k;
		n >>= 1;
	}
	return k;
}

static constexpr uint32_t threshold = 16;

template <typename RandomAccessIterator, typename Size, typename Compare>
static void __introsort(RandomAccessIterator first, RandomAccessIterator last,
	Size depth_limit, Compare comp) {

	// 元素较少，直接插入排序
	if(distance(first, last) < threshold) {
		__insertion_sort(first, last, comp);
	}
	if(depth_limit == 0) {
		// 递归过深，使用堆排序
		partial_sort(first, last, last, comp);
		return;
	}

	// 使用快速排序
	auto cut = __unguarded_partition(first, last,
		__median(*first, *(first + ((last - first) >> 1)), *(last - 1), comp), comp);

	// 递归处理
	__introsort(first, cut, depth_limit - 1, comp);
	__introsort(cut, last, depth_limit - 1, comp);
}

template <typename RandomAccessIterator, typename Compare>
void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	if(first != last) {
		__introsort(first, last, __log(last - first) << 1, comp);
		// // 对各个子序列进行排序
		// __final_insertion_sort(first, last, comp);
	}
}

template <typename RandomAccessIterator>
void sort(RandomAccessIterator first, RandomAccessIterator last) {
	using T = typename iterator_traits<RandomAccessIterator>::value_type;
	return sort(first, last, less<T>());
}

template <typename InputIterator, typename T, typename Compare = less<T>>
Pair<InputIterator, InputIterator> equal_range(InputIterator first, InputIterator last,
	const T &value, Compare comp = Compare()) {
	return Pair<InputIterator, InputIterator>(
		lower_bound(first, last, value, comp), upper_bound(first, last, value, comp));
}

template <typename BidirectionalIterator, typename Compare>
void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
	BidirectionalIterator last, Compare comp) {
	if(first == middle || middle == last) {
		return;
	}
	using T = typename iterator_traits<BidirectionalIterator>::value_type;
	auto len = distance(first, middle);
	T tmp[len];
	copy(first, middle, tmp);
	merge(middle, last, tmp, tmp + len, first);
}

template <typename BidirectionalIterator>
void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last) {
	using T = typename iterator_traits<BidirectionalIterator>::value_type;
	inplace_merge(first, middle, last, less<T>());
}

template <typename RandomAccessIterator, typename Compare>
void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
	RandomAccessIterator last, Compare comp) {
	while(last - first > 3) {
		auto cut = __unguarded_partition(first, last,
			__median(*first, *(first + ((last - first) >> 1)), *(last - 1), comp), comp);

		if(nth - cut > 0) {
			first = cut;
		} else {
			last = cut;
		}
	}
	__insertion_sort(first, last, comp);
}

template <typename RandomAccessIterator>
void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last) {
	using T = typename iterator_traits<RandomAccessIterator>::value_type;
	return nth_element(first, nth, last, less<T>());
}

template <typename RandomAccessIterator, typename Compare>
void mergesort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	auto n = distance(first, last);
	if(n <= 1) {
		return;
	}
	auto middle = first;
	advance(middle, n >> 1);
	mergesort(first, middle, comp);
	mergesort(middle, last, comp);
	inplace_merge(first, middle, last);
}

template <typename RandomAccessIterator>
void mergesort(RandomAccessIterator first, RandomAccessIterator last) {
	using T = typename iterator_traits<RandomAccessIterator>::value_type;
	return mergesort(first, last, less<T>());
}



} // namespace stl

#endif // _ALGO_HPP__