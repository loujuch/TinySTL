#ifndef _VECTOR_HPP__
#define _VECTOR_HPP__

#include "allocator.hpp"
#include "iterator.hpp"
#include "uninitialized.hpp"

#include <iostream>

namespace stl {

template <typename T, typename ALLOCATOR = Allocator<T>>
class Vector {
public:
	using value_type = T;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using reference = value_type &;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	using iterator = pointer;
	using const_iterator = const_pointer;
private:
	ALLOCATOR m_allocator_;

	iterator m_start_;
	iterator m_last_;
	iterator m_end_of_storage_;

	// 获取应分配内存大小
	size_type new_memory(size_type cap) {
		if(cap == 0) {
			return 0;
		}
		size_type ncap = capacity();
		if(ncap == 0) {
			return cap;
		}
		while(ncap < cap) {
			ncap <<= 1;
		}
		return ncap;
	}
public:
	Vector() :
		m_start_(nullptr),
		m_last_(nullptr),
		m_end_of_storage_(nullptr) {
	}

	explicit Vector(size_type n) :
		m_start_(m_allocator_.allocate(n)),
		m_last_(m_start_ + n),
		m_end_of_storage_(m_last_) {
		uninitialized_fill(m_start_, m_last_, m_allocator_);
	}

	explicit Vector(size_type n, const T &x) :
		m_start_(m_allocator_.allocate(n)),
		m_last_(m_start_ + n),
		m_end_of_storage_(m_last_) {
		uninitialized_fill(x, m_start_, m_last_, m_allocator_);
	}

	Vector(const Vector<T, ALLOCATOR> &v) :
		m_start_(m_allocator_.allocate(v.capacity())),
		m_last_(m_start_ + v.size()),
		m_end_of_storage_(m_start_ + v.capacity()) {
		uninitialized_copy(v.m_start_, v.m_last_, m_start_, m_allocator_);
	}

	Vector(Vector<T, ALLOCATOR> &&v) :
		m_start_(v.m_start_),
		m_last_(v.m_last_),
		m_end_of_storage_(v.m_end_of_storage_) {
		v.m_start_ = nullptr;
		v.m_last_ = nullptr;
		v.m_end_of_storage_ = nullptr;
	}

	~Vector() {
		clear();
	}

	inline Vector &operator=(const Vector<T, ALLOCATOR> &v) {
		if(this != &v) {
			clear();

			m_start_ = m_allocator_.allocate(v.capacity());
			m_last_ = m_start_ + v.capacity();
			m_end_of_storage_ = m_last_;

			uninitialized_copy(v.m_start_, v.m_last_, m_start_, m_allocator_);
		}
	}

	inline Vector &operator=(Vector<T, ALLOCATOR> &&v) {
		if(this != &v) {
			clear();

			m_start_ = v.m_start_;
			m_last_ = v.m_last_;
			m_end_of_storage_ = v.m_end_of_storage_;
		}
	}

	// 迭代器相关
	inline iterator begin() {
		return m_start_;
	}

	inline iterator end() {
		return m_last_;
	}

	inline const_iterator begin() const {
		return m_start_;
	}

	inline const_iterator end() const {
		return m_last_;
	}

	// 容量相关
	inline size_type size() const {
		return m_last_ - m_start_;
	}

	inline size_type capacity() const {
		return m_end_of_storage_ - m_start_;
	}

	inline bool empty() const {
		return m_last_ == m_start_;
	}

	inline void clear() {
		if(m_start_) {
			initialized_destory(m_start_, m_last_, m_allocator_);
			m_allocator_.deallocate(m_start_, m_end_of_storage_ - m_start_);

			m_start_ = nullptr;
			m_last_ = nullptr;
			m_end_of_storage_ = nullptr;
		}
	}

	inline void resize(size_type n) {
		if(n == 0) {
			clear();
			return;
		}
		size_type sn = size();
		if(sn > n) {
			initialized_destory(m_start_ + n, m_last_, m_allocator_);
			m_last_ = m_start_ + n;
		} else if(sn < n) {
			size_type cap = capacity();
			if(n <= cap) {
				uninitialized_fill(m_last_, m_start_ + n, m_allocator_);
				m_last_ = m_start_ + n;
			} else {
				size_type ncap = new_memory(n);
				auto p = m_allocator_.allocate(ncap, m_start_);
				if(m_start_ != nullptr) {
					uninitialized_copy(m_start_, m_last_, p, m_allocator_);
				}
				clear();
				m_start_ = p;
				m_last_ = p + sn;
				m_end_of_storage_ = p + ncap;
				uninitialized_fill(m_last_, m_start_ + n, m_allocator_);
				m_last_ = m_start_ + n;
			}
		}
	}

	inline void resize(size_type n, const T &x) {
		if(n == 0) {
			clear();
			return;
		}
		size_type sn = size();
		if(sn > n) {
			initialized_destory(m_start_ + n, m_last_, m_allocator_);
			m_last_ = m_start_ + n;
		} else if(sn < n) {
			size_type cap = capacity();
			if(n <= cap) {
				uninitialized_fill(x, m_last_, m_start_ + n, m_allocator_);
				m_last_ = m_start_ + n;
			} else {
				size_type ncap = new_memory(n);
				auto p = m_allocator_.allocate(ncap, m_start_);
				if(m_start_ != nullptr) {
					uninitialized_copy(m_start_, m_last_, p, m_allocator_);
				}
				clear();
				m_start_ = p;
				m_last_ = p + sn;
				m_end_of_storage_ = p + ncap;
				uninitialized_fill(x, m_last_, m_start_ + n, m_allocator_);
				m_last_ = m_start_ + n;
			}
		}
	}

	// 元素相关
	inline value_type &operator[](size_type n) {
		return m_start_[n];
	}

	inline const value_type &operator[](size_type n) const {
		return m_start_[n];
	}

	inline value_type &at(size_type n) {
		return m_start_[n];
	}

	inline const value_type &at(size_type n) const {
		return m_start_[n];
	}

	// 增删操作
	iterator insert(iterator pos, const value_type &elem) {
		if(m_last_ == m_end_of_storage_) {
			auto cap = capacity() + 1;
			auto ncap = new_memory(cap);
			auto p = m_allocator_.allocate(ncap);
			auto dis = distance(m_start_, pos);
			uninitialized_move(m_start_, pos, p, m_allocator_);
			m_allocator_.construct(p + dis, elem);
			uninitialized_move(pos, m_last_, p + dis + 1);
			clear();
			m_start_ = p;
			m_last_ = p + cap + 1;
			m_end_of_storage_ = p + ncap;
			return m_start_ + dis;
		} else {
			for(auto p = m_last_;p != pos;--p) {
				m_allocator_.construct(p, std::move(*(p - 1)));
			}
			m_allocator_.construct(pos, elem);
			++m_last_;
			return pos;
		}
	}

	iterator insert(iterator pos, size_type n, const value_type &elem) {
		auto need_cap = size() + n;
		if(need_cap > capacity()) {
			auto ncap = new_memory(need_cap);
			auto p = m_allocator_.allocate(ncap);
			auto dis = distance(m_start_, pos);
			uninitialized_move(m_start_, pos, p, m_allocator_);
			for(size_type i = 0;i < n;++i) {
				m_allocator_.construct(p + dis + i, elem);
			}
			uninitialized_move(pos, m_last_, p + dis + n, m_allocator_);
			return m_start_ + dis;
		} else {
		}
		return m_start_;
	}

	template <typename I>
	iterator insert(iterator pos, I first, I last) {
		return m_start_;
	}

	inline void push_back(const value_type &v) {

	}
};

} // namespace stl

#endif // _VECTOR_HPP__