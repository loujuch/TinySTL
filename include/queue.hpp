#ifndef _QUEUE_HPP__
#define _QUEUE_HPP__

#include "deque.hpp"

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

	bool size() const {
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
}; // class Stack

} // namespace slk


#endif // _QUEUE_HPP__