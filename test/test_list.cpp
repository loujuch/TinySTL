#include "list.hpp"
#include "vector.hpp"

#include <iostream>

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

	int getn() const {
		return n;
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

void show(const stl::List<Test> &t) {
	std::cout << "size: " << t.size() << std::endl;
	for(auto &tt : t) {
		tt.print();
	}
	std::cout << std::endl;
}

void show(const stl::List<int> &t) {
	std::cout << "size: " << t.size() << std::endl;
	for(auto &tt : t) {
		std::cout << "print: " << tt << std::endl;
	}
	std::cout << std::endl;
}

#define test_show(t) std::cout<<#t<<":"<<__LINE__<<std::endl;show(t)

void test_fb(const stl::List<Test> &t) {
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
	stl::List<Test> list0;
	test_show(list0);
	stl::List<Test> list1(10);
	test_show(list1);
	stl::List<Test> list2(10, Test(10));
	test_show(list2);
	stl::List<Test> list3(list2);
	test_show(list3);
	stl::List<Test> list4(list1.begin(), list1.end());
	test_show(list4);
	stl::List<Test> list5(std::move(list2));
	test_show(list5);

	stl::Vector<Test> vector0(5, Test(-2));
	stl::List<Test> list6(vector0.begin() + 1, vector0.end() - 1);
	test_show(list6);

	list0 = list1;
	test_show(list0);
	list1 = std::move(list4);
	test_show(list1);

	test_show(list0);
	test_fb(list0);
	test_show(list1);
	test_fb(list1);
	test_show(list2);
	test_fb(list2);
	test_show(list3);
	test_fb(list3);
	test_show(list4);
	test_fb(list4);
	test_show(list5);
	test_fb(list5);
	test_show(list6);
	test_fb(list6);

	list6.push_back(Test(1120201453));
	list6.push_front(Test(-1120201453));
	test_show(list6);
	test_fb(list6);

	auto p = list6.begin();
	list6.insert(++p, Test(11));
	test_show(list6);
	p = list6.end();
	list6.insert(--p, Test(-11));
	test_show(list6);

	list6.clear();
	list6.push_back(Test(1120201453));
	list6.push_front(Test(-1120201453));
	test_show(list6);
	test_fb(list6);

	list6.insert(list6.begin(), 3, Test(11));
	list6.insert(list6.end(), 3, Test(2020));
	test_show(list6);

	list6.clear();
	list6.push_back(Test(1120201453));
	list6.push_front(Test(-1120201453));
	list6.insert(++list6.begin(), ++++list5.begin(), ----list5.end());
	test_show(list6);

	list6.clear();
	list6.emplace_back(1120201453);
	list6.emplace_front(-1120201453);
	test_show(list6);
	for(int i = 0;i < 3;++i) {
		list6.emplace_back(i);
	}
	test_show(list6);
	for(int i = 0;i < 3;++i) {
		list6.emplace_front(i);
	}
	test_show(list6);

	list6.pop_front();
	list6.pop_back();
	test_show(list6);

	auto p6 = list6.begin(), q6 = list6.end();
	list6.erase(++++p6, --q6);
	test_show(list6);

	auto p7 = list6.begin();
	list6.erase(++p7);
	test_show(list6);

	test_show(list5);
	test_show(list6);
	auto p8 = list6.begin();
	list6.splice(++p8, list5);
	test_show(list5);
	test_show(list6);

	test_show(list3);
	test_show(list6);
	list6.splice(list6.begin(), list3);
	test_show(list3);
	test_show(list6);

	list6.clear();
	test_show(list6);

	list4.emplace_back();
	test_show(list4);
	list4.splice(list4.begin(), stl::List<Test>(3, Test(-22)));
	test_show(list4);

	test_show(list1);
	list4.splice(list4.begin(), list1, ++++list1.begin());
	test_show(list1);
	test_show(list4);

	test_show(list1);
	list4.splice(list4.begin(), list1, ++++list1.begin(), ----list1.end());
	test_show(list1);
	test_show(list4);

	stl::List<int> list7(10);
	auto pp = list7.begin();
	for(int i = 0;i < 10;++i) {
		*pp = i << 1;
		++pp;
	}

	stl::List<int> list8(5);
	auto ppp = list8.begin();
	for(int i = 0;i < 5;++i) {
		*ppp = (i << 1) + 3;
		++ppp;
	}

	test_show(list7);
	test_show(list8);
	list7.merge(list8);
	test_show(list7);
	test_show(list8);

	stl::List<Test> list9(10);
	auto pp9 = list9.begin();
	for(int i = 0;i < 10;++i) {
		(*pp9).set(i << 1);
		++pp9;
	}

	stl::List<Test> list10(5);
	auto ppp9 = list10.begin();
	for(int i = 0;i < 5;++i) {
		(*ppp9).set((i << 1) + 3);
		++ppp9;
	}

	test_show(list9);
	test_show(list10);
	list9.merge(list10, [](const Test &t1, const Test &t2) {
		return t1.getn() < t2.getn();
		});
	test_show(list9);
	test_show(list10);

	test_show(list9);
	list9.remove(Test(9));
	list9.remove(Test(10));
	list9.remove(Test(11));
	test_show(list9);

	test_show(list1);
	list1.remove_if([](const Test &t) {
		return t.getn() == 0;
		});
	test_show(list1);

	for(int i = 0;i < 3;++i) {
		list1.emplace_back(0);
	}
	for(int i = 0;i < 3;++i) {
		list1.emplace_back(i);
	}
	for(int i = 0;i < 3;++i) {
		list1.emplace_back(3);
	}
	list1.emplace_back(-1);

	test_show(list1);
	list1.unique();
	test_show(list1);


	for(int i = 0;i < 3;++i) {
		list1.emplace_back(0);
	}
	for(int i = 0;i < 3;++i) {
		list1.emplace_back(i);
	}
	for(int i = 0;i < 3;++i) {
		list1.emplace_back(3);
	}
	list1.emplace_back(-1);

	test_show(list1);
	list1.unique([](const Test &t1, const Test &t2) {
		return t1.getn() == t2.getn();
		});
	test_show(list1);

	test_show(list1);
	list1.reverse();
	test_show(list1);

	test_show(list1);
	list1.sort([](const Test &t1, const Test &t2) {
		return t1.getn() < t2.getn();
		});
	test_show(list1);
}

int main() {
	main_test();
	Test::print_static();
	return 0;
}