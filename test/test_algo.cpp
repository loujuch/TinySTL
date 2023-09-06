#include <iostream>

#include "algo.hpp"
#include "numeric.hpp"
#include "vector.hpp"

template <typename I>
void show(const I first, const I last) {
	std::cout << "size: " << last - first << std::endl;
	stl::for_each(first, last, [](int i) {
		std::cout << i << ' ';
		});
	std::cout << std::endl;
}

void show(const stl::Vector<int> &v) {
	show(v.begin(), v.end());
}
void main_func() {
	int num[8] = { 0,1,2,3,4,4,5,6 };

	stl::Vector<int> v(num, num + 8);
	show(v);

	auto p = v.begin();
	p = stl::adjacent_find(v.begin(), v.end());
	show(p, v.end());

	std::cout << stl::count(v.begin(), v.end(), 4) << std::endl;
	std::cout << stl::count_if(v.begin(), v.end(), [](int n) { return n == 4; }) << std::endl;

	p = stl::find(v.begin(), v.end(), 4);
	show(p, v.end());
	p = stl::find_if(v.begin(), v.end(), [](int n) { return n > 4; });
	show(p, v.end());

	p = stl::find_end(v.begin(), v.end(), num + 3, num + 6);
	show(p, v.end());

	p = stl::find_first_of(v.begin(), v.end(), num + 3, num + 6);
	show(p, v.end());

	stl::generate(v.begin() + 1, v.end() - 1, []() {return -1;});
	show(v);

	stl::generate_n(v.begin() + 2, 3, []() {return -2;});
	show(v);

	std::cout << *stl::max_element(v.begin(), v.end()) << std::endl;
	std::cout << *stl::min_element(v.begin(), v.end()) << std::endl;

	int num1[8] = { -1,1,3,5,7,9,11,15 };
	int num2[7] = { -2,1,3,4,5,7,19 };

	stl::Vector<int> tmp(24);
	p = stl::merge(num1, num1 + 8, num2, num2 + 7, tmp.begin());
	show(tmp.begin(), p);

	auto q = stl::partition(tmp.begin(), p, [](int n) {
		return n % 2 != 0;
		});
	show(tmp.begin(), q);
	show(q, p);

	q = stl::remove(tmp.begin(), p, 3);
	show(tmp.begin(), q);
	show(q, p);

	stl::replace_if(tmp.begin(), p, [](int i) {return i == 3;}, 2);
	show(tmp.begin(), p);

	show(tmp.begin(), p);
	stl::reverse(tmp.begin(), p);
	show(tmp.begin(), p);

	stl::Vector<int> ttmp(tmp.size());
	q = stl::reverse_copy(tmp.begin(), p, ttmp.begin());
	show(tmp.begin(), p);
	show(ttmp.begin(), q);

	q = stl::rotate_copy(tmp.begin(), tmp.begin() + 5, p, ttmp.begin());
	show(ttmp.begin(), q);

	stl::rotate(tmp.begin(), tmp.begin() + 5, p);
	show(tmp.begin(), p);

	stl::Vector<int> test0(num1, num1 + 8);
	p = stl::search(test0.begin(), test0.end(), num1 + 4, num1 + 6);
	show(test0.begin(), p);
	show(p, test0.end());

	num1[4] = 1024;
	if(stl::search(test0.begin(), test0.end(), num1 + 3, num1 + 5) == test0.end()) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	p = stl::search_n(test0.begin(), test0.end(), 1, 3);
	show(test0.begin(), p);
	show(p, test0.end());

	p = stl::search_n(test0.begin(), test0.end(), 2, 3);
	show(test0.begin(), p);
	show(p, test0.end());

	test0[1] = 3;
	p = stl::search_n(test0.begin(), test0.end(), 2, 3);
	show(test0.begin(), p);
	show(p, test0.end());

	stl::tranform(test0.begin(), test0.end(), test0.begin(), [](int i) {return i * 2;});
	show(test0);

	show(num1, num1 + 8);
	stl::tranform(test0.begin(), test0.end(), num1, test0.begin(), [](int i, int j) {return i + j;});
	show(test0);

	test0[0] = 0;
	test0[1] = 0;

	test0[4] = 10;
	test0[5] = 10;

	p = stl::unique(test0.begin(), test0.end());
	show(test0.begin(), p);
	show(p, test0.end());

	stl::iota(test0.begin(), test0.end(), 0);
	stl::tranform(test0.begin(), test0.end(), test0.begin(), [](int i) {return i * 2;});
	show(test0);

	p = stl::lower_bound(test0.begin(), test0.end(), 3);
	show(test0.begin(), p);
	show(p, test0.end());
	p = stl::lower_bound(test0.begin(), test0.end(), 4);
	show(test0.begin(), p);
	show(p, test0.end());

	p = stl::upper_bound(test0.begin(), test0.end(), 3);
	show(test0.begin(), p);
	show(p, test0.end());
	p = stl::upper_bound(test0.begin(), test0.end(), 4);
	show(test0.begin(), p);
	show(p, test0.end());

	if(stl::binary_search(test0.begin(), test0.end(), 3)) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	if(stl::binary_search(test0.begin(), test0.end(), 4)) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	stl::next_permutation(test0.begin(), test0.end());
	show(test0);

	stl::prev_permutation(test0.begin(), test0.end());
	show(test0);

	stl::random_shuffle(test0.begin(), test0.end());
	show(test0);

	stl::partial_sort(test0.begin(), test0.begin() + 4, test0.end());
	show(test0);
	stl::random_shuffle(test0.begin(), test0.end());
	show(test0);

	stl::Vector<int> test1(test0.size() - 3);
	stl::partial_sort_copy(test0.begin(), test0.end(), test1.begin(), test1.end());
	show(test1);

	test1.resize(test0.size());
	stl::random_shuffle(test0.begin(), test0.end());
	show(test0);

	stl::sort(test0.begin(), test0.end());
	show(test0);

	stl::random_shuffle(test0.begin(), test0.end());
	show(test0);

	stl::sort(test0.begin(), test0.begin() + 3);
	stl::sort(test0.begin() + 3, test0.end());
	show(test0);

	stl::inplace_merge(test0.begin(), test0.begin() + 3, test0.end());
	show(test0);

	stl::random_shuffle(test0.begin(), test0.end());
	show(test0);

	stl::nth_element(test0.begin(), test0.begin() + 4, test0.end());
	show(test0);

	stl::random_shuffle(test0.begin(), test0.end());
	show(test0);

	stl::mergesort(test0.begin(), test0.end());
	show(test0);
}

int main() {
	main_func();
	return 0;
}