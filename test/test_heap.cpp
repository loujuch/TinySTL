#include <iostream>

#include "alogrithm.hpp"

void main_func() {
	int a[1024] = { 0 };

	for(int i = 0;i < 10;++i) {
		a[i] = i;
	}

	stl::make_heap(a, a + 10);

	for(int i = 0;i < 10;++i) {
		std::cout << a[i] << ' ';
	}
	std::cout << std::endl;

	if(stl::is_heap(a, a + 10)) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	if(stl::is_heap_until(a, a + 10) == a + 10) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	std::cout << std::endl;

	stl::sort_heap(a, a + 10);

	for(int i = 0;i < 10;++i) {
		std::cout << a[i] << ' ';
	}
	std::cout << std::endl;

	if(stl::is_heap(a, a + 10)) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	if(stl::is_heap_until(a, a + 10) == a + 10) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	std::cout << std::endl;

	a[10] = 8;

	stl::make_heap(a, a + 10);

	for(int i = 0;i < 11;++i) {
		std::cout << a[i] << ' ';
	}
	std::cout << std::endl;

	if(stl::is_heap(a, a + 10)) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	if(stl::is_heap_until(a, a + 10) == a + 10) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	std::cout << std::endl;


	stl::push_heap(a, a + 11);

	for(int i = 0;i < 11;++i) {
		std::cout << a[i] << ' ';
	}
	std::cout << std::endl;

	if(stl::is_heap(a, a + 11)) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	if(stl::is_heap_until(a, a + 11) == a + 11) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	std::cout << std::endl;

	stl::pop_heap(a, a + 11);

	for(int i = 0;i < 11;++i) {
		std::cout << a[i] << ' ';
	}
	std::cout << std::endl;
}

int main() {
	main_func();
	return 0;
}