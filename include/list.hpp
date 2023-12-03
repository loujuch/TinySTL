#ifndef _LIST_HPP__
#define _LIST_HPP__

#include "iterator.hpp"
#include "allocator.hpp"
#include "uninitialized.hpp"
#include "functional.hpp"
#include "reverse_iterator.hpp"

namespace stl {

namespace {

template <typename T>
struct Node {
	T m_value_;
	Node<T> *m_prev_;
	Node<T> *m_next_;
public:
	Node() : m_value_(), m_prev_(m_prev_), m_next_(m_next_) {
	}

	Node(const T &t) :m_value_(t), m_prev_(m_prev_), m_next_(m_next_) {
	}

	Node(T &&t) :m_value_(std::move(t)), m_prev_(m_prev_), m_next_(m_next_) {
	}

	Node(const T &t, Node *prev_ptr, Node *next_ptr) :
		m_value_(t), m_prev_(prev_ptr), m_next_(next_ptr) {
	}

	Node(T &&t, Node *prev_ptr, Node *next_ptr) :
		m_value_(std::move(t)), m_prev_(prev_ptr), m_next_(next_ptr) {
	}

	template <typename ... Args>
	Node(Args&& ... args) :
		m_value_(std::forward<Args>(args)...), m_prev_(m_prev_), m_next_(m_next_) {
	}
}; // class ListNode

} // namespace

template <typename T>
class ListIterator :public Iterator<bidirectional_iterator_tag, T> {
public:
	using pointer = Node<T> *;

	template <typename U, typename ALLOC>
	friend class List;
private:
	pointer m_ptr_;
public:
	explicit ListIterator(pointer ptr = nullptr) :m_ptr_(ptr) {
	}

	inline ListIterator<T> &operator++() {
		m_ptr_ = m_ptr_->m_next_;
		return *this;
	}

	inline ListIterator<T> operator++(int) {
		auto out = *this;
		m_ptr_ = m_ptr_->m_next_;
		return out;
	}

	inline ListIterator<T> &operator--() {
		m_ptr_ = m_ptr_->m_prev_;
		return *this;
	}

	inline ListIterator<T> operator--(int) {
		auto out = *this;
		m_ptr_ = m_ptr_->m_prev_;
		return out;
	}

	inline T &operator *() const {
		return m_ptr_->m_value_;
	}

	inline T *operator->() const {
		return &(m_ptr_->m_value_);
	}

	inline pointer base() const {
		return m_ptr_;
	}

	inline bool operator==(const ListIterator<T> &i) const {
		return m_ptr_ == i.m_ptr_;
	}

	inline bool operator!=(const ListIterator<T> &i) const {
		return m_ptr_ != i.m_ptr_;
	}
}; // class ListIterator

template <typename T, typename ALLOC = Allocator<T>>
class List {
public:
	using value_type = T;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using reference = value_type &;
	using const_reference = const value_type &;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	using iterator = ListIterator<value_type>;
	using const_iterator = const ListIterator<value_type>;
	using reverse_iterator = ReverseIterator<iterator>;
	using const_reverse_iterator = const ReverseIterator<iterator>;
private:
	typename ALLOC::template rebind<Node<T>>::other m_allocator_;

	iterator m_head_;
	iterator m_tail_;
	size_type m_length_;

	void create_memory(iterator &head, iterator &tail, size_type n) {
		if(n == 0) {
			return;
		}
		head = iterator(m_allocator_.allocate(1));
		tail = head;
		--n;
		while(n--) {
			tail.m_ptr_->m_next_ = m_allocator_.allocate(1);
			tail.m_ptr_->m_next_->m_prev_ = tail.m_ptr_;
			++tail;
		}
		head.m_ptr_->m_prev_ = nullptr;
		tail.m_ptr_->m_next_ = nullptr;
	}

	void insert_nodes(iterator pos, iterator ipos_first, iterator ipos_last) {
		ipos_first.m_ptr_->m_prev_ = pos.m_ptr_->m_prev_;
		ipos_last.m_ptr_->m_next_ = pos.m_ptr_;
		pos.m_ptr_->m_prev_ = ipos_last.m_ptr_;
		if(ipos_first.m_ptr_->m_prev_) {
			ipos_first.m_ptr_->m_prev_->m_next_ = ipos_first.m_ptr_;
		}
	}

	void erase_nodes(iterator pos_first, iterator pos_last) {
		pos_last.m_ptr_->m_prev_->m_next_ = nullptr;
		pos_last.m_ptr_->m_prev_ = pos_first.m_ptr_->m_prev_;
		pos_first.m_ptr_->m_prev_ = nullptr;
		if(pos_last.m_ptr_->m_prev_) {
			pos_last.m_ptr_->m_prev_->m_next_ = pos_last.m_ptr_;
		}
	}
public:
	// 空间相关
	List() : m_head_(m_allocator_.allocate(1)), m_tail_(m_head_), m_length_(0) {
		m_head_.m_ptr_->m_prev_ = nullptr;
		m_head_.m_ptr_->m_next_ = nullptr;
	}

	explicit List(size_type n) :m_length_(n) {
		create_memory(m_head_, m_tail_, n);
		if(n) {
			m_tail_.m_ptr_->m_next_ = m_allocator_.allocate(1);
			m_tail_.m_ptr_->m_next_->m_prev_ = m_tail_.m_ptr_;
			++m_tail_;
			m_tail_.m_ptr_->m_next_ = nullptr;
		} else {
			m_head_ = iterator(m_allocator_.allocate(1));
			m_tail_ = m_head_;
		}
		uninitialized_fill(m_head_, m_tail_, m_allocator_);
	}

	explicit List(size_type n, const_reference x) :m_length_(n) {
		create_memory(m_head_, m_tail_, n);
		if(n) {
			m_tail_.m_ptr_->m_next_ = m_allocator_.allocate(1);
			m_tail_.m_ptr_->m_next_->m_prev_ = m_tail_.m_ptr_;
			++m_tail_;
			m_tail_.m_ptr_->m_next_ = nullptr;
		} else {
			m_head_ = iterator(m_allocator_.allocate(1));
			m_tail_ = m_head_;
		}
		uninitialized_fill(x, m_head_, m_tail_, m_allocator_);
	}

	template <typename InputIterator>
	explicit List(InputIterator first, InputIterator last) :
		m_length_(distance(first, last)) {
		create_memory(m_head_, m_tail_, m_length_);
		if(m_length_) {
			m_tail_.m_ptr_->m_next_ = m_allocator_.allocate(1);
			m_tail_.m_ptr_->m_next_->m_prev_ = m_tail_.m_ptr_;
			++m_tail_;
			m_tail_.m_ptr_->m_next_ = nullptr;
		} else {
			m_head_ = iterator(m_allocator_.allocate(1));
			m_tail_ = m_head_;
		}
		uninitialized_copy(first, last, m_head_, m_allocator_);
	}

	List(const List<T, ALLOC> &list) :m_length_(list.size()) {
		create_memory(m_head_, m_tail_, m_length_);
		if(m_length_) {
			m_tail_.m_ptr_->m_next_ = m_allocator_.allocate(1);
			m_tail_.m_ptr_->m_next_->m_prev_ = m_tail_.m_ptr_;
			++m_tail_;
			m_tail_.m_ptr_->m_next_ = nullptr;
		} else {
			m_head_ = iterator(m_allocator_.allocate(1));
			m_tail_ = m_head_;
		}
		uninitialized_copy(list.m_head_, list.m_tail_, m_head_, m_allocator_);
	}

	List(List<T, ALLOC> &&list) :
		m_head_(list.m_head_), m_tail_(list.m_tail_), m_length_(list.m_length_) {
		list.m_head_ = iterator(m_allocator_.allocate(1));
		list.m_tail_ = list.m_head_;
		list.m_length_ = 0;

		list.m_head_.m_ptr_->m_prev_ = nullptr;
		list.m_head_.m_ptr_->m_next_ = nullptr;
	}

	~List() {
		clear();
		m_allocator_.deallocate(m_head_.m_ptr_, 1);
		m_length_ = 0;
	}

	List<T, ALLOC> &operator=(const List<T, ALLOC> &l) {
		if(this != &l) {
			clear();

			m_length_ = l.m_length_;
			if(m_length_) {
				auto tmp = m_head_;
				create_memory(m_head_, m_tail_, m_length_);
				m_tail_.m_ptr_->m_next_ = tmp.m_ptr_;
				tmp.m_ptr_->m_prev_ = m_tail_.m_ptr_;
				++m_tail_;

				uninitialized_copy(l.m_head_, l.m_tail_, m_head_, m_allocator_);
			}
		}
		return *this;
	}

	List<T, ALLOC> &operator=(List<T, ALLOC> &&l) {
		if(this != &l) {
			clear();
			l.swap(*this);
		}
		return *this;
	}

	void clear() {
		initialized_destory(m_head_, m_tail_, m_allocator_);
		while(m_head_ != m_tail_) {
			auto tmp = m_head_.m_ptr_->m_next_;
			m_allocator_.deallocate(m_head_.m_ptr_, 1);
			m_head_ = iterator(tmp);
		}

		m_head_.m_ptr_->m_prev_ = nullptr;
		m_head_.m_ptr_->m_next_ = nullptr;
		m_length_ = 0;
	}

	void swap(List<T, ALLOC> &l) {
		if(this != &l) {
			std::swap(l.m_head_, m_head_);
			std::swap(l.m_tail_, m_tail_);
			std::swap(l.m_length_, m_length_);
		}
	}

	template <typename U, typename P>
	friend void swap(List<U, P> &v, List<U, P> &vv);

	//比较操作相关
	bool operator==(const List<T, ALLOC> &v) const {
		if(m_length_ != v.m_length_) {
			return false;
		}
		auto mt = m_head_;
		auto mv = v.m_head_;

		while(mt != m_tail_ && mv != v.m_tail_) {
			if(*mt != *mv) {
				return false;
			}
			++mt;
			++mv;
		}
		return true;
	}

	bool operator!=(const List<T, ALLOC> &v) const {
		return !operator==(v);
	}

	// 迭代器相关
	inline iterator begin() {
		return m_head_;
	}

	inline const_iterator begin() const {
		return m_head_;
	}

	inline iterator end() {
		return m_tail_;
	}

	inline const_iterator end() const {
		return m_tail_;
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
	inline bool empty() const {
		return 0 == m_length_;
	}

	inline size_type size() const {
		return m_length_;
	}

	// 元素相关
	inline reference front() {
		return m_head_.m_ptr_->m_value_;
	}

	inline const_reference front() const {
		return m_head_.m_ptr_->m_value_;
	}

	inline reference back() {
		return m_tail_.m_ptr_->m_prev_->m_value_;
	}

	inline const_reference back() const {
		return m_tail_.m_ptr_->m_prev_->m_value_;
	}

	// 增删操作
	iterator insert(iterator pos, const value_type &elem) {
		iterator ipos(m_allocator_.allocate(1));
		m_allocator_.construct(ipos.base(), elem);
		insert_nodes(pos, ipos, ipos);
		++m_length_;
		if(pos == m_head_) {
			m_head_ = ipos;
		}
		return ipos;
	}

	iterator insert(iterator pos, value_type &&elem) {
		iterator ipos(m_allocator_.allocate(1));
		m_allocator_.construct(ipos.base(), std::move(elem));
		insert_nodes(pos, ipos, ipos);
		++m_length_;
		if(pos == m_head_) {
			m_head_ = ipos;
		}
		return ipos;
	}

	iterator insert(iterator pos, size_type n, const value_type &elem) {
		if(n == 0) {
			return pos;
		}
		iterator ipos_first, ipos_last;
		create_memory(ipos_first, ipos_last, n);
		uninitialized_fill_n(elem, ipos_first, n, m_allocator_);
		insert_nodes(pos, ipos_first, ipos_last);
		m_length_ += n;
		if(pos == m_head_) {
			m_head_ = ipos_first;
		}
		return ipos_first;
	}

	template <typename InputIterator>
	iterator insert(iterator pos, InputIterator first, InputIterator last) {
		auto n = distance(first, last);
		if(n <= 0) {
			return pos;
		}
		iterator ipos_first, ipos_last;
		create_memory(ipos_first, ipos_last, n);
		uninitialized_copy(first, last, ipos_first, m_allocator_);
		insert_nodes(pos, ipos_first, ipos_last);
		m_length_ += n;
		if(pos == m_head_) {
			m_head_ = ipos_first;
		}
		return ipos_first;
	}

	void push_front(const value_type &elem) {
		insert(begin(), elem);
	}

	void push_back(const value_type &elem) {
		insert(end(), elem);
	}

	template <typename ... Args>
	void emplace_front(Args&& ... args) {
		iterator ipos(m_allocator_.allocate(1));
		m_allocator_.construct(ipos.base(), std::forward<Args>(args)...);
		insert_nodes(m_head_, ipos, ipos);
		++m_length_;
		m_head_ = ipos;
	}

	template <typename ... Args>
	void emplace_back(Args&& ... args) {
		iterator ipos(m_allocator_.allocate(1));
		m_allocator_.construct(ipos.base(), std::forward<Args>(args)...);
		insert_nodes(m_tail_, ipos, ipos);
		++m_length_;
		if(m_length_ == 1) {
			m_head_ = ipos;
		}
	}

	iterator erase(iterator pos) {
		if(pos == m_head_) {
			++m_head_;
		}
		auto ipos = pos;
		++ipos;
		erase_nodes(pos, ipos);
		m_allocator_.destory(pos.base());
		m_allocator_.deallocate(pos.base(), 1);
		--m_length_;
		return ipos;
	}

	iterator erase(iterator first, iterator last) {
		if(first == m_head_) {
			m_head_ = last;
		}
		auto n = distance(first, last);
		if(n <= 0) {
			return first;
		}
		initialized_destory(first, last, m_allocator_);
		erase_nodes(first, last);
		auto p = first.m_ptr_;
		while(p != nullptr) {
			auto q = p->m_next_;
			m_allocator_.deallocate(p, 1);
			p = q;
		}
		m_length_ -= n;
		return last;
	}

	void pop_front() {
		erase(m_head_);
	}

	void pop_back() {
		auto n = m_tail_;
		erase(--n);
	}

	// splice
	void splice(iterator pos, List<T, ALLOC> &x) {
		if(x.empty() || this == &x) {
			return;
		}
		auto first = x.begin();
		auto last = x.end();
		auto nlast = last;
		auto length = x.size();
		--nlast;
		erase_nodes(first, last);
		x.m_length_ = 0;
		x.m_head_ = x.m_tail_;
		insert_nodes(pos, first, nlast);
		if(pos == m_head_) {
			m_head_ = first;
		}
		m_length_ += length;
	}

	void splice(iterator pos, List<T, ALLOC> &x, iterator i) {
		auto ii = i;
		++ii;
		splice(pos, x, i, ii);
	}

	void splice(iterator pos, List<T, ALLOC> &x, iterator first, iterator last) {
		if(this == &x) {
			return;
		}
		if(first == x.m_head_) {
			x.m_head_ = last;
		}
		auto n = distance(first, last);
		if(n <= 0) {
			return;
		}
		auto ii = last;
		--ii;
		erase_nodes(first, last);
		x.m_length_ -= n;
		insert_nodes(pos, first, ii);
		if(pos == m_head_) {
			m_head_ = first;
		}
		m_length_ += n;
	}

	void splice(iterator pos, List<T, ALLOC> &&x) {
		splice(pos, x);
	}

	void splice(iterator pos, List<T, ALLOC> &&x, iterator i) {
		splice(pos, x, i);
	}

	void splice(iterator pos, List<T, ALLOC> &&x, iterator first, iterator last) {
		splice(pos, x, first, last);
	}

	// merge 元素比较次数小于 N+R-1
	void merge(List<T, ALLOC> &other) {
		merge(other, less<value_type>());
	}

	void merge(List<T, ALLOC> &&other) {
		merge(other);
	}

	template <typename Compare>
	void merge(List<T, ALLOC> &other, Compare comp) {
		if(this == &other || other.empty()) {
			return;
		}
		auto m_first = m_head_;
		auto o_first = other.m_head_;
		while(o_first != other.m_tail_) {
			while(m_first != m_tail_ && comp(*m_first, *o_first)) {
				++m_first;
			}
			if(m_first == m_tail_) {
				splice(m_first, other, o_first, other.m_tail_);
				break;
			}
			auto o_last = o_first;
			++o_last;
			while(o_last != other.m_tail_ && !comp(*m_first, *o_last)) {
				++o_last;
			}
			splice(m_first, other, o_first, o_last);
			o_first = o_last;
		}
	}

	template <typename Compare>
	void merge(List<T, ALLOC> &&other, Compare comp) {
		merge(other, comp);
	}

	// remove
	void remove(const value_type &value) {
		auto p = m_head_;
		while(p != m_tail_ && *p == value) {
			++p;
		}
		if(p != m_head_) {
			erase(m_head_, p);
			m_head_ = p;
		}
		auto q = p;
		++q;
		while(p != m_tail_) {
			if(*p == value) {
				erase(p);
				p = q;
			} else {
				++p;
			}
			++q;
		}
	}

	template<typename UnaryPredicate>
	void remove_if(UnaryPredicate up) {
		auto p = m_head_;
		while(p != m_tail_ && up(*p)) {
			++p;
		}
		if(p != m_head_) {
			erase(m_head_, p);
			m_head_ = p;
		}
		auto q = p;
		++q;
		while(p != m_tail_) {
			if(up(*p)) {
				erase(p);
				p = q;
			} else {
				++p;
			}
			++q;
		}
	}

	// unique
	void unique() {
		auto p = m_head_, q = p;
		++q;
		while(p != m_tail_) {
			while(q != m_tail_ && *p == *q) {
				++q;
			}
			++p;
			if(p != q) {
				erase(p, q);
			}
			p = q;
			++q;
		}
	}

	template<typename BinaryPredicate>
	void unique(BinaryPredicate bp) {
		auto p = m_head_, q = p;
		++q;
		while(p != m_tail_) {
			while(q != m_tail_ && bp(*p, *q)) {
				++q;
			}
			++p;
			if(p != q) {
				erase(p, q);
			}
			p = q;
			++q;
		}
	}

	// 反转
	void reverse() noexcept {
		if(empty()) {
			return;
		}
		auto p = m_head_.m_ptr_, q = m_tail_.m_ptr_;
		while(p != q) {
			std::swap(p->m_prev_, p->m_next_);
			p = p->m_prev_;
		}
		auto m = q->m_prev_;
		m->m_prev_ = nullptr;
		q->m_prev_ = m_head_.m_ptr_;
		m_head_.m_ptr_->m_next_ = q;
		m_head_.m_ptr_ = m;
	}

	// 排序
	void sort() {
		sort(less<value_type>());
	}

	template <typename Compare>
	void sort(Compare comp) {
		if(m_length_ <= 1) {
			return;
		}
		List<T, ALLOC> carry;
		List<T, ALLOC> counter[64];

		size_type fill = 0;
		while(!empty()) {
			// 获取一个元素
			carry.splice(carry.begin(), *this, begin());

			size_type i = 0;
			while(i < fill && !counter[i].empty()) {
				carry.merge(counter[i++], comp);
			}
			carry.swap(counter[i]);
			fill += fill == i;
		}
		for(size_type i = 1;i < fill;++i) {
			counter[i].merge(counter[i - 1], comp);
		}
		counter[fill - 1].swap(*this);
	}
}; // class List


template <typename U, typename P>
void swap(List<U, P> &v, List<U, P> &vv) {
	v.swap(vv);
}

} // namespace stl

#endif // _LIST_HPP__