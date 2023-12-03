#include "unordered_map.hpp"

#include <iostream>

int main() {
	stl::UnorderedMap<int, int> um0;
	um0.emplace(1, 1);
	std::cout << um0[1] << std::endl;
	std::cout << um0[-1] << std::endl;
	return 0;
}