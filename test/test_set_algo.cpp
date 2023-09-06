#include <iostream>

#include "vector.hpp"
#include "alogrithm.hpp"

void show(const stl::Vector<int> &v) {
	std::cout << "size: " << v.size() << std::endl;
	for(int i : v) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;
}

void main_func() {
	int a[10] = { 0,1,2,3,4,5,6,7,8,9 };
	int b[12] = { -5,-3,-1,1,3,5,7,9,11,13,15,17 };

	stl::Vector<int> va(a, a + 10);
	show(va);
	stl::Vector<int> vb(b, b + 12);
	show(vb);

	auto p = vb.begin();
	stl::Vector<int> v(24);

	p = stl::set_difference(va.begin(), va.end(), vb.begin(), vb.end(), v.begin());
	std::cout << "size: " << p - v.begin() << std::endl;
	for(auto q = v.begin();q != p;++q) {
		std::cout << *q << ' ';
	}
	std::cout << std::endl;

	p = stl::set_intersection(va.begin(), va.end(), vb.begin(), vb.end(), v.begin());
	std::cout << "size: " << p - v.begin() << std::endl;
	for(auto q = v.begin();q != p;++q) {
		std::cout << *q << ' ';
	}
	std::cout << std::endl;

	p = stl::set_symmetric_difference(va.begin(), va.end(), vb.begin(), vb.end(), v.begin());
	std::cout << "size: " << p - v.begin() << std::endl;
	for(auto q = v.begin();q != p;++q) {
		std::cout << *q << ' ';
	}
	std::cout << std::endl;

	p = stl::set_union(va.begin(), va.end(), vb.begin(), vb.end(), v.begin());
	std::cout << "size: " << p - v.begin() << std::endl;
	for(auto q = v.begin();q != p;++q) {
		std::cout << *q << ' ';
	}
	std::cout << std::endl;

	int ttt[4] = { 1,2,3,4 };
	stl::Vector<int> tmp(ttt, ttt + 4);
	if(stl::include(va.begin(), va.end(), tmp.begin(), tmp.end())) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	if(stl::include(vb.begin(), vb.end(), tmp.begin(), tmp.end())) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}
}

int main() {
	main_func();
	return 0;
}