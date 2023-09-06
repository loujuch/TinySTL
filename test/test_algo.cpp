#include <iostream>

#include "algo.hpp"
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
}

int main() {
	main_func();
	return 0;
}