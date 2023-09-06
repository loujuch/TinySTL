#include <iostream>

#include "algobase.hpp"
#include "numeric.hpp"
#include "vector.hpp"

void main_func() {
	stl::Vector<int> test1(10);

	stl::iota(test1.begin(), test1.end(), 0);

	stl::Vector<int> test2 = test1;

	if(test1.size() == test2.size() && stl::equal(test1.begin(), test1.end(), test2.begin())) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	test2[0] = 1024;

	if(test1.size() == test2.size() && stl::equal(test1.begin(), test1.end(), test2.begin())) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	stl::fill(test1.begin(), test1.end(), -1);
	for(auto p : test1) {
		std::cout << p << ' ';
	}
	std::cout << std::endl;
	stl::fill_n(test1.begin(), test1.size() - 2, -2);
	for(auto p : test1) {
		std::cout << p << ' ';
	}
	std::cout << std::endl;

	for(auto p : test1) {
		std::cout << p << ' ';
	}
	std::cout << std::endl;
	for(auto p : test2) {
		std::cout << p << ' ';
	}
	std::cout << std::endl;

	stl::iter_swap(test1.begin(), test2.begin());

	for(auto p : test1) {
		std::cout << p << ' ';
	}
	std::cout << std::endl;
	for(auto p : test2) {
		std::cout << p << ' ';
	}
	std::cout << std::endl;

	if(stl::lexicographical_compare(test1.begin(), test1.end(), test2.begin(), test2.end())) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}
	stl::swap(test1, test2);
	if(stl::lexicographical_compare(test1.begin(), test1.end(), test2.begin(), test2.end())) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	stl::fill_n(test1.begin(), 3, -1);
	stl::fill_n(test2.begin(), 3, -1);
	auto tmp = stl::mismatch(test1.begin(), test1.end(), test2.begin());
	std::cout << tmp.first - test1.begin() << ' ' << tmp.second - test2.begin() << std::endl;

	int num[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

	stl::Vector<int> test3(num, num + 16);
	for(auto p : test3) {
		std::cout << p << ' ';
	}
	std::cout << std::endl;

	stl::Vector<int> test4(16llu, -1);
	for(auto p : test4) {
		std::cout << p << ' ';
	}
	std::cout << std::endl;

	auto q = stl::copy(test3.begin(), test3.end(), test4.begin());
	std::cout << q - test4.begin() << std::endl;
	for(auto p : test4) {
		std::cout << p << ' ';
	}
	std::cout << std::endl;
}

int main() {
	main_func();
	return 0;
}