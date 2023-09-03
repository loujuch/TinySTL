#include <iostream>

#include "stack.hpp"

void main_func() {
	stl::Stack<int> stack0;

	stack0.emplace(1);
	stack0.emplace(2);

	std::cout << stack0.top() << std::endl;
	std::cout << stack0.top() << std::endl;
	stack0.pop();

	stack0.push(3);

	while(!stack0.empty()) {
		std::cout << stack0.top() << std::endl;
		stack0.pop();
	}
}

int main() {
	main_func();
	return 0;
}