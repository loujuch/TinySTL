#ifndef _HASH_TABLE_HPP__
#define _HASH_TABLE_HPP__

#include <iostream>

#include "functional.hpp"
#include "allocator.hpp"
#include "utility.hpp"
#include "iterator.hpp"
#include "algobase.hpp"
#include "uninitialized.hpp"
#include "assert.h"

#include "unordered_set"

namespace stl {

template <typename Value, typename KeyOfValue, typename Hash>
struct HashTableListNode {
	Value m_value;
	size_t m_hash_cache;
	HashTableListNode<Value, KeyOfValue, Hash> *m_next;
public:
	HashTableListNode() :m_value(), m_hash_cache(Hash()(KeyOfValue()(m_value))),
		m_next(nullptr) {
	}

	HashTableListNode(const Value &t) :m_value(t), m_hash_cache(Hash()(KeyOfValue()(m_value))),
		m_next(nullptr) {
	}

	HashTableListNode(Value &&t) :m_value(stl::move(t)), m_hash_cache(Hash()(KeyOfValue()(m_value))),
		m_next(nullptr) {
	}

	template <typename ... Args>
	HashTableListNode(Args&& ... args) :
		m_value(std::forward<Args>(args)...), m_hash_cache(Hash()(KeyOfValue()(m_value))),
		m_next(nullptr) {
	}
}; // class HashTableListNode

template <typename Value, typename KeyOfValue, typename Hash>
class HashTableIterator :public Iterator<forward_iterator_tag, Value> {
public:
	template <typename Key, typename Value1, typename KeyOfValue1, typename Hash1,
		typename H2, typename KeyEqual, typename ALLOC
	>
	friend class HashTable;
public:
	using pointer = HashTableListNode<Value, KeyOfValue, Hash> *;
	using map_pointer = pointer *;
private:
	map_pointer m_slot_;
	map_pointer m_slot_limit_;
	pointer m_ptr_;
public:
	explicit HashTableIterator(pointer ptr, map_pointer slot, map_pointer slot_limit) :
		m_ptr_(ptr), m_slot_(slot), m_slot_limit_(slot_limit) {
	}

	inline HashTableIterator<Value, KeyOfValue, Hash> &operator++() {
		m_ptr_ = m_ptr_->m_next;
		while(m_slot_ != m_slot_limit_ && m_ptr_ == nullptr) {
			++m_slot_;
			m_ptr_ = m_slot_ != m_slot_limit_ ? *m_slot_ : nullptr;
		}
		return *this;
	}

	inline HashTableIterator<Value, KeyOfValue, Hash> operator++(int) {
		auto out = *this;
		m_ptr_ = m_ptr_->m_next;
		while(m_slot_ != m_slot_limit_ && m_ptr_ == nullptr) {
			++m_slot_;
			m_ptr_ = m_slot_ != m_slot_limit_ ? *m_slot_ : nullptr;
		}
		return out;
	}

	inline Value &operator *() const {
		return m_ptr_->m_value;
	}

	inline Value *operator->() const {
		return &(m_ptr_->m_value);
	}

	inline pointer base() const {
		return m_ptr_;
	}

	inline bool operator==(const HashTableIterator<Value, KeyOfValue, Hash> &i) const {
		return m_ptr_ == i.m_ptr_;
	}

	inline bool operator!=(const HashTableIterator<Value, KeyOfValue, Hash> &i) const {
		return m_ptr_ != i.m_ptr_;
	}
}; // class HashTableIterator


template <typename Key, typename Value, typename KeyOfValue, typename Hash,
	typename H2 = stl::modulus<size_t>, typename KeyEqual = stl::equal_to<Key>,
	typename ALLOC = Allocator<Value>
>
class HashTable {
public:
	using key_type = Key;
	using value_type = Value;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using hasher = Hash;
	using key_equal = KeyEqual;
	using allocator_type = ALLOC;
	using reference = value_type &;
	using const_reference = const value_type &;
	using pointer = value_type *;
	using const_pointer = const value_type *;

	using iterator = HashTableIterator<Value, KeyOfValue, Hash>;
	using const_iterator = const HashTableIterator<Value, KeyOfValue, Hash>;
private:
	static constexpr int num_primes = 28;
private:
	using link_type = HashTableListNode<Value, KeyOfValue, Hash> *;
	using map_type = link_type *;
	using self = HashTable<Key, Value, KeyOfValue, Hash, H2, KeyEqual, ALLOC>;

	typename ALLOC::template rebind<HashTableListNode<Value,
		KeyOfValue, Hash>>::other m_node_allocator_;
	typename ALLOC::template rebind<HashTableListNode<Value,
		KeyOfValue, Hash> *>::other m_node_ptr_allocator_;

	size_type m_size_;
	int m_map_size_index_;

	map_type m_map_;

	iterator m_head_;
	iterator m_tail_;
private:
	template <typename ... Args>
	link_type __alloc_a_link_node(Args&&...args) {
		link_type res = m_node_allocator_.allocate(1);
		m_node_allocator_.construct(res, std::forward<Args>(args)...);
		return res;
	}

	void __dealloc_a_link_node(link_type node) {
		m_node_allocator_.destory(node);
		m_node_allocator_.deallocate(node, 1);
	}

	map_type __get_a_map_with(size_type n) {
		map_type res = m_node_ptr_allocator_.allocate(n);
		uninitialized_fill_n(nullptr, res, n, m_node_ptr_allocator_);
		return res;
	}

	void __destory_map(map_type p, size_type n) {
		m_node_ptr_allocator_.deallocate(p, n);
	}

	link_type __destory_a_link(link_type p) {
		if(p != nullptr) {
			__destory_a_link(p->m_next);
			__dealloc_a_link_node(p);
		}
		return nullptr;
	}

	link_type __copy_a_link(link_type p) {
		if(p == nullptr) {
			return nullptr;
		}
		link_type head = __alloc_a_link_node(p->m_value);
		head->m_next = __copy_a_link(p->m_next);
		return head;
	}

	size_type __get_slot(link_type p) {
		return H2()(p->m_hash_cache, bucket_count());
	}

	// 返回a是否应该在b前面
	bool __is_front_node(link_type a, link_type b) {
		if(a->m_hash_cache < b->m_hash_cache) {
			return true;
		}
		if(a->m_hash_cache > b->m_hash_cache) {
			return false;
		}
		return KeyEqual()(KeyOfValue()(a->m_value), KeyOfValue()(b->m_value));
	}

	// 返回插入点前一个结点，若需要头插则返回nullptr
	link_type __find_insert_pos(link_type insert_obj, link_type slot) {
		link_type p = slot;
		if(__is_front_node(insert_obj, p)) {
			// insert_obj 应在 p 前面
			return nullptr;
		}
		while(p->m_next != nullptr && !__is_front_node(insert_obj, p->m_next)) {
			// insert_obj 在 p->next 后面
			p = p->m_next;
		}
		// insert_obj 在 p 后面 和 p->next 前面
		return p;
	}

	void __insert_to_slot(link_type insert_obj, link_type &slot) {
		bool is_head = true;

		if(slot == nullptr) {
			// slot为空，直接插入
			slot = insert_obj;
			insert_obj->m_next = nullptr;
		} else {
			link_type pos = __find_insert_pos(insert_obj, slot);
			if(pos == nullptr) {
				insert_obj->m_next = slot;
				slot = insert_obj;
			} else {
				insert_obj->m_next = pos->m_next;
				pos->m_next = insert_obj;
				is_head = false;
			}
		}

		if(is_head && (m_head_.base() == nullptr || &slot <= m_head_.m_slot_)) {
			m_head_ = iterator(insert_obj, &slot, m_map_ + bucket_count());
		}

		++m_size_;
	}
public:
	HashTable() : m_size_(0), m_map_size_index_(0), m_map_(__get_a_map_with(bucket_count())),
		m_head_(nullptr, m_map_, m_map_ + bucket_count()), m_tail_(m_head_) {
	}

	HashTable(const self &ht) :m_size_(ht.size()), m_map_size_index_(ht.m_map_size_index_),
		m_map_(__get_a_map_with(bucket_count())),
		m_head_(nullptr, m_map_, m_map_ + bucket_count()), m_tail_(m_head_) {
		for(size_type i = 0;i < bucket_count();++i) {
			m_map_[i] = __copy_a_link(ht.m_map_[i]);
			if(m_head_.base() == nullptr && m_map_[i] != nullptr) {
				m_head_ = iterator(m_map_[i], &m_map_[i], m_map_ + bucket_count());
			}
		}
	}

	HashTable(self &&ht) :m_size_(ht.m_size_), m_map_size_index_(ht.m_map_size_index_),
		m_map_(ht.m_map_), m_head_(ht.m_head_), m_tail_(ht.m_tail_) {
		ht.m_size_ = 0;
		m_map_size_index_ = 0;
		ht.m_map_ = ht.__get_a_map_with(ht.bucket_count());
		m_head_ = m_tail_ = iterator(nullptr, ht.m_map_, ht.m_map_ + ht.bucket_count());
	}

	self &operator=(const self &ht) {
		if(this != &ht) {
			clear();

			__destory_map(m_map_, bucket_count());

			m_size_ = ht.m_size_;
			m_map_size_index_ = ht.m_map_size_index_;
			m_map_ = __get_a_map_with(bucket_count());
			m_head_ = m_tail_ = iterator(nullptr, m_map_, m_map_ + bucket_count());

			for(size_type i = 0;i < bucket_count();++i) {
				m_map_[i] = __copy_a_link(ht.m_map_[i]);
				if(m_head_.base() == nullptr && m_map_[i] != nullptr) {
					m_head_ = iterator(m_map_[i], &m_map_[i], m_map_ + bucket_count());
				}
			}
		}
		return *this;
	}

	self &operator=(self &&ht) {
		if(this != &ht) {
			clear();

			map_type p = m_map_;

			m_size_ = ht.m_size_;
			m_map_size_index_ = ht.m_map_size_index_;
			m_map_ = ht.m_map_;
			m_head_ = ht.m_head_;
			m_tail_ = ht.m_tail_;

			ht.m_size_ = 0;
			m_map_size_index_ = 0;
			ht.m_map_ = p;
			m_head_ = m_tail_ = iterator(nullptr, ht.m_map_, ht.m_map_ + ht.bucket_count());
		}
		return *this;
	}

	~HashTable() {
		clear();
		__destory_map(m_map_, bucket_count());
	}

	void clear() {
		// 解构链表
		for(size_type i = 0;i < bucket_count();++i) {
			m_map_[i] = __destory_a_link(m_map_[i]);
		}

		// 重设m_map_
		if(m_map_size_index_ > 0) {
			__destory_map(m_map_, bucket_count());
			m_map_size_index_ = 0;
			m_map_ = __get_a_map_with(bucket_count());
		}

		// 重设迭代器
		m_head_ = m_tail_ = iterator(nullptr, m_map_, m_map_ + bucket_count());
		m_size_ = 0;
	}

	void swap(self &ht) {
		stl::swap(*this, ht);
	}

	inline size_type size() const {
		return m_size_;
	}

	inline bool empty() const {
		return m_size_ == 0;
	}

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

	iterator find(const Key &key) const {
		size_type hash = Hash()(key);
		size_type slot = H2()(hash, bucket_count());
		link_type l = m_map_[slot];
		while(l != nullptr) {
			if(hash == l->m_hash_cache) {
				// 找到相等的
				if(KeyEqual()(KeyOfValue()(l->m_value), key)) {
					return iterator(l, m_map_, m_map_ + bucket_count());
				}
			} else if(hash < l->m_hash_cache) {
				// hash过小，未查到
				break;
			}
			l = l->m_next;
		}
		return end();
	}

	size_type count(const Key &key) const {
		iterator it = find(key);
		if(it == end()) {
			return 0;
		}
		size_type n = 1;
		link_type p = it.base()->m_next;
		size_type hash = it.base()->m_hash_cache;
		while(true) {
			if(p == nullptr || p->m_hash_cache != hash) {
				break;
			}
			if(!KeyEqual()(key, KeyOfValue()(p->m_value))) {
				break;
			}
			p = p->m_next;
			++n;
		}
		return n;
	}

	iterator erase(const_iterator pos) {
		link_type ipos = pos.base();
		iterator it = pos;
		++it;
		size_type slot = __get_slot(ipos);
		if(m_map_[slot] == ipos) {
			m_map_[slot] = ipos->m_next;
			__dealloc_a_link_node(ipos);
		} else {
			link_type p = m_map_[slot];
			while(p != nullptr && p->m_next != ipos) {
				p = p->m_next;
			}
			if(p != nullptr) {
				p->m_next = ipos->m_next;
				__dealloc_a_link_node(ipos);
			}
		}
		--m_size_;
		return it;
	}

	iterator erase(const_iterator first, const_iterator last) {
		while(first != last) {
			erase(first);
			++first;
		}
		return last;
	}

	size_type erase(const Key &key) {
		iterator it = find(key);
		if(it == end()) {
			return 0;
		}
		size_type n = 1;
		link_type p = it.base();
		size_type hash = p->m_hash_cache;
		map_type slot = &m_map_[__get_slot(p)];
		map_type limit = m_map_ + bucket_count();
		p = p->m_next;
		erase(it);
		while(true) {
			if(p == nullptr || p->m_hash_cache != hash) {
				break;
			}
			if(!KeyEqual()(key, KeyOfValue()(p->m_value))) {
				break;
			}
			link_type q = p->m_next;
			erase(iterator(p, slot, limit));
			p = q;
			++n;
		}
		return n;
	}

	template <typename ... Args>
	stl::Pair<iterator, bool> emplace_unique(Args&& ... args) {
		// 创建插入结点
		link_type ipos = __alloc_a_link_node(std::forward<Args>(args)...);

		auto iter = find(KeyOfValue()(ipos->m_value));
		if(iter != end()) {
			__dealloc_a_link_node(ipos);
			return stl::Pair<iterator, bool>(iter, false);
		}

		if(m_size_ == bucket_count()) {
			// 元素过多，重新hash
			rehash(bucket_count() + 1);
		}

		// 查找插入点
		size_type h2 = __get_slot(ipos);

		// 进行插入
		__insert_to_slot(ipos, m_map_[h2]);

		return stl::Pair<iterator, bool>(iterator(ipos, &m_map_[h2], m_map_ + bucket_count()), true);
	}

	template <typename ... Args>
	iterator emplace_equal(Args&& ... args) {
		if(m_size_ == bucket_count()) {
			// 元素过多，重新hash
			rehash(bucket_count() + 1);
		}

		// 创建插入结点
		link_type ipos = __alloc_a_link_node(std::forward<Args>(args)...);

		// 查找插入点
		size_type h2 = __get_slot(ipos);

		// 进行插入
		__insert_to_slot(ipos, m_map_[h2]);

		return iterator(ipos, &m_map_[h2], m_map_ + bucket_count());
	}

	void rehash(size_type count) {
		if(count <= bucket_count()) {
			// count 过小
			return;
		}
		// 找到第一个大于等于 count 的 map 值
		size_type bucket = bucket_count();

		for(m_map_size_index_ += 1;m_map_size_index_ < num_primes;++m_map_size_index_) {
			if(count <= bucket_count()) {
				break;
			}
		}

		assert(m_map_size_index_ < num_primes);

		size_type s = m_size_;

		// 新的 map
		iterator iter = begin();

		map_type old_map = m_map_;
		m_map_ = __get_a_map_with(bucket_count());
		m_head_ = m_tail_;

		// 将其装入新桶中，每个链按hash值小到大排列
		while(iter != end()) {
			// 获取当前链
			link_type lt = iter.base();
			// 指向下一链
			++iter;
			// 获取hash和二层hash
			size_type h2 = __get_slot(lt);

			// 进行插入
			__insert_to_slot(lt, m_map_[h2]);
		}

		// 消除旧map
		__destory_map(old_map, bucket);

		m_size_ = s;
	}

	size_type bucket_count() const {
		static const size_type prime_list[num_primes] = {
			53ul, 97ul, 193ul, 389ul, 769ul,
			1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
			49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
			1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
			50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};
		return prime_list[m_map_size_index_];
	}
}; // class HashTable

} // namespace stl


#endif // _HASH_TABLE_HPP__