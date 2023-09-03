#include <iostream>

#include "queue.hpp"

void print(const stl::PriorityQueue<int> &pq0) {
	std::cout << pq0.top() << std::endl;
}

void main_func() {
	stl::PriorityQueue<int> pq0;

	pq0.push(1);
	print(pq0);
	pq0.emplace(2);
	print(pq0);
	pq0.emplace(3);
	print(pq0);
	pq0.emplace(-1);
	print(pq0);

	pq0.pop();
	print(pq0);

	pq0.emplace(-2);
	print(pq0);

	std::cout << "size: " << pq0.size() << std::endl;
	while(!pq0.empty()) {
		print(pq0);
		pq0.pop();
	}
}

int main() {
	main_func();
	return 0;
}