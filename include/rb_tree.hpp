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

	static constexpr RBNodeColor rb_node_red = false;
	static constexpr RBNodeColor rb_node_black = true;
public:
	RBNodeColor m_color;
	pointer m_left;
	pointer m_right;
	pointer m_parent;
public:
	RBNodeBase(RBNodeColor color = rb_node_red) :
		m_color(color), m_left(nullptr), m_right(nullptr), m_parent(nullptr) {
	}

	// 将结点值的交换转化为对指针的交换，用于提高速度并减少失效迭代器数量
	void swap(RBNodeBase &p) {
		if(this != &p) {
			// 交换结点颜色
			std::swap(m_color, p.m_color);
			if(m_parent == p.m_parent) {
				// 共用父结点

				// 交换父结点
				if(m_parent) {
					std::swap(m_parent->m_left, m_parent->m_right);
				}

				// 交换左结点
				if(m_left) {
					m_left->m_parent = &p;
				}
				if(p.m_left) {
					p.m_left->m_parent = this;
				}

				// 交换右结点
				if(m_right) {
					m_right->m_parent = &p;
				}
				if(p.m_right) {
					p.m_right->m_parent = this;
				}

				// 交换内部结点
				std::swap(m_left, p.m_left);
				std::swap(m_right, p.m_right);
				std::swap(m_parent, p.m_parent);
			} else if(p.m_parent == this) {
				if(this->m_left == &p) {
					// p为this左结点

					// 解连接
					p.m_parent = this->m_parent;
					this->m_left = p.m_left;

					// 内链接回调
					this->m_parent = &p;
					p.m_left = this;

					// 外连接回调
					if(p.m_parent) {
						if(p.m_parent->m_left == this) {
							p.m_parent->m_left = &p;
						}
						if(this == m_parent->m_right) {
							p.m_parent->m_right = &p;
						}
					}
					if(this->m_left) {
						this->m_left->m_parent = this;
					}

					// 纯外连接交换
					if(p.m_right) {
						p.m_right->m_parent = this;
					}
					if(this->m_right) {
						this->m_right->m_parent = &p;
					}
					std::swap(p.m_right, this->m_right);
				} else {
					// p为this右结点

					// 解连接
					p.m_parent = this->m_parent;
					this->m_right = p.m_right;

					// 内链接回调
					this->m_parent = &p;
					p.m_right = this;

					// 外连接回调
					if(p.m_parent) {
						if(p.m_parent->m_right == this) {
							p.m_parent->m_right = &p;
						}
						if(p.m_parent->m_left == this) {
							p.m_parent->m_left = &p;
						}
					}
					if(this->m_right) {
						this->m_right->m_parent = this;
					}

					// 纯外连接交换
					if(p.m_left) {
						p.m_left->m_parent = this;
					}
					if(this->m_left) {
						this->m_left->m_parent = &p;
					}
					std::swap(p.m_left, this->m_left);
				}

			} else if(m_parent == &p) {
				if(p.m_left == this) {
					// this为p左结点

					// 解连接
					this->m_parent = p.m_parent;
					p.m_left = this->m_left;

					// 内链接回调
					p.m_parent = this;
					this->m_left = &p;

					// 外连接回调
					if(this->m_parent) {
						if(this->m_parent->m_left == &p) {
							this->m_parent->m_left = this;
						}
						if(this->m_parent->m_right == &p) {
							this->m_parent->m_right = this;
						}
					}
					if(p.m_left) {
						p.m_left->m_parent = &p;
					}

					// 纯外连接交换
					if(p.m_right) {
						p.m_right->m_parent = this;
					}
					if(this->m_right) {
						this->m_right->m_parent = &p;
					}
					std::swap(p.m_right, this->m_right);

				} else {
					// this为p右结点

					// 解连接
					this->m_parent = p.m_parent;
					p.m_right = this->m_right;

					// 内链接回调
					p.m_parent = this;
					this->m_right = &p;

					// 外连接回调
					if(this->m_parent) {
						if(this->m_parent->m_right == &p) {
							this->m_parent->m_right = this;
						}
						if(this->m_parent->m_left == &p) {
							this->m_parent->m_left = this;
						}
					}
					if(p.m_right) {
						p.m_right->m_parent = &p;
					}

					// 纯外连接交换
					if(p.m_left) {
						p.m_left->m_parent = this;
					}
					if(this->m_left) {
						this->m_left->m_parent = &p;
					}
					std::swap(p.m_left, this->m_left);
				}
			} else {
				// 无共用结点

				// 交换父结点
				if(m_parent) {
					if(this == m_parent->m_left) {// 为什么是这边？
						m_parent->m_left = &p;
					}
					if(this == m_parent->m_right) {
						m_parent->m_right = &p;
					}
				}
				if(p.m_parent) {
					if(&p == p.m_parent->m_left) {
						p.m_parent->m_left = this;
					}
					if(this == m_parent->m_right) {
						p.m_parent->m_right = this;
					}
				}

				// 交换左结点
				if(m_left) {
					m_left->m_parent = &p;
				}
				if(p.m_left) {
					p.m_left->m_parent = this;
				}

				// 交换右结点
				if(m_right) { // 为什么会运行到这？
					m_right->m_parent = &p;
				}
				if(p.m_right) {
					p.m_right->m_parent = this;
				}

				// 交换内部结点
				std::swap(m_left, p.m_left);
				std::swap(m_right, p.m_right);
				std::swap(m_parent, p.m_parent);
			}
		}
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

	template <typename ... Args>
	RBNode(Args&& ... args) : RBNodeBase(), m_value(std::forward<Args>(args)...) {
	}
}; // struct RBNode

class RBBaseIterator {
public:
	using iterator_category = bidirectional_iterator_tag;
	using difference_type = ::ptrdiff_t;


	using base_ptr = typename RBNodeBase::pointer;
	using self = RBBaseIterator;
protected:
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
			while(m_pointer_ != p->m_right) {
				m_pointer_ = p;
				p = p->m_parent;
			}
			m_pointer_ = p;
		}
	}

	bool operator==(const self &i) const {
		return m_pointer_ == i.m_pointer_;
	}

	bool operator!=(const self &i) const {
		return !operator==(i);
	}
}; // class RBBaseIterator

template <typename T>
class RBIterator :public RBBaseIterator {
	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename ALLOC>
	friend class RBTree;

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
	base_ptr ptr() {
		return (base_ptr)m_pointer_;
	}
public:
	RBIterator(base_ptr p = nullptr) :RBBaseIterator(p) {
	}

	reference operator*() {
		return static_cast<base_ptr>(m_pointer_)->m_value;
	}

	pointer operator->() {
		return &(static_cast<base_ptr>(m_pointer_)->m_value);
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

template <typename Key, typename Value = Key, typename KeyOfValue = Identity<Value>,
	typename Compare = less<Key>, typename ALLOC = Allocator<Value>
>
class RBTree {
public:
	using key_type = Key;
	using value_type = Value;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using reference = value_type &;
	using const_reference = const value_type &;

	using size_type = ::size_t;
	using difference_type = ::ptrdiff_t;

	using link_type = RBNode<value_type> *;
	using rb_color = typename RBNodeBase::RBNodeColor;

	using iterator = RBIterator<value_type>;
	using const_iterator = const RBIterator<value_type>;

	using insert_return = Pair<iterator, bool>;

	static constexpr rb_color rb_red = RBNodeBase::rb_node_red;
	static constexpr rb_color rb_black = RBNodeBase::rb_node_black;
protected:
	typename ALLOC::template rebind<RBNode<value_type>>::other m_allocator_;

	size_type m_size_;
	link_type m_head_;
	Compare m_comparator_;
	KeyOfValue m_key_of_value_;

protected:
	bool compare_vv(const value_type &l, const value_type &r) const {
		return m_comparator_(m_key_of_value_(l), m_key_of_value_(r));
	}

	bool compare_vk(const value_type &l, const key_type &r) const {
		return m_comparator_(m_key_of_value_(l), r);
	}

	bool compare_kv(const key_type &l, const value_type &r) const {
		return m_comparator_(l, m_key_of_value_(r));
	}
protected:
	link_type get_node_mem() {
		return m_allocator_.allocate(1);
	}

	void free_node_mem(link_type p) {
		m_allocator_.deallocate(p, 1);
	}


	template <typename ... Args>
	link_type create_node(Args&& ... args) {
		link_type tmp = get_node_mem();
		m_allocator_.construct(tmp, std::forward<Args>(args)...);
		return tmp;
	}

	link_type clone_node(link_type l) {
		link_type tmp = create_node(l->m_value);
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
		return p != nullptr && p->m_color == rb_red;
	}

	inline static bool is_black(link_type p) {
		return  !is_red(p);
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
		tmp->m_left = clone(tmp, left(source));
		tmp->m_right = clone(tmp, right(source));
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
		m_size_ = rbt.m_size_;
	}

	RBTree(RBTree &&rbt) :
		m_size_(rbt.m_size_), m_head_(rbt.m_head_), m_comparator_(rbt.m_comparator_) {
		rbt.m_size_ = 0;
		rbt.m_head_ = nullptr;
		rbt.init();
	}

	~RBTree() {
		clear();
		destory_node(m_head_);
	}

	RBTree &operator=(const RBTree &rb) {
		if(this != &rb) {
			clear();

			root() = clone(m_head_, rb.root());
			most_left() = minimum(root());
			most_right() = maximum(root());
		}
		return *this;
	}

	RBTree &operator=(RBTree &&rb) {
		if(this != &rb) {
			clear();
			this->swap(rb);
		}
		return *this;
	}

	void clear() {
		if(m_size_) {
			destory(root());
			shrink_head();
			m_size_ = 0;
		}
	}

	void swap(RBTree &rb) {
		if(this != &rb) {
			std::swap(m_allocator_, rb.m_allocator_);
			std::swap(m_comparator_, rb.m_comparator_);
			std::swap(m_head_, rb.m_head_);
			std::swap(m_key_of_value_, rb.m_key_of_value_);
			std::swap(m_size_, rb.m_size_);
		}
	}

	inline iterator begin() const {
		return iterator(most_left());
	}

	inline iterator end() const {
		return iterator(m_head_);
	}

	inline size_type size() const {
		return m_size_;
	}

	inline bool empty() const {
		return m_size_ == 0;;
	}

	iterator find(const key_type &value) const {
		iterator tmp = lower_bound(value);
		return (tmp == end() || compare_kv(value, *tmp)) ? end() : tmp;
	}

	// 返回第一个不小于的迭代器
	iterator lower_bound(const key_type &value) const {
		// 接下来保存 y 在大于等于 value 的位置
		link_type y = m_head_;

		link_type x = root();

		while(x) {
			if(compare_vk(x->m_value, value)) {
				// x小于value, 
				x = right(x);
			} else {
				// x大于等于value，更新y
				y = x;
				x = left(x);
			}
		}

		// y未改变或 value小于y
		return (y == m_head_) ? end() : iterator(y);
	}

	// 返回第一个大于的迭代器
	iterator upper_bound(const key_type &value) const {
		// 接下来保存 y 在大于等于 value 的位置
		link_type y = m_head_;

		link_type x = root();

		while(x) {
			if(compare_kv(value, x->m_value)) {
				// value小于x，更新y
				y = x;
				x = left(x);
			} else {
				// value大于等于x
				x = right(x);
			}
		}

		// y未改变
		return (y == m_head_) ? end() : iterator(y);
	}

	size_type erase_unique(const key_type &key) {
		iterator l = find(key);
		if(l == end()) {
			return 0;
		}
		erase(l);
		return 1;
	}

	size_type erase_equal(const key_type &key) {
		iterator l = find(key);
		if(l == end()) {
			return 0;
		}
		iterator r = upper_bound(key);
		erase(l, r);
		return distance(l, r);
	}

	iterator erase(iterator pos) {
		iterator out = pos;
		++out;
		__erase(pos.ptr());
		return out;
	}

	// TODO: 降低算法复杂度到 log(size)+distance(first, last)
	iterator erase(iterator first, iterator last) {
		for(;first != last;++first) {
			erase(first);
		}
		return last;
	}

	template <typename ... Args>
	insert_return emplace_unique(Args&& ... args) {
		link_type y = m_head_;
		link_type x = root();

		bool le = true;

		link_type tmp = create_node(std::forward<Args>(args)...);

		while(x) {
			y = x;
			le = compare_vv(tmp->m_value, x->m_value);
			x = le ? left(x) : right(x);
		}

		iterator i(y);

		if(le) {
			// 插入为左部，不可能与父结点相等
			if(i == begin()) {
				// 一定不相等
				return insert_return(__insert(tmp, y, le), true);
			} else {
				// 获取最接近的结点
				--i;
			}
		}

		if(compare_vv(*i, tmp->m_value)) {
			// 不相等
			return insert_return(__insert(tmp, y, le), true);
		}

		destory_node(tmp);

		return insert_return(i, false);
	}

	template <typename ... Args>
	insert_return emplace_equal(Args&& ... args) {
		link_type y = m_head_;
		link_type x = root();

		bool le = true;

		link_type tmp = create_node(std::forward<Args>(args)...);

		while(x) {
			y = x;
			le = compare_vv(x->m_value, tmp->m_value);
			x = le ? right(x) : left(x);
		}

		return insert_return(__insert(tmp, y, le), true);
	}


	insert_return insert_unique(const value_type &value) {
		link_type y = m_head_;
		link_type x = root();

		bool le = true;

		while(x) {
			y = x;
			le = compare_vv(value, x->m_value);
			x = le ? left(x) : right(x);
		}

		iterator i(y);

		if(le) {
			// 插入为左部，不可能与父结点相等
			if(i == begin()) {
				// 一定不相等
				return insert_return(__insert(create_node(value), y, le), true);
			} else {
				// 获取最接近的结点
				--i;
			}
		}

		if(compare_vv(*i, value)) {
			// 不相等
			return insert_return(__insert(create_node(value), y, le), true);
		}

		return insert_return(i, false);
	}

	insert_return insert_equal(const value_type &value) {
		link_type y = m_head_;
		link_type x = root();

		bool le = true;

		while(x) {
			y = x;
			le = compare_vv(x->m_value, value);
			x = le ? right(x) : left(x);
		}

		return insert_return(__insert(create_node(value), y, le), true);
	}

	insert_return insert_unique(value_type &&value) {
		link_type y = m_head_;
		link_type x = root();

		bool le = true;

		while(x) {
			y = x;
			le = compare_vv(value, x->m_value);
			x = le ? left(x) : right(x);
		}

		iterator i(y);

		if(le) {
			// 插入为左部，不可能与父结点相等
			if(i == begin()) {
				// 一定不相等
				return insert_return(__insert(create_node(std::move(value)), y, le), true);
			} else {
				// 获取最接近的结点
				--i;
			}
		}

		if(compare_vv(*i, value)) {
			// 不相等
			return insert_return(__insert(create_node(std::move(value)), y, le), true);
		}

		return insert_return(i, false);
	}

	insert_return insert_equal(value_type &&value) {
		link_type y = m_head_;
		link_type x = root();

		bool le = true;

		while(x) {
			y = x;
			le = compare_vv(x->m_value, value);
			x = le ? right(x) : left(x);
		}

		return insert_return(__insert(create_node(std::move(value)), y, le), true);
	}

	void dfs(link_type n) {
		if(n == nullptr) {
			std::cout << "null ";
			return;
		}
		std::cout << n->m_value << ':' << n->m_color << ' ';
		dfs(left(n));
		dfs(right(n));
	}

	void show() {
		dfs(root());
		std::cout << std::endl;
	}
protected:
	void __erase(link_type z) {
		if(m_size_ == 1) {
			destory_node(root());
			shrink_head();
			m_size_ = 0;
			return;
		}

		// 最大结点一定没有两个子结点，无需考虑end()
		link_type y = (left(z) == nullptr || right(z) == nullptr) ? z : (++iterator(z)).ptr();
		link_type x = left(y) ? left(y) : right(y);

		if(y != z) {
			// z有两子结点，y只有一子结点或无子结点，x指向y的右子节点
			y->swap(*z);
			if(z == root()) {
				root() = y;
			}
			y = z;
		} else {
			// z可能是极值结点
			if(most_left() == z) {
				most_left() = (++iterator(most_left())).ptr();
			}
			if(most_right() == z) {
				most_right() = (--iterator(most_right())).ptr();
			}
		}

		// y只有一子结点或无子结点，x为y唯一子结点或null（卸下y结点，使用x结点取代）
		link_type p = parent(y);

		if(x != nullptr) {
			parent(x) = p;
		}
		if(y == root()) {
			root() = x;
		} else if(left(p) == y) {
			left(p) = x;
		} else {
			right(p) = x;
		}

		if(x != nullptr) {
			erase_rb_tree_rebanlance(y, x);
		}

		// 删去结点
		destory_node(y);

		// 取出一结点
		--m_size_;
	}

	void erase_rb_tree_rebanlance(link_type y, link_type &x) {
		if(is_red(y)) {
			return;
		}
		link_type p = parent(x);
		while(x != root() && is_black(x)) {
			if(x == left(p)) {
				link_type b = right(p);
				if(is_red(b)) {
					b->m_color = rb_black;
					p->m_color = rb_red;

					rotate_left(p, root());
					b = right(p);
				}
				if(is_black(left(b)) && is_black(right(b))) {
					b->m_color = rb_red;

					x = p;
					p = parent(x);
				} else {
					if(is_black(right(b))) {
						if(left(b) != nullptr) {
							left(b)->m_color = rb_black;
						}
						b->m_color = rb_red;
						rotate_right(b, root());
						b = right(p);
					}
					b->m_color = p->m_color;
					p->m_color = rb_black;

					if(right(b) != nullptr) {
						right(b)->m_color = rb_black;
					}

					rotate_left(p, root());
					break;
				}
			} else {
				link_type b = left(p);
				if(is_red(b)) {
					b->m_color = rb_black;
					p->m_color = rb_red;

					rotate_right(p, root());
					b = left(p);
				}
				if(is_black(left(b)) && is_black(right(b))) {
					b->m_color = rb_red;

					x = p;
					p = parent(x);
				} else {
					if(is_black(left(b))) {
						if(right(b) != nullptr) {
							right(b)->m_color = rb_black;
						}
						b->m_color = rb_red;
						rotate_left(b, root());
						b = left(p);
					}
					b->m_color = p->m_color;
					p->m_color = rb_black;

					if(left(b) != nullptr) {
						left(b)->m_color = rb_black;
					}

					rotate_right(p, root());
					break;
				}
			}
		}
		if(x != nullptr) {
			x->m_color = rb_black;
		}
	}

	iterator __insert(link_type insert_obj, link_type parent_obj, bool is_less) {

		parent(insert_obj) = parent_obj;
		left(insert_obj) = nullptr;
		right(insert_obj) = nullptr;

		if(parent_obj == m_head_) {
			// 为空时插入
			insert_obj->m_color = rb_black;

			root() = insert_obj;
			most_left() = root();
			most_right() = root();

			m_size_ = 1;
			return insert_obj;
		}

		insert_obj->m_color = rb_red;

		if(is_less) {
			// 插入左部
			left(parent_obj) = insert_obj;
			if(parent_obj == most_left()) {
				most_left() = insert_obj;
			}
		} else {
			// 插入右部
			right(parent_obj) = insert_obj;
			if(parent_obj == most_right()) {
				most_right() = insert_obj;
			}
		}

		insert_rb_tree_rebanlance(insert_obj, root());

		++m_size_;

		return iterator(insert_obj);
	}

	void insert_rb_tree_rebanlance(link_type x, link_type &r) {
		while(x != r && is_red(parent(x))) {
			// 父结点
			link_type p = parent(x);
			// 爷爷结点
			link_type pp = parent(p);

			// 结点父结点是否为左结点
			bool is_left_p = (p == left(pp));

			link_type opp;

			// 获取伯父结点
			if(is_left_p) {
				opp = right(pp);
			} else {
				opp = left(pp);
			}

			if(opp && is_red(opp)) {
				// 存在伯父结点且其为红色，进行重染色
				p->m_color = rb_black;
				opp->m_color = rb_black;
				pp->m_color = rb_red;

				// 递归向上
				x = pp;
			} else {
				// 伯父结点不存在或其为红色，进行旋转

				bool is_left_x = (x == left(p));
				if(is_left_p ^ is_left_x) {
					x = p;
					if(is_left_p) {
						rotate_left(x, r);
					} else {
						rotate_right(x, r);
					}

					p = parent(x);
					pp = parent(p);
				}

				p->m_color = rb_black;
				pp->m_color = rb_red;

				if(is_left_p) {
					rotate_right(pp, r);
				} else {
					rotate_left(pp, r);
				}
			}
		}
		r->m_color = rb_black;
	}

	void rotate_left(link_type x, link_type &r) {
		link_type y = right(x);
		right(x) = left(y);
		if(left(y)) {
			parent(left(y)) = x;
		}
		parent(y) = parent(x);

		if(x == r) {
			r = y;
		} else if(x == left(parent(x))) {
			left(parent(x)) = y;
		} else {
			right(parent(x)) = y;
		}
		left(y) = x;
		parent(x) = y;
	}

	void rotate_right(link_type x, link_type &r) {
		link_type y = left(x);
		left(x) = right(y);
		if(right(y)) {
			parent(right(y)) = x;
		}
		parent(y) = parent(x);

		if(x == r) {
			r = y;
		} else if(x == left(parent(x))) {
			left(parent(x)) = y;
		} else {
			right(parent(x)) = y;
		}
		right(y) = x;
		parent(x) = y;
	}
}; // class RBTree

} // namespace stl


#endif // _RB_TREE_HPP__