#ifndef _UNORDERED_SET_HPP__
#define _UNORDERED_SET_HPP__

#include "hashtable.hpp"
#include "functional.hpp"
#include "allocator.hpp"

namespace stl {

template<typename Key,
	typename Hash = stl::stlHash<Key>,
	typename KeyEqual = stl::equal_to<Key>,
	typename ALLOC = stl::Allocator<Key>
>
class UnorderedSet {
private:
	using hash_table_type = HashTable<Key, Key, Identity<Key>, Hash,
		modulus<size_t>, KeyEqual, ALLOC>;
public:
	using key_type = Key;
	using value_type = Key;
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
	UnorderedSet() :ht() {
	}

	UnorderedSet(const UnorderedSet &other) :ht(other) {
	}

	UnorderedSet(UnorderedSet &&other) :ht(stl::move(other)) {
	}

	UnorderedSet &operator=(const UnorderedSet &other) {
		if(this != &other) {
			*this = other;
		}
		return *this;
	}

	UnorderedSet &operator=(UnorderedSet &&other) {
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

	void swap(UnorderedSet &s) {
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
}; // class UnorderedSet

template<typename Key,
	typename Hash = stl::stlHash<Key>,
	typename KeyEqual = stl::equal_to<Key>,
	typename ALLOC = stl::Allocator<Key>
>
class UnorderedMultiSet {
private:
	using hash_table_type = HashTable<Key, Key, Identity<Key>, Hash,
		modulus<size_t>, KeyEqual, ALLOC>;
public:
	using key_type = Key;
	using value_type = Key;
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
	UnorderedMultiSet() :ht() {
	}

	UnorderedMultiSet(const UnorderedMultiSet &other) :ht(other) {
	}

	UnorderedMultiSet(UnorderedMultiSet &&other) :ht(stl::move(other)) {
	}

	UnorderedMultiSet &operator=(const UnorderedMultiSet &other) {
		if(this != &other) {
			*this = other;
		}
		return *this;
	}

	UnorderedMultiSet &operator=(UnorderedMultiSet &&other) {
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

	void swap(UnorderedMultiSet &s) {
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
}; // class UnorderedMultiSet

} // namespace stl


#endif // _UNORDERED_SET_HPP__