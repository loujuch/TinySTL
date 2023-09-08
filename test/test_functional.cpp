#include <iostream>

#include "functional.hpp"

void main_func() {
	auto bg = stl::not2(stl::less<int>());
	if(bg(2, 1)) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}
}

int main() {
	main_func();
	return 0;
}