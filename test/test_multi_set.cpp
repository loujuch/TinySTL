#include <iostream>

#include "set.hpp"

void show(const stl::MultiSet<int> &set_) {
	std::cout << "size: " << set_.size() << std::endl;
	for(auto &p : set_) {
		std::cout << p << ' ';
	}
	std::cout << std::endl << std::endl;
}

void main_func() {
	stl::MultiSet<int> set0;

	set0.emplace();
	show(set0);
	set0.insert(1);
	show(set0);
	set0.insert(2);
	show(set0);
	set0.insert(1);
	show(set0);
	set0.insert(-1);
	show(set0);
	set0.insert(2);
	show(set0);

	int n = -1;
	set0.insert(n);
	show(set0);

	set0.erase(2);
	show(set0);

	std::cout << set0.count(0) << std::endl;
	std::cout << set0.count(3) << std::endl;

	auto set1 = set0;
	show(set1);
	auto set2 = std::move(set0);
	show(set2);

	set2.insert(1120201453);
	show(set2);

	set1 = set2;
	show(set1);

	set2.insert(1120201454);
	show(set2);

	set1 = std::move(set2);
	show(set1);
	show(set2);

	std::cout << set1.count(1) << std::endl;

	show(set1);
	for(auto i = set1.rbegin();i != set1.rend();++i) {
		std::cout << *i << ' ';
	}
	std::cout << std::endl;
}

int main() {
	main_func();
	return 0;
}