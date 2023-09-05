#include <iostream>

#include "numeric.hpp"
#include "vector.hpp"

void main_func() {
	stl::Vector<int> tmp(10);

	stl::iota(tmp.begin(), tmp.end(), 0);

	std::cout << stl::accumulate(tmp.begin(), tmp.end(), 0) << std::endl;

	stl::Vector<int> out(1024);

	auto p = stl::adjacent_difference(tmp.begin(), tmp.end(), out.begin());

	std::cout << (p - out.begin()) << std::endl;
	for(auto q = out.begin();q != p;++q) {
		std::cout << *q << ' ';
	}
	std::cout << std::endl;

	p = stl::partial_sum(tmp.begin(), tmp.end(), out.begin());

	std::cout << (p - out.begin()) << std::endl;
	for(auto q = out.begin();q != p;++q) {
		std::cout << *q << ' ';
	}
	std::cout << std::endl;

	std::cout << stl::inner_product(tmp.begin(), tmp.end(), out.begin(), 0) << std::endl;
}

int main() {
	main_func();
	return 0;
}