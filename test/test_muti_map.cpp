#include <iostream>

#include "map.hpp"

void show(const stl::MultiMap<int, int> &map_) {
	std::cout << "size: " << map_.size() << std::endl;
	for(auto &p : map_) {
		std::cout << '[' << p.first << ' ' << p.second << ']' << ' ';
	}
	std::cout << std::endl << std::endl;
}

void main_func() {
	stl::MultiMap<int, int> map0;
	show(map0);

	map0.insert(stl::Pair<int, int>(1, 2));
	show(map0);
	map0.insert(stl::Pair<int, int>(1, 2));
	show(map0);
	map0.insert(stl::Pair<int, int>(-1, 2));
	show(map0);
	map0.insert(stl::Pair<int, int>(5, 2));
	show(map0);
	map0.insert(stl::Pair<int, int>(3, 2));
	show(map0);

	std::cout << "count: " << map0.count(-1) << std::endl;

	map0.erase(-1);
	show(map0);
	map0.erase(10);
	show(map0);

	std::cout << "count: " << map0.count(1) << std::endl;
	std::cout << "count: " << map0.count(-1) << std::endl;

	auto map1 = map0;
	show(map1);


	auto map2 = std::move(map0);
	show(map2);

	map2.erase(1);
	show(map2);
}

int main() {
	main_func();
	return 0;
}