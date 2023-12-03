#include "unordered_set.hpp"

#include "vector.hpp"
#include "algorithm.hpp"

int main() {
	stl::UnorderedSet<int> us0;
	for(int i = 0;i < 1024;++i) {
		us0.emplace(i);
	}
	for(int i = 1000;i < 1024;++i) {
		us0.emplace(i);
	}

	stl::Vector<int> v;

	for(int i : us0) {
		v.emplace_back(i);
	}

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
	std::cout << us0.size() << std::endl;

	v.clear();

	for(int i = 32;i < 1096;++i) {
		us0.erase(i);
	}
	for(int i : us0) {
		v.emplace_back(i);
	}

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
	std::cout << us0.size() << std::endl;
	return 0;
}