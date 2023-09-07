#ifndef _VECTOR_HPP__
#define _VECTOR_HPP__

#include "allocator.hpp"
#include "iterator.hpp"
#include "uninitialized.hpp"
#include "reverse_iterator.hpp"

namespace stl {

template <typename T, typename ALLOCATOR = Allocator<T>>
class Vector {
public:
	using value_type = T;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using reference = value_type &;
	using const_reference = const value_type &;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	using iterator = pointer;
	using const_iterator = const_pointer;
	using reverse_iterator = ReverseIterator<iterator>;
	using const_reverse_iterator = const ReverseIterator<iterator>;
private:
	ALLOCATOR m_allocator_;

	iterator m_start_;
	iterator m_last_;
	iterator m_end_of_storage_;

	// 获取应分配内存大小
	size_type new_memory(size_type cap) {
		if(cap == 0) {
			return 1;
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

	template <typename InputIterator>
	explicit Vector(InputIterator first, InputIterator last) :
		m_start_(nullptr),
		m_last_(nullptr),
		m_end_of_storage_(nullptr) {
		auto n = distance(first, last);

		m_start_ = iterator(m_allocator_.allocate(n));
		m_last_ = m_start_ + n;
		m_end_of_storage_ = m_last_;

		uninitialized_copy(first, last, m_start_, m_allocator_);
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

	inline Vector<T, ALLOCATOR> &operator=(const Vector<T, ALLOCATOR> &v) {
		if(this != &v) {
			clear();

			m_start_ = m_allocator_.allocate(v.capacity());
			m_last_ = m_start_ + v.capacity();
			m_end_of_storage_ = m_last_;

			uninitialized_copy(v.m_start_, v.m_last_, m_start_, m_allocator_);
		}
		return *this;
	}

	inline Vector<T, ALLOCATOR> &operator=(Vector<T, ALLOCATOR> &&v) {
		if(this != &v) {
			clear();

			m_start_ = v.m_start_;
			m_last_ = v.m_last_;
			m_end_of_storage_ = v.m_end_of_storage_;

			v.m_start_ = nullptr;
			v.m_last_ = nullptr;
			v.m_end_of_storage_ = nullptr;
		}
		return *this;
	}

	void swap(Vector<T, ALLOCATOR> &v) {
		if(this != &v) {
			std::swap(m_start_, v.m_start_);
			std::swap(m_last_, v.m_last_);
			std::swap(m_end_of_storage_, v.m_end_of_storage_);
		}
	}

	template <typename U, typename P>
	friend void swap(Vector<U, P> &v, Vector<U, P> &vv);

	//比较操作相关
	bool operator==(const Vector<T, ALLOCATOR> &v) const {
		if(v.size() != size()) {
			return false;
		}
		auto si = v.size();
		for(size_type i = 0;i < si;++i) {
			if(v[i] != at(i)) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const Vector<T, ALLOCATOR> &v) const {
		return !operator==(v);
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
	inline reverse_iterator rbegin() {
		return reverse_iterator(end());
	}

	inline reverse_iterator rend() {
		return reverse_iterator(begin());
	}

	inline const_reverse_iterator rbegin() const {
		return reverse_iterator(end());
	}

	inline const_reverse_iterator rend() const {
		return reverse_iterator(begin());
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
					uninitialized_move(m_start_, m_last_, p, m_allocator_);
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
					uninitialized_move(m_start_, m_last_, p, m_allocator_);
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

	void reserve(size_type n) {
		auto cap = capacity();
		auto si = size();
		if(cap >= n) {
			return;
		}
		auto p = m_allocator_.allocate(n);
		if(m_start_ != nullptr) {
			uninitialized_move(m_start_, m_last_, p, m_allocator_);
		}
		clear();

		m_start_ = p;
		m_last_ = m_start_ + si;
		m_end_of_storage_ = m_start_ + n;
	}

	void shrink_to_fit() {
		auto cap = capacity();
		auto si = size();
		if(cap == si) {
			return;
		}
		auto p = m_allocator_.allocate(si);
		uninitialized_move(m_start_, m_last_, p, m_allocator_);
		clear();

		m_start_ = p;
		m_last_ = m_start_ + si;
		m_end_of_storage_ = m_last_;
	}

	// 元素相关
	inline reference operator[](size_type n) {
		return m_start_[n];
	}

	inline const_reference operator[](size_type n) const {
		return m_start_[n];
	}

	inline reference at(size_type n) {
		return m_start_[n];
	}

	inline const_reference at(size_type n) const {
		return m_start_[n];
	}

	inline reference front() {
		return *begin();
	}

	inline const_reference front() const {
		return *begin();
	}

	inline reference back() {
		return *(end() - 1);
	}

	inline const_reference back() const {
		return *(end() - 1);
	}

	inline pointer data() {
		return m_start_;
	}

	inline const_pointer data() const {
		return m_start_;
	}

	// 增删操作
	iterator insert(iterator pos, const value_type &elem) {
		return insert(pos, (size_type)1, elem);
	}

	iterator insert(iterator pos, size_type n, const value_type &elem) {
		auto need_cap = size() + n;
		if(need_cap > capacity()) {
			auto ncap = new_memory(need_cap);
			auto p = m_allocator_.allocate(ncap);
			auto dis = distance(m_start_, pos);
			uninitialized_move(m_start_, pos, p, m_allocator_);
			uninitialized_fill_n(elem, p + dis, n, m_allocator_);
			uninitialized_move(pos, m_last_, p + dis + n, m_allocator_);

			clear();

			m_start_ = p;
			m_last_ = p + need_cap;
			m_end_of_storage_ = p + ncap;

			return m_start_ + dis;
		} else {
			auto move_number = distance(pos, m_last_);
			if(move_number > n) {
				// 构造尾部
				uninitialized_move(m_last_ - n, m_last_, m_last_, m_allocator_);

				// 赋值尾部
				auto e = m_last_ - (move_number - n) - 1;
				for(auto i = m_last_ - 1;i != e;--i) {
					*(i) = std::move(*(i - n));
				}

				// 赋值中部
				for(size_t i = 0;i < n;++i) {
					*(pos + i) = elem;
				}
			} else {
				// 构造尾部
				uninitialized_move(pos, m_last_, pos + n, m_allocator_);

				// 构造中部
				uninitialized_fill_n(elem, m_last_, n - move_number, m_allocator_);

				// 赋值中部
				for(auto p = pos;p != m_last_;++p) {
					*p = elem;
				}
			}
			m_last_ += n;
			return pos;
		}
	}

	iterator insert(iterator pos, value_type &&elem) {
		if(m_last_ >= m_end_of_storage_) {
			auto cap = capacity() + 1;
			auto ncap = new_memory(cap);
			auto p = m_allocator_.allocate(ncap);
			auto dis = distance(m_start_, pos);

			uninitialized_move(m_start_, pos, p, m_allocator_);
			m_allocator_.construct(p + dis, std::move(elem));
			uninitialized_move(pos, m_last_, p + dis + 1, m_allocator_);

			clear();

			m_start_ = p;
			m_last_ = p + cap;
			m_end_of_storage_ = p + ncap;

			return m_start_ + dis;
		} else {
			if(pos != m_last_) {
				auto p = m_last_;
				m_allocator_.construct(p, std::move(*(p - 1)));
				--p;
				for(;p != pos;--p) {
					*p = std::move(*(p - 1));
				}
				*pos = std::move(elem);
			} else {
				m_allocator_.construct(pos, std::move(elem));
			}
			++m_last_;
			return pos;
		}
	}

	template <typename I>
	iterator insert(iterator pos, I first, I last) {
		auto n = distance(first, last);
		auto need_cap = size() + n;
		if(need_cap > capacity()) {
			auto ncap = new_memory(need_cap);
			auto p = m_allocator_.allocate(ncap);
			auto dis = distance(m_start_, pos);
			uninitialized_move(m_start_, pos, p, m_allocator_);
			uninitialized_copy(first, last, p + dis, m_allocator_);
			uninitialized_move(pos, m_last_, p + dis + n, m_allocator_);

			clear();

			m_start_ = p;
			m_last_ = p + need_cap;
			m_end_of_storage_ = p + ncap;

			return m_start_ + dis;
		} else {
			auto move_number = distance(pos, m_last_);
			if(move_number > n) {
				// 构造尾部
				uninitialized_move(m_last_ - n, m_last_, m_last_, m_allocator_);

				// 赋值尾部
				auto e = m_last_ - (move_number - n) - 1;
				for(auto i = m_last_ - 1;i != e;--i) {
					*(i) = std::move(*(i - n));
				}

				// 赋值中部
				for(size_t i = 0;i < n;++i) {
					*(pos + i) = *(first + i);
				}
			} else {
				// 构造尾部
				uninitialized_move(pos, m_last_, pos + n, m_allocator_);

				// 构造中部
				uninitialized_copy(first + move_number, last, m_last_, m_allocator_);

				// 赋值中部
				for(auto p = pos;p != m_last_;++p, ++first) {
					*p = *first;
				}
			}
			m_last_ += n;
			return pos;
		}
		return m_start_;
	}

	void push_back(const value_type &elem) {
		insert(end(), elem);
	}

	void push_back(value_type &&elem) {
		insert(end(), std::move(elem));
	}

	template <typename ... Args>
	void emplace_back(Args&& ... args) {
		if(m_last_ >= m_end_of_storage_) {
			auto cap = capacity() + 1;
			auto si = size();
			auto ncap = new_memory(cap);
			auto p = m_allocator_.allocate(ncap);

			uninitialized_move(m_start_, m_last_, p, m_allocator_);
			clear();

			m_start_ = p;
			m_last_ = p + si;
			m_end_of_storage_ = p + ncap;
		}

		m_allocator_.construct(m_last_, std::forward<Args>(args)...);
		++m_last_;
	}

	iterator erase(iterator pos) {
		return erase(pos, pos + 1);
	}

	iterator erase(iterator first, iterator last) {
		auto n = distance(first, last);
		for(auto p = last;p != m_last_;++p) {
			*(p - n) = *p;
		}
		initialized_destory(m_last_ - n, m_last_, m_allocator_);
		m_last_ -= n;
		return first;
	}

	void pop_back() {
		erase(m_last_ - 1);
	}

	// 分配器相关
	inline ALLOCATOR get_allocator() {
		return m_allocator_;
	}
};

template <typename T, typename ALLOCATOR>
void swap(Vector<T, ALLOCATOR> &v, Vector<T, ALLOCATOR> &vv) {
	v.swap(vv);
}

} // namespace stl

#endif // _VECTOR_HPP__