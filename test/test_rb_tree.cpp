#include <iostream>

#include "rb_tree.hpp"

#include <set>

void show(const stl::RBTree<int> &rb) {
	std::cout << "size: " << rb.size() << std::endl;
	for(auto i : rb) {
		std::cout << i << ' ';
	}
	std::cout << std::endl << std::endl;
}

void main_func() {
	stl::RBTree<int> rb;
	show(rb);

	rb.insert_unique(1);
	rb.insert_unique(1);

	show(rb);
	rb.insert_unique(3);

	show(rb);
	rb.insert_unique(2);

	show(rb);
	rb.insert_unique(5);

	show(rb);
	rb.insert_unique(-1);

	show(rb);
	rb.insert_unique(10);
	show(rb);

	for(int i = 0;i < 1024;++i) {
		rb.insert_unique(i);
	}
	for(int i = 0;i > -1024;--i) {
		rb.insert_unique(i);
	}
	show(rb);

	for(int i = -1020;i < 1020;++i) {
		rb.erase_unique(i);
		// show(rb);
	}
	show(rb);
}

struct get_key :public stl::UnaryFunction<stl::Pair<int, int>, int> {
	int operator()(const stl::Pair<int, int> &p) const {
		return p.first;
	}
};

void show2(const stl::RBTree<int, stl::Pair<int, int>, get_key> &rb) {
	std::cout << "size: " << rb.size() << std::endl;
	for(auto i : rb) {
		std::cout << '[' << i.first << ' ' << i.second << ']' << ' ';
	}
	std::cout << std::endl << std::endl;
}

void main_func2() {
	stl::Pair<int, int> ptr(0, 1);

	stl::RBTree<int, stl::Pair<int, int>, get_key> rb;

	rb.insert_unique(ptr);
	show2(rb);

	ptr.first = -1;

	rb.insert_unique(ptr);
	show2(rb);

	ptr.first = 3;

	rb.insert_unique(ptr);
	show2(rb);

	auto p = rb.lower_bound(0);
	std::cout << p->first << ' ' << p->second << std::endl;
	p = rb.upper_bound(0);
	std::cout << p->first << ' ' << p->second << std::endl;
}

void show3(const stl::RBNode<int> *n) {
	if(n == nullptr) {
		std::cout << "null ";
		return;
	}
	std::cout << n->m_value << ' ';
	show3((stl::RBNode<int> *)(n->m_left));
	show3((stl::RBNode<int> *)(n->m_right));
}

void main_func3() {
	stl::RBNode<int> n0(0);
	stl::RBNode<int> n1(1);
	stl::RBNode<int> n2(2);
	stl::RBNode<int> n3(3);
	stl::RBNode<int> n4(4);
	stl::RBNode<int> n5(5);
	stl::RBNode<int> n6(6);

	n0.m_left = &n1;
	n1.m_parent = &n0;

	n0.m_right = &n2;
	n2.m_parent = &n0;

	n1.m_left = &n3;
	n3.m_parent = &n1;

	n1.m_right = &n4;
	n4.m_parent = &n1;

	n2.m_left = &n5;
	n5.m_parent = &n2;

	n2.m_right = &n6;
	n6.m_parent = &n2;

	show3(&n0);
	std::cout << std::endl;

	// n1, n2共用父结点
	n1.swap(n2);

	show3(&n0);
	std::cout << std::endl;

	// n2, n6无关联
	n2.swap(n6);

	show3(&n0);
	std::cout << std::endl;

	// n2为n1右结点
	n1.swap(n2);

	show3(&n0);
	std::cout << std::endl;

	// n1为n2右结点
	n1.swap(n2);

	show3(&n0);
	std::cout << std::endl;

	// n1为n5右结点
	n1.swap(n5);

	show3(&n0);
	std::cout << std::endl;

	// n5为n1右结点
	n1.swap(n5);

	show3(&n0);
	std::cout << std::endl;
}

int main() {
	main_func();
	return 0;
}