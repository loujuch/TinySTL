#include "array.hpp"

#include <iostream>

int main() {
	stl::Array<int, 5> test1;
	stl::Array<int, 5> test2;
	for(int i = 0;i < 5;++i) {
		test1.at(i) = i;
		test2.at(i) = 4 - i;
	}
	for(auto i : test1) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;
	for(auto i : test2) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;

	test1.swap(test2);

	for(auto i : test1) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;
	for(auto i : test2) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;
	return 0;
}