#include <iostream>

#include "vector.hpp"
#include "algorithm.hpp"
#include "numeric.hpp"

class Test {
	int n;

	static int create;
	static int destory;
public:
	Test() :n(0) {
		++create;
		// std::cout << "default ctor: " << n << std::endl;
	}

	explicit Test(int m) :n(m) {
		++create;
		// std::cout << "arg ctor: " << n << std::endl;
	}

	Test(const Test &t) :n(t.n) {
		++create;
		// std::cout << "copy ctor: " << n << std::endl;
	}

	Test(Test &&t) :n(t.n) {
		++create;
		t.n = 0;
		// std::cout << "copy ctor: " << n << std::endl;
	}

	void set(int ns) {
		n = ns;
	}

	void print() const {
		std::cout << "print: " << n << std::endl;
	}

	Test &operator=(const Test &t) {
		if(this != &t) {
			n = t.n;
		}
		return *this;
	}

	Test &operator=(Test &&t) {
		if(this != &t) {
			n = t.n;
			t.n = 0;
		}
		return *this;
	}

	bool operator==(const Test &t) const {
		return n == t.n;
	}

	bool operator!=(const Test &t) const {
		return !operator==(t);
	}

	~Test() {
		++destory;
		// std::cout << "dtor: " << n << std::endl;
	}

	static void print_static() {
		std::cout << "create: " << create << " destory: " << destory << std::endl;
	}
};

int Test::create = 0;
int Test::destory = 0;

void show(const stl::Vector<Test> &t) {
	std::cout << "size: " << t.size() << " capacity: " << t.capacity() << std::endl;
	for(auto &tt : t) {
		tt.print();
	}
	std::cout << std::endl;
}

#define test_show(t) std::cout<<#t<<":"<<__LINE__<<std::endl;show(t)

void test_fb(const stl::Vector<Test> &t) {
	std::cout << "front: ";
	t.front().print();
	std::cout << "back: ";
	t.back().print();
	std::cout << std::endl;
}

void main_test() {
	// 构造测试
	stl::Vector<Test> test0;
	test_show(test0);
	stl::Vector<Test> test1(4);
	test_show(test1);
	for(int i = 0;i < test1.size();++i) {
		test1.at(i).set(i);
	}
	stl::Vector<Test> test2(4, Test());
	test_show(test2);
	stl::Vector<Test> test3(test1);
	test_show(test3);
	stl::Vector<Test> test4(std::move(test1));
	test_show(test4);

	stl::Vector<Test> test5(test4.begin() + 1, test4.end() - 1);
	test_show(test5);

	test0 = test4;
	test_show(test0);
	test_show(test4);
	test1 = std::move(test4);
	test_show(test0);
	test_show(test4);

	if(test0 == test1) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	if(test0 != test1) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}

	std::cout << std::endl;

	test_show(test0);
	test_show(test1);
	test_show(test2);
	test_show(test3);
	test_show(test4);

	test2.resize(4);
	test4.resize(4, Test(-1));

	test_show(test0);
	test_show(test1);
	test_show(test2);
	test_show(test3);
	test_show(test4);

	test0.reserve(1024);
	test_show(test0);
	test0.shrink_to_fit();
	test_show(test0);

	test_show(test0);
	test_fb(test0);
	test_show(test1);
	test_fb(test1);
	test_show(test2);
	test_fb(test2);
	test_show(test3);
	test_fb(test3);
	test_show(test4);
	test_fb(test4);

	test0.clear();
	test_show(test0);

	Test test(1120201453);

	test0.insert(test0.begin(), Test(0));
	test_show(test0);
	test0.insert(test0.begin(), Test(0));
	test_show(test0);
	test0.insert(test0.begin(), 10, Test(1));
	test_show(test0);
	test0.insert(test0.begin(), test);
	test_show(test0);
	test0.insert(test0.end(), test1.begin(), test1.end());
	test_show(test0);
	test_show(test1);

	for(int i = 0;i < 4;++i) {
		test0.push_back(test);
		test_show(test0);
	}

	for(int i = 0;i < 5;++i) {
		test1.emplace_back(i + 1024);
		test_show(test1);
	}

	for(int i = 0;i < 10;++i) {
		test0.erase(test0.begin());
		test_show(test0);
	}

	test0.erase(test0.begin() + 2, test0.begin() + 7);
	test_show(test0);

	while(!test1.empty()) {
		test1.pop_back();
		test_show(test1);
	}

	test_show(test0);
	test_show(test1);
	test0.swap(test1);
	test_show(test0);
	test_show(test1);
	stl::swap(test0, test1);
	test_show(test0);
	test_show(test1);

	stl::Vector<int> tmp1(1024);
	stl::iota(tmp1.begin(), tmp1.end(), 0);
	for(int i : tmp1) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;
	for(auto i = tmp1.rbegin(); i != tmp1.rend();++i) {
		std::cout << *i << ' ';
	}
	std::cout << std::endl;
}

int main() {
	main_test();
	Test::print_static();
	return 0;
}