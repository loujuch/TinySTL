#include <iostream>

#include "deque.hpp"

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

	Test(Test &&t) :n(t.n) {
		std::cout << "move ctor: " << n << std::endl;
		t.n = 0;
	}

	int getn() const {
		return n;
	}

	void set(int ns) {
		n = ns;
	}

	void print() const {
		std::cout << "print: " << n << std::endl;
	}

	bool operator==(const Test &t) const {
		return n == t.n;
	}

	bool operator!=(const Test &t) const {
		return !operator==(t);
	}

	// bool operator<(const Test &t) const {
	// 	return n < t.n;
	// }

	~Test() {
		// std::cout << "dtor: " << n << std::endl;
	}
};

void show(const stl::Deque<Test> &t) {
	std::cout << "size: " << t.size() << std::endl;
	for(auto &tt : t) {
		tt.print();
	}
	std::cout << std::endl;
}

void show(const stl::Deque<int> &t) {
	std::cout << "size: " << t.size() << std::endl;
	for(auto &tt : t) {
		std::cout << "print: " << tt << std::endl;
	}
	std::cout << std::endl;
}

#define test_show(t) std::cout<<#t<<":"<<__LINE__<<std::endl;show(t)

void test_fb(const stl::Deque<Test> &t) {
	if(t.empty()) {
		return;
	}
	std::cout << "front: ";
	t.front().print();
	std::cout << "back: ";
	t.back().print();
	std::cout << std::endl;
}

void main_test() {
	stl::Deque<Test> deque0;
	test_show(deque0);
	test_fb(deque0);
	stl::Deque<Test> deque1(10);
	test_show(deque1);
	test_fb(deque1);
	stl::Deque<Test> deque2(10, Test(-1));
	test_show(deque2);
	test_fb(deque2);
	stl::Deque<Test> deque3(deque2.begin() + 2, deque2.end() - 2);
	test_show(deque3);
	test_fb(deque3);
	stl::Deque<Test> deque4(deque1);
	test_show(deque4);
	test_fb(deque4);
	stl::Deque<Test> deque5(std::move(deque1));
	test_show(deque5);
	test_fb(deque5);

	deque0 = deque5;
	test_show(deque0);
	test_fb(deque0);

	deque1 = std::move(deque5);
	test_show(deque1);
	test_fb(deque1);

	for(int i = 0;i < deque0.size();++i) {
		deque0[i].set(i);
	}

	for(int i = 0;i < deque0.size();++i) {
		deque0[i].print();
	}

	test_show(deque0);
	deque0.insert(deque0.begin() + 2, Test(10));
	test_show(deque0);


	test_show(deque0);
	deque0.insert(deque0.end() - 2, Test(-10));
	test_show(deque0);

	deque0.clear();
	test_show(deque0);

	deque0.insert(deque0.begin(), Test(10));
	deque0.insert(deque0.begin(), Test(10));
	test_show(deque0);


	deque0.insert(deque0.begin() + 1, 8, Test(-10));
	test_show(deque0);

	deque0.clear();

	test_show(deque0);
	test_show(deque1);
	deque0.insert(deque0.begin(), deque1.begin() + 1, deque1.end() - 2);
	test_show(deque0);


	test_show(deque0);
	test_show(deque1);
	deque0.insert(deque0.begin() + 2, deque1.begin() + 5, deque1.end() - 2);
	test_show(deque0);

	deque0.clear();

	deque0.push_back(Test(-1));
	test_show(deque0);
	deque0.push_back(Test(-2));
	test_show(deque0);
	deque0.push_front(Test(0));
	test_show(deque0);
	deque0.push_front(Test(1));
	test_show(deque0);

	deque0.insert(deque0.begin() + 2, deque1.begin() + 5, deque1.end() - 2);
	test_show(deque1);
	test_show(deque0);

	deque0.erase(deque0.begin() + 1);
	test_show(deque0);

	deque0.erase(deque0.end() - 2);
	test_show(deque0);

	deque0.erase(deque0.begin() + 1, deque0.end() - 1);
	test_show(deque0);

	deque0.clear();
	deque0.emplace_back(1);
	deque0.emplace_back(2);
	deque0.emplace_back(3);
	test_show(deque0);

	deque0.clear();
	deque0.emplace_front(1);
	deque0.emplace_front(2);
	deque0.emplace_front(3);
	test_show(deque0);
}

int main() {
	main_test();
	return 0;
}