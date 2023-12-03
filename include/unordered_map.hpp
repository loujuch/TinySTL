#ifndef _UNORDERED_MAP_HPP__
#define _UNORDERED_MAP_HPP__

#include "hashtable.hpp"
#include "utility.hpp"
#include "functional.hpp"

namespace stl {

template <typename Key, typename Value, typename Hash = stl::stlHash<Key>,
	typename KeyEqual = stl::equal_to<Key>,
	typename ALLOC = stl::Allocator<stl::Pair<const Key, Value>>
> class UnorderedMap {
public:
	using key_type = Key;
	using mapped_type = Value;
	using value_type = stl::Pair<const Key, Value>;
private:
	struct map_comp_key :public UnaryFunction<value_type, key_type> {
		key_type operator()(const value_type &l) const {
			return l.first;
		}
	};

	using hash_table_type = HashTable<key_type, value_type, map_comp_key, Hash,
		modulus<size_t>, KeyEqual, ALLOC>;
public:
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using hasher = Hash;
	using key_equal = KeyEqual;
	using allocator_type = ALLOC;
	using reference = value_type &;
	using const_reference = const value_type &;
	using pointer = value_type *;
	using const_pointer = const value_type *;

	using iterator = typename hash_table_type::iterator;
	using const_iterator = typename hash_table_type::const_iterator;
private:
	hash_table_type ht;
public:
	UnorderedMap() :ht() {
	}

	UnorderedMap(const UnorderedMap &other) :ht(other) {
	}

	UnorderedMap(UnorderedMap &&other) :ht(stl::move(other)) {
	}

	UnorderedMap &operator=(const UnorderedMap &other) {
		if(this != &other) {
			*this = other;
		}
		return *this;
	}

	UnorderedMap &operator=(UnorderedMap &&other) {
		if(this != &other) {
			*this = stl::move(other);
		}
		return *this;
	}

	iterator begin() {
		return ht.begin();
	}

	iterator begin() const {
		return ht.begin();
	}

	iterator end() {
		return ht.end();
	}

	iterator end() const {
		return ht.end();
	}

	bool empty() const {
		return ht.empty();
	}

	size_type size() const {
		return ht.size();
	}

	void clear() {
		ht.clear();
	}

	stl::Pair<iterator, bool> insert(const value_type &value) {
		return ht.emplace_unique(value);
	}

	stl::Pair<iterator, bool> insert(value_type &&value) {
		return ht.emplace_unique(stl::move(value));
	}

	template <typename ... Args>
	stl::Pair<iterator, bool> emplace(Args&&... args) {
		return ht.emplace_unique(std::forward<Args>(args)...);
	}

	void swap(UnorderedMap &s) {
		if(this != &s) {
			s.swap(*this);
		}
	}

	iterator erase(iterator pos) {
		return ht.erase(pos);
	}

	iterator erase(const_iterator first, const_iterator last) {
		return ht.erase(first, last);
	}

	size_type erase(const Key &key) {
		return ht.erase(key);
	}

	size_type bucket_count() const {
		return ht.bucket_count();
	}

	iterator find(const Key &key) const {
		return ht.find(key);
	}

	void rehash(size_type n) {
		ht.rehash(n);
	}

	mapped_type at(const Key &key) {
		iterator it = find(key);
		if(it == end()) {
			auto p = emplace(key, Value());
			it = p.first;
		}
		return it->second;
	}

	mapped_type operator[](const Key &key) {
		return at(key);
	}
};

template <typename Key, typename Value, typename Hash = stl::stlHash<Key>,
	typename KeyEqual = stl::equal_to<Key>,
	typename ALLOC = stl::Allocator<stl::Pair<const Key, Value>>
> class UnorderedMultiMap {
public:
	using key_type = Key;
	using mapped_type = Value;
	using value_type = stl::Pair<const Key, Value>;
private:
	struct map_comp_key :public UnaryFunction<value_type, key_type> {
		key_type operator()(const value_type &l) const {
			return l.first;
		}
	};

	using hash_table_type = HashTable<key_type, value_type, map_comp_key, Hash,
		modulus<size_t>, KeyEqual, ALLOC>;
public:
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using hasher = Hash;
	using key_equal = KeyEqual;
	using allocator_type = ALLOC;
	using reference = value_type &;
	using const_reference = const value_type &;
	using pointer = value_type *;
	using const_pointer = const value_type *;

	using iterator = typename hash_table_type::iterator;
	using const_iterator = typename hash_table_type::const_iterator;
private:
	hash_table_type ht;
public:
	UnorderedMultiMap() :ht() {
	}

	UnorderedMultiMap(const UnorderedMultiMap &other) :ht(other) {
	}

	UnorderedMultiMap(UnorderedMultiMap &&other) :ht(stl::move(other)) {
	}

	UnorderedMultiMap &operator=(const UnorderedMultiMap &other) {
		if(this != &other) {
			*this = other;
		}
		return *this;
	}

	UnorderedMultiMap &operator=(UnorderedMultiMap &&other) {
		if(this != &other) {
			*this = stl::move(other);
		}
		return *this;
	}

	iterator begin() {
		return ht.begin();
	}

	iterator begin() const {
		return ht.begin();
	}

	iterator end() {
		return ht.end();
	}

	iterator end() const {
		return ht.end();
	}

	bool empty() const {
		return ht.empty();
	}

	size_type size() const {
		return ht.size();
	}

	void clear() {
		ht.clear();
	}

	iterator insert(const value_type &value) {
		return ht.emplace_equal(value);
	}

	iterator insert(value_type &&value) {
		return ht.emplace_equal(stl::move(value));
	}

	template <typename ... Args>
	iterator emplace(Args&&... args) {
		return ht.emplace_equal(std::forward<Args>(args)...);
	}

	void swap(UnorderedMultiMap &s) {
		if(this != &s) {
			s.swap(*this);
		}
	}

	iterator erase(iterator pos) {
		return ht.erase(pos);
	}

	iterator erase(const_iterator first, const_iterator last) {
		return ht.erase(first, last);
	}

	size_type erase(const Key &key) {
		return ht.erase(key);
	}

	size_type bucket_count() const {
		return ht.bucket_count();
	}

	iterator find(const Key &key) const {
		return ht.find(key);
	}

	void rehash(size_type n) {
		ht.rehash(n);
	}
};

} // namespace stl

#endif // _UNORDERED_MAP_HPP__