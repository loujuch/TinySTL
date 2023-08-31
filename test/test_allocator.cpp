#include <vector>
#include <list>
#include <iostream>

#include "allocator.hpp"

class Test {
public:
	Test() {
		std::cout << "ctor" << std::endl;
	}

	~Test() {
		std::cout << "dtor" << std::endl;
	}
};

void test_list() {
	std::list<Test, stl::Allocator<Test>> testv(10);
	testv.erase(testv.begin());
	for(int i = 0;i < 10;++i) {
		testv.erase(testv.begin());
		testv.emplace_back();
	}
}

void test_vector() {
	std::vector<Test, stl::Allocator<Test>> testv;
	for(int i = 0;i < 10;++i) {
		testv.emplace_back();
	}
}

int main() {
	// test_list();
	test_vector();
	return 0;
}