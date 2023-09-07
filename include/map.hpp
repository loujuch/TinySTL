#ifndef _MAP_HPP__
#define _MAP_HPP__

#include "functional.hpp"
#include "allocator.hpp"
#include "rb_tree.hpp"
#include "utility"

namespace stl {

template <typename Key, typename Value, typename Compare = stl::less<Key>,
	typename ALLOC = stl::Allocator<stl::Pair<const Key, Value>>
>
class Map {
public:
	using key_type = Key;
	using mapped_type = Value;
	using value_type = stl::Pair<const Key, Value>;
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
	using reverse_iterator = ReverseIterator<iterator>;
	using const_reverse_iterator = const ReverseIterator<iterator>;
private:
	struct map_comp_key :public UnqryFunction<value_type, key_type> {
		key_type operator()(const value_type &l) const {
			return l.first;
		}
	};
private:
	RBTree<key_type, value_type, map_comp_key, Compare, ALLOC> m_rb_tree_;
public:
	explicit Map(const Compare &comp = Compare()) :m_rb_tree_(comp) {
	}

	Map(const Map &other) :m_rb_tree_(other.m_rb_tree_) {
	}

	Map(Map &&other) :m_rb_tree_(std::move(other.m_rb_tree_)) {
	}

	template <typename InputIterator>
	Map(InputIterator first, InputIterator last, const Compare &comp = Compare()) :
		Map(comp) {
		insert(first, last);
	}

	Map &operator=(const Map &other) {
		if(this != &other) {
			m_rb_tree_ = other.m_rb_tree_;
		}
		return *this;
	}

	Map &operator=(Map &&other) {
		if(this != &other) {
			m_rb_tree_ = std::move(other.m_rb_tree_);
		}
		return *this;
	}

	void clear() {
		m_rb_tree_.clear();
	}

	mapped_type &at(const Key &key) {
		iterator tmp = find(key);
		if(tmp == end()) {
			auto t = emplace(key, mapped_type());
			tmp = t.first;
		}
		return (*tmp).second;
	}

	mapped_type &operator[](const Key &key) {
		return at(key);
	}

	inline iterator begin() {
		return m_rb_tree_.begin();
	}

	inline const_iterator begin() const {
		return m_rb_tree_.begin();
	}

	inline reverse_iterator rbegin() const {
		return m_rb_tree_.rbegin();
	}

	inline reverse_iterator rend() const {
		return m_rb_tree_.rend();
	}

	iterator end() {
		return m_rb_tree_.end();
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

	stl::Pair<iterator, bool> insert(const stl::Pair<Key, Value> &value) {
		return m_rb_tree_.emplace_unique(value.first, value.second);
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

	size_type erase(const Key &key) {
		return m_rb_tree_.erase_unique(key);
	}

	void swap(Map &other) {
		m_rb_tree_.swap(other.m_rb_tree_);
	}

	iterator find(const Key &key) const {
		return m_rb_tree_.find(key);
	}

	size_type count(const Key &key) const {
		return find(key) == end() ? 0 : 1;
	}

	iterator lower_bound(const Key &key) const {
		return m_rb_tree_.lower_bound(key);
	}

	iterator upper_bound(const Key &key) const {
		return m_rb_tree_.upper_bound(key);
	}
}; // class Map


template <typename Key, typename Value, typename Compare = stl::less<Key>,
	typename ALLOC = stl::Allocator<stl::Pair<const Key, Value>>
>
class MultiMap {
public:
	using key_type = Key;
	using mapped_type = Value;
	using value_type = stl::Pair<const Key, Value>;
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
	using reverse_iterator = ReverseIterator<iterator>;
	using const_reverse_iterator = const ReverseIterator<iterator>;
private:
	struct map_comp_key :public UnqryFunction<value_type, key_type> {
		key_type operator()(const value_type &l) const {
			return l.first;
		}
	};
private:
	RBTree<key_type, value_type, map_comp_key, Compare, ALLOC> m_rb_tree_;
public:
	explicit MultiMap(const Compare &comp = Compare()) :m_rb_tree_(comp) {
	}

	MultiMap(const MultiMap &other) :m_rb_tree_(other.m_rb_tree_) {
	}

	MultiMap(MultiMap &&other) :m_rb_tree_(std::move(other.m_rb_tree_)) {
	}

	template <typename InputIterator>
	MultiMap(InputIterator first, InputIterator last, const Compare &comp = Compare()) :
		MultiMap(comp) {
		insert(first, last);
	}

	MultiMap &operator=(const MultiMap &other) {
		if(this != &other) {
			m_rb_tree_ = other.m_rb_tree_;
		}
		return *this;
	}

	MultiMap &operator=(MultiMap &&other) {
		if(this != &other) {
			m_rb_tree_ = std::move(other.m_rb_tree_);
		}
		return *this;
	}

	void clear() {
		m_rb_tree_.clear();
	}

	inline iterator begin() {
		return m_rb_tree_.begin();
	}

	inline const_iterator begin() const {
		return m_rb_tree_.begin();
	}

	inline reverse_iterator rbegin() const {
		return m_rb_tree_.rbegin();
	}

	inline reverse_iterator rend() const {
		return m_rb_tree_.rend();
	}

	iterator end() {
		return m_rb_tree_.end();
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

	stl::Pair<iterator, bool> insert(const stl::Pair<Key, Value> &value) {
		return m_rb_tree_.emplace_equal(value.first, value.second);
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

	size_type erase(const Key &key) {
		return m_rb_tree_.erase_equal(key);
	}

	void swap(MultiMap &other) {
		m_rb_tree_.swap(other.m_rb_tree_);
	}

	iterator find(const Key &key) const {
		return m_rb_tree_.find(key);
	}

	size_type count(const Key &key) const {
		auto p = find(key);
		if(p == end()) {
			return 0;
		}
		auto q = upper_bound(key);
		return distance(p, q);
	}

	iterator lower_bound(const Key &key) const {
		return m_rb_tree_.lower_bound(key);
	}

	iterator upper_bound(const Key &key) const {
		return m_rb_tree_.upper_bound(key);
	}
}; // class MultiMap

} // namespace stl

#endif // _MAP_HPP__