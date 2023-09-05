#ifndef _SET_HPP__
#define _SET_HPP__

#include "functional.hpp"
#include "allocator.hpp"
#include "rb_tree.hpp"

namespace stl {

template <typename T, typename Compare = stl::less<const T>,
	typename ALLOC = stl::Allocator<T>
>
class Set {
public:
	using key_type = T;
	using value_type = const T;
	using size_type = ::size_t;
	using difference_type = ::ptrdiff_t;
	using key_compare = Compare;
	using allocator_type = ALLOC;
	using reference = value_type &;
	using const_reference = const value_type &;
	using pointer = value_type *;
	using const_pointer = const value_type *;

	using iterator = RBIterator<value_type>;
	using const_iterator = const iterator;
private:
private:
	RBTree<key_type, value_type, Identity<const value_type>, Compare, ALLOC> m_rb_tree_;
public:
	explicit Set(const Compare &comp = Compare()) :m_rb_tree_(comp) {
	}

	Set(const Set &other) :m_rb_tree_(other.m_rb_tree_) {
	}

	Set(Set &&other) :m_rb_tree_(std::move(other.m_rb_tree_)) {
	}

	template <typename InputIterator>
	Set(InputIterator first, InputIterator last, const Compare &comp = Compare()) :
		Set(comp) {
		insert(first, last);
	}

	Set &operator=(const Set &other) {
		if(this != &other) {
			m_rb_tree_ = other.m_rb_tree_;
		}
		return *this;
	}

	Set &operator=(Set &&other) {
		if(this != &other) {
			m_rb_tree_ = std::move(other.m_rb_tree_);
		}
		return *this;
	}

	void clear() {
		m_rb_tree_.clear();
	}

	const_iterator begin() const {
		return m_rb_tree_.begin();
	}

	const_iterator end() const {
		return m_rb_tree_.end();
	}

	size_type size() const {
		return m_rb_tree_.size();
	}

	bool empty() const {
		return m_rb_tree_.empty();
	}

	stl::Pair<iterator, bool> insert(const value_type &value) {
		return m_rb_tree_.insert_unique(value);
	}

	stl::Pair<iterator, bool> insert(value_type &&value) {
		return m_rb_tree_.insert_unique(std::move(value));
	}

	template <typename InputIterator>
	void insert(InputIterator first, InputIterator last) {
		for(;first != last;++first) {
			m_rb_tree_.insert_unique(*first);
		}
	}

	template <typename... Args>
	stl::Pair<iterator, bool> emplace(Args&&... args) {
		return m_rb_tree_.emplace_unique(std::forward<Args>(args)...);
	}

	iterator erase(iterator pos) {
		return m_rb_tree_.erase(pos);
	}

	iterator erase(iterator first, iterator last) {
		return m_rb_tree_.erase(first, last);
	}

	size_type erase(const key_type &key) {
		return m_rb_tree_.erase_unique(key);
	}

	void swap(Set &other) {
		m_rb_tree_.swap(other.m_rb_tree_);
	}

	iterator find(const key_type &key) const {
		return m_rb_tree_.find(key);
	}

	size_type count(const key_type &key) const {
		return find(key) == end() ? 0 : 1;
	}

	iterator lower_bound(const key_type &key) const {
		return m_rb_tree_.lower_bound(key);
	}

	iterator upper_bound(const key_type &key) const {
		return m_rb_tree_.upper_bound(key);
	}
}; // class Map


template <typename T, typename Compare = stl::less<T>,
	typename ALLOC = stl::Allocator<T>
>
class MultiSet {
public:
	using key_type = T;
	using value_type = T;
	using size_type = ::size_t;
	using difference_type = ::ptrdiff_t;
	using key_compare = Compare;
	using allocator_type = ALLOC;
	using reference = value_type &;
	using const_reference = const value_type &;
	using pointer = value_type *;
	using const_pointer = const value_type *;

	using iterator = RBIterator<value_type>;
	using const_iterator = const iterator;
private:
	RBTree<key_type, value_type, Identity<value_type>, Compare, ALLOC> m_rb_tree_;
public:
	explicit MultiSet(const Compare &comp = Compare()) :m_rb_tree_(comp) {
	}

	MultiSet(const MultiSet &other) :m_rb_tree_(other.m_rb_tree_) {
	}

	MultiSet(MultiSet &&other) :m_rb_tree_(std::move(other.m_rb_tree_)) {
	}

	template <typename InputIterator>
	MultiSet(InputIterator first, InputIterator last, const Compare &comp = Compare()) :
		MultiSet(comp) {
		insert(first, last);
	}

	MultiSet &operator=(const MultiSet &other) {
		if(this != &other) {
			m_rb_tree_ = other.m_rb_tree_;
		}
		return *this;
	}

	MultiSet &operator=(MultiSet &&other) {
		if(this != &other) {
			m_rb_tree_ = std::move(other.m_rb_tree_);
		}
		return *this;
	}

	void clear() {
		m_rb_tree_.clear();
	}

	const_iterator begin() const {
		return m_rb_tree_.begin();
	}

	const_iterator end() const {
		return m_rb_tree_.end();
	}

	size_type size() const {
		return m_rb_tree_.size();
	}

	bool empty() const {
		return m_rb_tree_.empty();
	}

	stl::Pair<iterator, bool> insert(const value_type &value) {
		return m_rb_tree_.insert_equal(value);
	}

	stl::Pair<iterator, bool> insert(value_type &&value) {
		return m_rb_tree_.insert_equal(std::move(value));
	}

	template <typename InputIterator>
	void insert(InputIterator first, InputIterator last) {
		for(;first != last;++first) {
			m_rb_tree_.insert_equal(*first);
		}
	}

	template <typename... Args>
	stl::Pair<iterator, bool> emplace(Args&&... args) {
		return m_rb_tree_.emplace_equal(std::forward<Args>(args)...);
	}

	iterator erase(iterator pos) {
		return m_rb_tree_.erase(pos);
	}

	iterator erase(iterator first, iterator last) {
		return m_rb_tree_.erase(first, last);
	}

	size_type erase(const key_type &key) {
		return m_rb_tree_.erase_equal(key);
	}

	void swap(MultiSet &other) {
		m_rb_tree_.swap(other.m_rb_tree_);
	}

	iterator find(const key_type &key) const {
		return m_rb_tree_.find(key);
	}

	size_type count(const key_type &key) const {
		auto p = find(key);
		if(p == end()) {
			return 0;
		}
		auto q = upper_bound(key);
		return distance(p, q);
	}

	iterator lower_bound(const key_type &key) const {
		return m_rb_tree_.lower_bound(key);
	}

	iterator upper_bound(const key_type &key) const {
		return m_rb_tree_.upper_bound(key);
	}
}; // class MultiSet

} // namespace stl

#endif // _SET_HPP__