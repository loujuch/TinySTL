#ifndef _ALLOCATOR_HPP__
#define _ALLOCATOR_HPP__

/**
 * 一级空间配置器
 * TODO：通过用户自定义宏判断是否开启二级空间配置器
*/

#include <new>

#include <stdint.h>

#ifdef _TINY_STL_MEMORY_POOL_
// 二级空间配置器头文件
#include "alloc.hpp"
#endif // _TINY_STL_MEMORY_POOL_

namespace stl {

template <typename T>
class Allocator {
private:
	template <typename U>
	struct remove_reference {
		using value_type = U;
	};

	template <typename U>
	struct remove_reference<U &> {
		using value_type = U;
	};

	template <typename U>
	struct remove_reference<const U &> {
		using value_type = U;
	};

	template <typename U>
	struct remove_reference<U &&> {
		using value_type = U;
	};
public:
	// 定义类型
	using value_type = typename remove_reference<T>::value_type;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using reference = value_type &;
	using const_reference = const value_type &;
	using size_type = size_t;
	using difference_type = ::ptrdiff_t;

	// 嵌套类型
	template <typename U>
	struct rebind {
		using other = Allocator<U>;
	}; // struct rebind

#ifdef _TINY_STL_MEMORY_POOL_
private:
	// 二级空间配置器实例
	Alloc m_alloc_;
#endif

public:
	// 构造函数
	Allocator() = default;
	Allocator(const Allocator &) = default;
	~Allocator() = default;

	template<typename U>
	Allocator(const Allocator<U> &u) {
	}

	// 获取对象地址
	inline pointer address(reference x) const {
		return static_cast<pointer>(&x);
	}

	inline const_pointer address(const_reference x) const {
		return static_cast<const_pointer>(&x);
	}

	// 分配内存空间
	pointer allocate(size_type n, const void *p = nullptr) {
		std::set_new_handler(nullptr);
		return static_cast<pointer>(::operator new(n * sizeof(value_type)));
	}

	// 释放内存空间
	void deallocate(pointer p, size_type n) {
		::operator delete(p);
	}

	// 对象最大数量
	size_type max_size() const {
		return static_cast<size_type>(UINT32_MAX / sizeof(value_type));
	}

	// 使用对应指针进行构造
	template <typename ... Args>
	void construct(pointer p, Args&& ... x) {
		new(p) value_type(std::forward<Args>(x)...);
	}

	// 使用对应指针进行析构
	void destory(pointer p) {
		p->~value_type();
	}
}; // class Allocator

} // namespace stl

#endif // _ALLOCATOR_HPP__