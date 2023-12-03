#include "hashtable.hpp"

#include <iostream>

#include "vector.hpp"
#include "algorithm.hpp"
#include "functional.hpp"

class Hash {
public:
	size_t operator()(int i) {
		return i;
	}
};

class H2 {
public:
	size_t operator()(size_t hash, size_t n) {
		return hash % n;
	}
};

int main() {
	stl::HashTable<int, int, stl::Identity<int>, Hash, H2, stl::equal_to<int>> tmp;

	tmp.emplace_equal(54);
	auto p = tmp.find(54);
	if(p == tmp.end()) {
		std::cout << __LINE__ << ' ' << "Not Found!" << std::endl;
	} else {
		std::cout << __LINE__ << ' ' << "Found!" << std::endl;
	}

	for(auto n : tmp) {
		std::cout << n << ' ';
	}
	std::cout << std::endl;

	p = tmp.find(0);
	if(p == tmp.end()) {
		std::cout << __LINE__ << ' ' << "Not Found!" << std::endl;
	} else {
		std::cout << __LINE__ << ' ' << "Found!" << std::endl;
	}
	tmp.emplace_equal(0);
	p = tmp.find(0);
	if(p == tmp.end()) {
		std::cout << __LINE__ << ' ' << "Not Found!" << std::endl;
	} else {
		std::cout << __LINE__ << ' ' << "Found!" << std::endl;
	}

	for(auto n : tmp) {
		std::cout << n << ' ';
	}
	std::cout << std::endl;

	tmp.emplace_equal(1);
	p = tmp.find(1);
	if(p == tmp.end()) {
		std::cout << __LINE__ << ' ' << "Not Found!" << std::endl;
	} else {
		std::cout << __LINE__ << ' ' << "Found!" << std::endl;
	}

	for(auto n : tmp) {
		std::cout << n << ' ';
	}
	std::cout << std::endl;

	p = tmp.find(1023);
	if(p == tmp.end()) {
		std::cout << __LINE__ << ' ' << "Not Found!" << std::endl;
	} else {
		std::cout << __LINE__ << ' ' << "Found!" << std::endl;
	}
	tmp.emplace_equal(1023);
	p = tmp.find(1023);
	if(p == tmp.end()) {
		std::cout << __LINE__ << ' ' << "Not Found!" << std::endl;
	} else {
		std::cout << __LINE__ << ' ' << "Found!" << std::endl;
	}
	for(auto n : tmp) {
		std::cout << n << ' ';
	}
	std::cout << std::endl;

	for(int i = 0;i < 128;++i) {
		tmp.emplace_unique(i);
	}
	for(int i = 0;i < 8;++i) {
		tmp.emplace_equal(5);
	}
	for(int i = 0;i < 5;++i) {
		tmp.emplace_equal(112);
	}

	auto tmp1 = tmp;

	stl::Vector<int> v;
	for(auto n : tmp1) {
		v.emplace_back(n);
		std::cout << n << ' ';
	}
	std::cout << std::endl;

	std::cout << tmp1.size() << std::endl;

	stl::sort(v.begin(), v.end());
	for(int i = 1;i < v.size();++i) {
		if(v[i - 1] + 1 != v[i]) {
			std::cout << v[i - 1] << ' ' << v[i] << std::endl;
		}
	}
	for(int i : v) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;
	std::cout << std::endl;

	v.clear();

	std::cout << tmp1.count(6) << std::endl;
	std::cout << tmp1.erase(6) << std::endl;
	std::cout << tmp1.count(6) << std::endl;
	std::cout << tmp1.count(5) << std::endl;
	std::cout << tmp1.erase(5) << std::endl;
	std::cout << tmp1.count(5) << std::endl;
	std::cout << tmp1.count(112) << std::endl;
	std::cout << tmp1.erase(112) << std::endl;
	std::cout << tmp1.count(112) << std::endl;

	for(auto n : tmp1) {
		v.emplace_back(n);
	}
	for(int i : v) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;

	return 0;
}