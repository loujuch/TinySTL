#include <iostream>

#include "vector.hpp"

class Test {
	int n;
public:
	Test() :n(0) {
		std::cout << "default ctor: " << n << std::endl;
	}

	explicit Test(int m) :n(m) {
		std::cout << "arg ctor: " << n << std::endl;
	}

	Test(const Test &t) :n(t.n) {
		std::cout << "copy ctor: " << n << std::endl;
	}

	void set(int ns) {
		n = ns;
	}

	void print() {
		std::cout << "print: " << n << std::endl;
	}

	~Test() {
		std::cout << "dtor: " << n << std::endl;
	}
};

int main() {
	stl::Vector<Test> test(10);
	std::cout << test.size() << ' ' << test.capacity() << std::endl;
	test.resize(5, Test());
	std::cout << test.size() << ' ' << test.capacity() << std::endl;
	for(int i = 0;i < test.size();++i) {
		test[i].set(i);
	}
	stl::Vector<Test> test1(test);
	for(auto &t : test1) {
		t.print();
	}
	stl::Vector<Test> test2(std::move(test));
	test2.insert(test2.begin(), Test(-1));
	test2.insert(test2.begin(), Test(-2));
	test2.insert(test2.begin() + 4, Test(-3));
	test2.insert(test2.end(), Test(-4));
	for(auto &t : test2) {
		t.print();
	}
	std::cout << "size: " << test.size() << " cap: " << test.capacity() << std::endl;
	std::cout << "size: " << test1.size() << " cap: " << test1.capacity() << std::endl;
	std::cout << "size: " << test2.size() << " cap: " << test2.capacity() << std::endl;
	test.insert(test.begin(), Test(1));
	return 0;
}