#include <iostream>

#include "queue.hpp"

void main_func() {
	stl::Queue<int> queue0;

	queue0.push(1023);
	std::cout << queue0.front() << ' ' << queue0.back() << std::endl;
	queue0.emplace(1024);
	std::cout << queue0.front() << ' ' << queue0.back() << std::endl;
	queue0.emplace(1025);
	std::cout << queue0.front() << ' ' << queue0.back() << std::endl;

	queue0.push(1026);
	std::cout << queue0.front() << ' ' << queue0.back() << std::endl;
	queue0.pop();
	std::cout << queue0.front() << ' ' << queue0.back() << std::endl;

	queue0.push(1027);
	std::cout << queue0.front() << ' ' << queue0.back() << std::endl;
	queue0.pop();
	std::cout << queue0.front() << ' ' << queue0.back() << std::endl;

	while(!queue0.empty()) {
		std::cout << queue0.front() << ' ' << queue0.back() << std::endl;
		queue0.pop();
	}
}

int main() {
	main_func();
	return 0;
}