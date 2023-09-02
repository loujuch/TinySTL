#ifndef _DEQUE_HPP__
#define _DEQUE_HPP__

#include "iterator.hpp"
#include "uninitialized.hpp"
#include "allocator.hpp"

#include <string.h>

namespace stl {

inline size_t tinystl_deque_buf_size(size_t n, size_t st) {
	return (n != 0) ? n : (512 < st ? (1) : (512 / st));
}

template <typename T, size_t BufferSize>
class DequeIterator {
public:
	template <typename U, size_t BS, typename ALLOC>
	friend class Deque;

	using iterator_category = random_access_iterator_tag;
	using value_type = T;
	using difference_type = ::ptrdiff_t;
	using size_type = size_t;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using reference = value_type &;
	using const_reference = const value_type &;

	using map_pointer = pointer *;
	using self = DequeIterator<T, BufferSize>;

	static inline constexpr size_type buffer_size() {
		return tinystl_deque_buf_size(BufferSize, sizeof(T));
	}
private:
	pointer m_first_;
	pointer m_last_;
	pointer m_cur_;

	map_pointer m_map_;

	void set_node(difference_type n) {
		m_map_ += n;

		m_first_ = *m_map_;
		m_last_ = m_first_ + buffer_size();
		m_cur_ = m_first_;
	}

	void clear() {
		m_first_ = nullptr;
		m_last_ = nullptr;
		m_cur_ = nullptr;
		m_map_ = nullptr;
	}
public:
	explicit DequeIterator(pointer cur = nullptr, map_pointer map_ptr = nullptr) :
		m_cur_(cur), m_map_(map_ptr) {
		if(m_map_ != nullptr) {
			m_first_ = *m_map_;
			m_last_ = m_first_ + buffer_size();
		}
	}

	reference operator*() const {
		return *m_cur_;
	}

	pointer operator->() const {
		return m_cur_;
	}

	difference_type operator-(const self &d) const {
		return buffer_size() * (m_map_ - d.m_map_ - 1) +
			(m_cur_ - m_first_) + (d.m_last_ - d.m_cur_);
	}

	bool operator==(const DequeIterator &d) const {
		return d.m_cur_ == m_cur_;
	}

	bool operator!=(const DequeIterator &d) const {
		return !(this->operator==(d));
	}

	self operator+(difference_type n) const {
		self s = *this;
		return (s += n);
	}

	self operator-(difference_type n) const {
		self s = *this;
		return (s -= n);
	}

	self &operator+=(difference_type n) {
		difference_type off = n + (m_cur_ - m_first_);

		if(off >= 0 && off < buffer_size()) {
			m_cur_ += n;
			return *this;
		}

		difference_type node_off = off > 0 ?
			(off / buffer_size()) :
			(-((-off - 1) / buffer_size())) - 1;
		set_node(node_off);
		m_cur_ = m_first_ + (off - node_off * buffer_size());
		return *this;
	}

	self &operator-=(difference_type n) {
		return *this += (-n);
	}


	self &operator++() {
		++m_cur_;
		if(m_cur_ == m_last_) {
			set_node(1);
		}
		return *this;
	}

	self operator++(int) {
		auto out = *this;
		++(*this);
		return out;
	}

	self &operator--() {
		if(m_cur_ == m_first_) {
			set_node(-1);
			m_cur_ = m_last_;
		}
		--m_cur_;
		return *this;
	}

	self operator--(int) {
		auto out = *this;
		--(*this);
		return out;
	}
}; // class DequeIterator

template <typename T, size_t BufferSize = 0, typename ALLOC = Allocator<T>>
class Deque {
public:
	using value_type = T;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using reference = value_type &;
	using const_reference = const value_type &;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	using iterator = DequeIterator<T, BufferSize>;
	using const_iterator = const iterator;

	using map_type = pointer;
	using map_pointer = map_type *;

	using self = Deque<T, BufferSize, ALLOC>;

	static inline constexpr size_type buffer_size() {
		return DequeIterator<T, BufferSize>::buffer_size();
	}
private:
	ALLOC m_buffer_allocator_;
	typename ALLOC::template rebind<map_type>::other m_map_allocator_;

	map_pointer m_map_first_;
	map_pointer m_map_last_;

	iterator m_first_;
	iterator m_last_;

	// 由于尾指针的存在，需要的应该为 n+1 个空间
	size_type map_node_number(size_type n) {
		return (n / buffer_size()) + 1;
	}

	void create_uninit_mem(size_type n) {
		m_map_first_ = m_map_allocator_.allocate(map_node_number(n));
		m_map_last_ = m_map_first_ + map_node_number(n);

		for(map_pointer p = m_map_first_;p != m_map_last_;++p) {
			*p = m_buffer_allocator_.allocate(buffer_size());
		}

		m_first_.m_map_ = m_map_first_;
		m_first_.m_first_ = *m_first_.m_map_;
		m_first_.m_last_ = m_first_.m_first_ + buffer_size();
		m_first_.m_cur_ = m_first_.m_first_;

		m_last_.m_map_ = m_map_last_ - 1;
		m_last_.m_first_ = *m_last_.m_map_;
		m_last_.m_last_ = m_last_.m_first_ + buffer_size();
		m_last_.m_cur_ = m_last_.m_first_ + (n % buffer_size());
	}

	// 前方剩余空间
	size_t free_front() const {
		return buffer_size() * (m_first_.m_map_ - m_map_first_) +
			(m_first_.m_cur_ - m_first_.m_first_);
	}

	// 后方剩余空间
	size_t free_back() const {
		return buffer_size() * (m_map_last_ - 1 - m_last_.m_map_) +
			(m_last_.m_last_ - m_last_.m_cur_);
	}

	// 准备向某方向的内存，dis为true表示向前，否则表示向后
	void ready_memory(size_type add_size, bool dis) {
		if(m_map_first_ == nullptr) {
			create_uninit_mem(add_size);
			if(dis) {
				m_first_ = m_last_;
			} else {
				m_last_ = m_first_;
			}
			return;
		}
		size_type free_mem;
		if(dis) {
			free_mem = free_front();
		} else {
			free_mem = free_back();
		}
		if(free_mem >= add_size) {
			return;
		}
		size_type nn = m_map_last_ - m_map_first_;
		size_type n = map_node_number(add_size - free_mem - 1);

		map_pointer p = m_map_allocator_.allocate(n + nn);

		map_pointer copy_p = p, ctor_p = p;
		if(dis) {
			copy_p += n;
		} else {
			ctor_p += nn;
		}

		for(size_type i = 0;i < n;++i) {
			ctor_p[i] = m_buffer_allocator_.allocate(buffer_size());
		}
		::memcpy(copy_p, m_map_first_, nn * sizeof(map_type));

		m_first_.m_map_ = copy_p + (m_first_.m_map_ - m_map_first_);
		m_last_.m_map_ = copy_p + (m_last_.m_map_ - m_map_first_);

		m_map_allocator_.deallocate(m_map_first_, nn);

		m_map_first_ = p;
		m_map_last_ = m_map_first_ + n + nn;
	}
public:
	Deque() :m_map_first_(nullptr), m_map_last_(nullptr), m_first_(), m_last_() {
	}

	Deque(size_type n) :Deque() {
		if(n == 0) {
			return;
		}
		create_uninit_mem(n);

		auto tmp = n;
		for(map_pointer p = m_map_first_;p != m_map_last_;++p, tmp -= n) {
			uninitialized_fill(*p, (*p) + std::min(buffer_size(), tmp), m_buffer_allocator_);
		}
	}

	Deque(size_type n, const_reference x) :Deque() {
		if(n == 0) {
			return;
		}
		create_uninit_mem(n);

		auto tmp = n;
		for(map_pointer p = m_map_first_;p != m_map_last_;++p, tmp -= n) {
			uninitialized_fill(x, *p, (*p) + std::min(buffer_size(), tmp), m_buffer_allocator_);
		}
	}

	template <typename InputIterator>
	Deque(InputIterator first, InputIterator last) :Deque() {
		auto n = distance(first, last);

		if(n == 0) {
			return;
		}

		create_uninit_mem(n);

		auto p = m_first_;
		for(auto q = first;q != last;++q, ++p) {
			m_buffer_allocator_.construct(p.m_cur_, *q);
		}
	}

	Deque(const self &other) :Deque(other.begin(), other.end()) {
	}

	Deque(self &&other) :
		m_map_first_(other.m_map_first_), m_map_last_(other.m_map_last_),
		m_first_(other.m_first_), m_last_(other.m_last_) {
		other.m_map_first_ = nullptr;
		other.m_map_last_ = nullptr;
		other.m_first_.clear();
		other.m_last_.clear();
	}

	~Deque() {
		clear();
	}

	self &operator=(const self &other) {
		if(this != &other) {
			clear();

			size_type n = other.size();
			if(n > 0) {
				create_uninit_mem(n);

				auto p = m_first_;
				for(auto q = other.m_first_;q != other.m_last_;++q, ++p) {
					m_buffer_allocator_.construct(p.m_cur_, *q);
				}
			}
		}
		return *this;
	}

	self &operator=(self &&other) {
		if(this != &other) {
			clear();

			m_map_first_ = other.m_map_first_;
			m_map_last_ = other.m_map_last_;
			m_first_ = other.m_first_;
			m_last_ = other.m_last_;

			other.m_map_first_ = nullptr;
			other.m_map_last_ = nullptr;
			other.m_first_.clear();
			other.m_last_.clear();
		}
		return *this;
	}

	void swap(self &d) {
		if(this != &d) {
			std::swap(m_first_, d.m_first_);
			std::swap(m_last_, d.m_last_);
			std::swap(m_map_first_, d.m_map_first_);
			std::swap(m_map_last_, d.m_last_);
		}
	}

	ALLOC get_allocator() const {
		return m_buffer_allocator_;
	}

	reference at(size_type n) {
		return *(m_first_ + n);
	}

	const_reference at(size_type n) const {
		return *(m_first_ + n);
	}

	reference operator[](size_type n) {
		return *(m_first_ + n);
	}

	const_reference operator[](size_type n) const {
		return *(m_first_ + n);
	}

	reference front() {
		return *m_first_;
	}

	const_reference front() const {
		return *m_first_;
	}

	reference back() {
		return *(m_last_ - 1);
	}

	const_reference back() const {
		return *(m_last_ - 1);
	}

	iterator begin() {
		return m_first_;
	}

	const_iterator begin() const {
		return m_first_;
	}

	iterator end() {
		return m_last_;
	}

	const_iterator end() const {
		return m_last_;
	}

	bool empty() const {
		return m_first_ == m_last_;
	}

	size_type size() const {
		return m_map_first_ == nullptr ? 0 : m_last_ - m_first_;
	}

	void shrink_to_fit() {
		if(m_first_.m_map_ == m_map_first_ && m_last_.m_map_ + 1 == m_map_last_) {
			return;
		}

		// 释放多余的缓冲区
		for(map_pointer p = m_map_first_;p != m_first_.m_map_;++p) {
			if(*p == nullptr) {
				continue;
			}
			m_map_allocator_.deallocate(*p, buffer_size());
			*p = nullptr;
		}

		for(map_pointer p = m_last_.m_map_ + 1;p != m_map_last_;++p) {
			if(*p == nullptr) {
				continue;
			}
			m_map_allocator_.deallocate(*p, buffer_size());
			*p = nullptr;
		}

		// 分配新缓冲区
		difference_type n = m_last_.m_map_ - m_first_.m_map_ + 1;
		map_pointer p = m_map_allocator_.allocate(n);
		::memcpy(p, m_first_.m_map_, n * sizeof(map_type));
		m_map_allocator_.deallocate(m_map_first_, m_map_last_ - m_map_first_);

		// 分配新迭代器
		m_map_first_ = p;
		m_map_last_ = p + n;
		m_first_.m_map_ = m_map_first_;
		m_last_.m_map_ = m_map_last_ - 1;
	}

	void clear() {
		if(m_map_first_ != nullptr) {

			// 进行析构
			if(m_first_.m_map_ == m_last_.m_map_) {
				initialized_destory(m_first_.m_cur_, m_last_.m_cur_, m_buffer_allocator_);
			} else {
				map_pointer p = m_first_.m_map_ + 1;
				initialized_destory(m_first_.m_cur_, m_first_.m_last_, m_buffer_allocator_);
				initialized_destory(m_last_.m_first_, m_last_.m_cur_, m_buffer_allocator_);
				for(;p != m_last_.m_map_;++p) {
					initialized_destory(*p, *p + buffer_size(), m_buffer_allocator_);
				}
			}
			m_first_.clear();
			m_last_.clear();

			for(map_pointer p = m_map_first_;p != m_map_last_;++p) {
				if(*p == nullptr) {
					continue;
				}
				m_buffer_allocator_.deallocate(*p, buffer_size());
				*p != nullptr;
			}
			m_map_allocator_.deallocate(m_map_first_, m_map_last_ - m_map_first_);
			m_map_first_ = nullptr;
			m_map_last_ = nullptr;
		}
	}

	// 增删操作
	iterator insert(iterator pos, const value_type &elem) {
		return insert(pos, 1, elem);
	}

	iterator insert(iterator pos, value_type &&elem) {
		if(pos.m_map_ == nullptr) {
			*this = std::move(self(1, std::move(elem)));
			return begin();
		}
		difference_type pos_front = pos - begin();
		difference_type pos_back = pos - begin();
		bool dis = pos_front < pos_back;
		ready_memory(1, dis);
		pos = m_first_ + pos_front;
		if(dis) {
			for(auto i = m_first_;i != pos;++i) {
				m_buffer_allocator_.construct((i - 1).m_cur_, std::move(*(i)));
			}
			--m_first_;
			--pos;
		} else {
			for(auto i = m_last_;i != pos;--i) {
				m_buffer_allocator_.construct(i.m_cur_, std::move(*(i - 1)));
			}
			++m_last_;
		}
		m_buffer_allocator_.construct(pos.m_cur_, elem);
		return pos;
	}

	iterator insert(iterator pos, size_type n, const value_type &elem) {
		if(pos.m_map_ == nullptr) {
			*this = std::move(self(n, std::move(elem)));
			return begin();
		}
		difference_type pos_front = pos - begin();
		difference_type pos_back = pos - begin();
		bool dis = pos_front < pos_back;
		ready_memory(n, dis);
		pos = m_first_ + pos_front;
		if(dis) {
			for(auto i = m_first_;i != pos;++i) {
				m_buffer_allocator_.construct((i - n).m_cur_, *(i));
			}
			m_first_ -= n;
			pos -= n;
		} else {
			for(auto i = m_last_;i != pos;--i) {
				m_buffer_allocator_.construct((i + n - 1).m_cur_, *(i - 1));
			}
			m_last_ += n;
		}
		auto p = pos;
		for(size_type i = 0;i < n;++i, ++p) {
			m_buffer_allocator_.construct(p.m_cur_, elem);
		}
		return pos;
	}

	template <typename InputIterator>
	iterator insert(iterator pos, InputIterator first, InputIterator last) {
		difference_type n = distance(first, last);
		if(pos.m_map_ == nullptr) {
			*this = std::move(self(first, last));
			return begin();
		}
		difference_type pos_front = pos - begin();
		difference_type pos_back = pos - begin();
		bool dis = pos_front < pos_back;
		ready_memory(n, dis);
		pos = m_first_ + pos_front;
		if(dis) {
			for(auto i = m_first_;i != pos;++i) {
				m_buffer_allocator_.construct((i - n).m_cur_, *(i));
			}
			m_first_ -= n;
			pos -= n;
		} else {
			for(auto i = m_last_;i != pos;--i) {
				m_buffer_allocator_.construct((i + n - 1).m_cur_, *(i - 1));
			}
			m_last_ += n;
		}
		auto p = pos;
		auto q = first;
		for(size_type i = 0;i < n;++i, ++p, ++q) {
			m_buffer_allocator_.construct(p.m_cur_, *q);
		}
		return pos;
	}

	void push_front(const_reference elem) {
		insert(begin(), elem);
	}

	void push_back(const_reference elem) {
		insert(end(), elem);
	}

	template <typename ... Args>
	void emplace_front(Args&& ... args) {
		if(m_map_first_ == nullptr) {
			create_uninit_mem(1);
		} else {
			ready_memory(1, true);
			--m_first_;
		}
		m_buffer_allocator_.construct(m_first_.m_cur_, std::forward<Args>(args)...);
	}

	template <typename ... Args>
	void emplace_back(Args&& ... args) {
		if(m_map_first_ == nullptr) {
			create_uninit_mem(1);
			m_buffer_allocator_.construct(m_first_.m_cur_, std::forward<Args>(args)...);
			return;
		}
		ready_memory(1, false);
		m_buffer_allocator_.construct(m_last_.m_cur_, std::forward<Args>(args)...);
		++m_last_;
	}

	iterator erase(iterator pos) {
		return erase(pos, pos + 1);
	}

	iterator erase(iterator first, iterator last) {
		difference_type pos_front = first - begin();
		difference_type pos_back = end() - last;
		difference_type n = distance(first, last);
		bool dis = pos_front < pos_back;

		for(auto p = first;p != last;++p) {
			m_buffer_allocator_.destory(p.m_cur_);
		}

		if(dis) {
			for(auto i = first;i != m_first_;--i) {
				m_buffer_allocator_.construct((i + n - 1).m_cur_, std::move(*(i - 1)));
			}
			m_first_ += n;
		} else {
			for(auto i = last;i != m_last_;++i) {
				m_buffer_allocator_.construct((i - n).m_cur_, std::move(*i));
			}
			m_last_ -= n;
		}
		return last;
	}

	void pop_front() {
		erase(begin());
	}

	void pop_back() {
		erase(end() - 1);
	}
}; // class Deque 

} // namespace stl

#endif // _DEQUE_HPP__