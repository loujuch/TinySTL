#ifndef _QUEUE_HPP__
#define _QUEUE_HPP__

#include "deque.hpp"
#include "vector.hpp"
#include "functional.hpp"
#include "algorithm.hpp"

namespace stl {

template <typename T, typename Container = Deque<T>>
class Queue {
public:
	using container_type = Container;
	using value_type = typename Container::value_type;
	using size_type = typename Container::size_type;
	using reference = typename Container::reference;
	using const_reference = typename Container::const_reference;
private:
	Container m_container_;
public:
	explicit Queue(const container_type &cont = Container()) :m_container_(cont) {
	}

	explicit Queue(container_type &&cont) :m_container_(std::move(cont)) {
	}

	Queue(const Queue &other) :m_container_(other.m_container_) {
	}

	Queue(Queue &&other) :m_container_(std::move(other.m_container_)) {
	}

	template <typename InputIterator>
	Queue(InputIterator first, InputIterator last) : m_container_(first, last) {
	}

	Queue &operator=(const Queue &other) {
		m_container_ = other.m_container_;
	}

	Queue &operator=(Queue &&other) {
		m_container_ = std::move(other.m_container_);
	}

	reference front() {
		return m_container_.front();
	}

	const_reference front() const {
		return m_container_.front();
	}

	reference back() {
		return m_container_.back();
	}

	const_reference back() const {
		return m_container_.back();
	}

	bool empty() const {
		return m_container_.empty();
	}

	size_type size() const {
		return m_container_.size();
	}

	void push(const value_type &v) {
		return m_container_.push_back(v);
	}

	void push(value_type &&v) {
		return m_container_.push_back(std::move(v));
	}

	void pop() {
		return m_container_.pop_front();
	}

	template <typename ... Args>
	void emplace(Args&& ... args) {
		return m_container_.emplace_back(std::forward<Args>(args)...);
	}

	void swap(Queue &other) {
		m_container_.swap(other.m_container_);
	}
}; // class Queue

template<class T,
	class Container = Vector<T>,
	class Compare = stl::less<typename Container::value_type>
>
class PriorityQueue {
public:
	using container_type = Container;
	using value_type = typename Container::value_type;
	using value_compare = Compare;
	using size_type = typename Container::size_type;
	using reference = typename Container::reference;
	using const_reference = typename Container::const_reference;
private:
	Container m_container_;
	Compare m_comparator_;
public:
	explicit PriorityQueue(const Compare &compare = Compare(),
		const Container &cont = Container()) :m_container_(cont), m_comparator_(compare) {
		make_heap(m_container_.begin(), m_container_.end(), m_comparator_);
	}

	explicit PriorityQueue(const Compare &compare, Container &&cont) :
		m_container_(std::move(cont)), m_comparator_(compare) {
		make_heap(m_container_.begin(), m_container_.end(), m_comparator_);
	}

	PriorityQueue(const PriorityQueue &other) :
		PriorityQueue(other.m_comparator_, other.m_container_) {
	}

	PriorityQueue(PriorityQueue &&other) :
		PriorityQueue(other.m_comparator_, std::move(other.m_container_)) {
	}

	template <typename InputIterator>
	explicit PriorityQueue(InputIterator first, InputIterator last,
		const Compare &compare = Compare()) :
		PriorityQueue(compare, Container(first, last)) {
	}

	PriorityQueue &operator=(const PriorityQueue &other) {
		if(this != &other) {
			m_comparator_ = other.m_comparator_;
			m_container_ = other.m_container_;
		}
		return *this;
	}

	PriorityQueue &operator=(PriorityQueue &&other) {
		if(this != &other) {
			m_comparator_ = other.m_comparator_;
			m_container_ = std::move(other.m_container_);
		}
		return *this;
	}

	const_reference top() const {
		return m_container_.front();
	}

	bool empty() const {
		return m_container_.empty();
	}

	size_type size() const {
		return m_container_.size();
	}

	void push(const T &value) {
		m_container_.push_back(value);
		push_heap(m_container_.begin(), m_container_.end());
	}

	void push(T &&value) {
		m_container_.push_back(std::move(value));
		push_heap(m_container_.begin(), m_container_.end());
	}

	void pop() {
		pop_heap(m_container_.begin(), m_container_.end());
		m_container_.pop_back();
	}

	template <typename ... Args>
	void emplace(Args&& ... args) {
		m_container_.emplace_back(std::forward<Args>(args)...);
		push_heap(m_container_.begin(), m_container_.end());
	}

	void swap(PriorityQueue &other) {
		std::swap(m_comparator_, other.m_comparator_);
		m_container_.swap(other.m_container_);
	}
}; // class PriorityQueue

} // namespace slk


#endif // _QUEUE_HPP__