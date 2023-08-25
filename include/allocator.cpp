#ifndef _ALLOCATOR_HPP__
#define _ALLOCATOR_HPP__

#include <new>

#include <cstddef>

namespace stl {

template <typename T>
class Allocator {
public:
	using value_type = T;
	using pointer = T *;
	using const_pointer = const T *;
	using reference = T &;
	using const_reference = const T &;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

	template <typename U>
	struct rebind {
		using other = Allocator<U>;
	}; // struct rebind
private:
public:
	Allocator() = default;
	Allocator(const Allocator &) = default;
	template<typename U>
	Allocator(const Allocator<U> &o);
	~Allocator() = default;

	pointer address(reference x) const {
		return static_cast<pointer>(&x);
	}

	const_pointer address(const_reference x) const {
		return static_cast<const_pointer>(&x);
	}

	pointer allocate(size_type n, const void *p = nullptr) {
		std::set_new_handler(nullptr);
	}

	void deallocate(pointer p, size_type n) {
	}

	size_type max_size() const {
	}

	void construct(pointer p, const_reference x) {
	}

	void destory(pointer p) {
	}
}; // class Allocator

} // namespace stl

#endif // _ALLOCATOR_HPP__