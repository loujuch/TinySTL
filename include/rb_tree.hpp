#ifndef _RB_TREE_HPP__
#define _RB_TREE_HPP__

#include "iterator.hpp"
#include "functional.hpp"
#include "allocator.hpp"
#include "utility.hpp"

namespace stl {

struct RBNodeBase {
public:
	using RBNodeColor = bool;
	using pointer = RBNodeBase *;

	static constexpr RBNodeColor rb_node_red = true;
	static constexpr RBNodeColor rb_node_black = true;
public:
	RBNodeColor m_color;
	pointer m_left;
	pointer m_right;
	pointer m_parent;
public:
	RBNodeBase(RBNodeColor color = rb_node_black) :
		m_color(color), m_left(nullptr), m_right(nullptr), m_parent(nullptr) {
	}
}; // struct RBNodeBase

template <typename T>
struct RBNode :public RBNodeBase {
public:
	using RBNodeColor = typename RBNodeBase::RBNodeColor;
	using value_type = T;
	using pointer = RBNode *;
public:
	value_type m_value;
public:
	RBNode() :RBNodeBase(), m_value() {
	}

	RBNode(const value_type &v) :RBNodeBase(), m_value(v) {
	}

	RBNode(value_type &&v) :RBNodeBase(), m_value(std::move(v)) {
	}
}; // struct RBNode

class RBBaseIterator {
public:
	using iterator_category = bidirectional_iterator_tag;
	using difference_type = ::ptrdiff_t;


	using base_ptr = typename RBNodeBase::pointer;
private:
	base_ptr m_pointer_;
public:
	RBBaseIterator(base_ptr p = nullptr) :m_pointer_(p) {
	}

	// 搜索树中下一个较大的节点
	void increment() {
		if(m_pointer_->m_right) {
			m_pointer_ = m_pointer_->m_right;
			while(m_pointer_->m_left) {
				m_pointer_ = m_pointer_->m_left;
			}
		} else {
			base_ptr p = m_pointer_->m_parent;
			while(m_pointer_ == p->m_right) {
				m_pointer_ = p;
				p = p->m_parent;
			}
			if(m_pointer_->m_right != p) {
				m_pointer_ = p;
			}
		}
	}

	// 搜索树中下一个较小的节点
	void decrement() {
		if(m_pointer_->m_color == RBNodeBase::rb_node_red &&
			m_pointer_->m_parent->m_parent == m_pointer_) {
			m_pointer_ = m_pointer_->m_right;
		} else if(m_pointer_->m_left) {
			m_pointer_ = m_pointer_->m_left;
			while(m_pointer_->m_right) {
				m_pointer_ = m_pointer_->m_right;
			}
		} else {
			base_ptr p = m_pointer_->m_parent;
			while(m_pointer_ != m_pointer_->m_right) {
				m_pointer_ = p;
				p = p->m_parent;
			}
			m_pointer_ = p;
		}
	}
}; // class RBBaseIterator

template <typename T>
class RBIterator :public RBBaseIterator {
	using iterator_category = typename RBBaseIterator::iterator_category;
	using difference_type = typename RBBaseIterator::difference_type;
	using value_type = T;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using reference = value_type &;
	using const_reference = const value_type &;

	using base_ptr = typename RBNode<T>::pointer;
	using self = RBIterator<T>;
private:
public:
	RBIterator(base_ptr p = nullptr) :RBBaseIterator(p) {
	}

	const_reference operator*() const {
		return static_cast<base_ptr>(m_pointer_)->value;
	}

	const_pointer operator->() const {
		return &(static_cast<base_ptr>(m_pointer_)->value);
	}

	self &operator++() {
		increment();
		return *this;
	}

	self operator++(int) {
		auto out = *this;
		increment();
		return out;
	}

	self &operator--() {
		decrement();
		return *this;
	}

	self operator--(int) {
		auto out = *this;
		decrement();
		return out;
	}
};

template <typename T, typename Compare = less<T>, typename ALLOC = Allocator<T>>
class RBTree {
public:
	using value_type = T;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using reference = value_type &;
	using const_reference = const value_type &;

	using size_type = ::size_t;
	using difference_type = ::ptrdiff_t;

	using link_type = RBNode<T> *;
	using rb_color = typename RBNodeBase::RBNodeColor;

	using iterator = RBIterator<T>;
	using const_iterator = const RBIterator<T>;

	using insert_return = Pair<iterator, bool>;

	static constexpr rb_color rb_red = RBNodeBase::rb_node_red;
	static constexpr rb_color rb_black = RBNodeBase::rb_node_black;
protected:
	typename ALLOC::template rebind<RBNode<T>>::other m_allocator_;

	size_type m_size_;
	link_type m_head_;
	Compare m_comparator_;

protected:
	link_type get_node_mem() {
		return m_allocator_.allocate(1);
	}

	void free_node_mem(link_type p) {
		m_allocator_.deallocate(p, 1);
	}

	link_type create_node(const value_type &v) {
		link_type tmp = get_node_mem();
		m_allocator_.construct(tmp, v);
		return tmp;
	}

	link_type create_node(value_type &&v) {
		link_type tmp = get_node_mem();
		m_allocator_.construct(tmp, std::move(v));
		return tmp;
	}

	link_type clone_node(link_type l) {
		link_type tmp = create_node(*l);
		tmp->m_color = l->m_color;
		tmp->m_left = nullptr;
		tmp->m_right = nullptr;
		tmp->m_parent = nullptr;
		return tmp;
	}

	void destory_node(link_type l) {
		m_allocator_.destory(l);
		free_node_mem(l);
	}

protected:
	inline static link_type &parent(link_type p) {
		return (link_type &)(p->m_parent);
	}

	inline static link_type &left(link_type p) {
		return (link_type &)(p->m_left);
	}

	inline static link_type &right(link_type p) {
		return (link_type &)(p->m_right);
	}

	inline static const_reference value(link_type p) {
		return p->m_value;
	}

	inline static rb_color color(link_type p) {
		return p->m_color;
	}

	inline static bool is_red(link_type p) {
		return p->m_color == rb_red;
	}

	inline static bool is_black(link_type p) {
		return p->m_color == rb_black;
	}

protected:
	inline link_type &root() const {
		return parent(m_head_);
	}

	inline link_type &most_left() const {
		return left(m_head_);
	}

	inline link_type &most_right() const {
		return right(m_head_);
	}

protected:
	inline static link_type minimum(link_type p) {
		while(p->m_left) {
			p = left(p);
		}
		return p;
	}

	inline static link_type maximum(link_type p) {
		while(p->m_right) {
			p = right(p);
		}
		return p;
	}

protected:
	void shrink_head() {
		parent(m_head_) = nullptr;
		left(m_head_) = m_head_;
		right(m_head_) = m_head_;
	}

	link_type clone(link_type parent, link_type source) {
		if(source == nullptr) {
			return nullptr;
		}
		link_type tmp = clone_node(source);
		tmp->m_parent = parent;
		tmp->m_left = clone(tmp, source->m_left);
		tmp->m_right = clone(tmp, source->m_right);
		return tmp;
	}

	void destory(link_type p) {
		if(p == nullptr) {
			return;
		}
		destory(left(p));
		destory(right(p));
		destory_node(p);
	}

	void init() {
		m_head_ = get_node_mem();
		m_head_->m_color = rb_red;
		shrink_head();
	}
public:
	RBTree(Compare comp = Compare()) :m_size_(0), m_head_(nullptr), m_comparator_(comp) {
		init();
	}

	RBTree(const RBTree &rbt) :RBTree() {
		root() = clone(m_head_, rbt.root());
		most_left() = minimum(root());
		most_right() = maximum(root());
	}

	RBTree(RBTree &&rbt) :m_size_(rbt.m_size_), m_head_(rbt.m_head_), m_comparator_(rbt.m_comparator_) {
		rbt.m_size_ = 0;
		rbt.m_head_ = nullptr;
		rbt.init();
	}

	~RBTree() {
		clear();
		destory_node(m_head_);
	}

	void clear() {
		if(m_size_) {
			destory(root());
			shrink_head();
			m_size_ = 0;
		}
	}

	insert_return insert_unique(const value_type &value) {
		link_type p = m_head_;
		link_type q = root();
	}

	insert_return insert_equal(const value_type &value) {
	}

	insert_return insert_unique(value_type &&value) {
	}

	insert_return insert_equal(value_type &&value) {
	}
protected:
	insert_return __insert() {
	}
}; // class RBTree

} // namespace stl


#endif // _RB_TREE_HPP__