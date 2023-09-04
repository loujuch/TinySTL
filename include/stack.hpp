#ifndef _STACK_HPP__
#define _STACK_HPP__

#include "vector.hpp"

namespace stl {

template <typename T, typename Container = Vector<T>>
class Stack {
public:
	using container_type = Container;
	using value_type = typename Container::value_type;
	using size_type = typename Container::size_type;
	using reference = typename Container::reference;
	using const_reference = typename Container::const_reference;
private:
	Container m_container_;
public:
	explicit Stack(const container_type &cont = Container()) :m_container_(cont) {
	}

	explicit Stack(container_type &&cont) :m_container_(std::move(cont)) {
	}

	Stack(const Stack &other) :m_container_(other.m_container_) {
	}

	Stack(Stack &&other) :m_container_(std::move(other.m_container_)) {
	}

	template <typename InputIterator>
	Stack(InputIterator first, InputIterator last) : m_container_(first, last) {
	}

	Stack &operator=(const Stack &other) {
		m_container_ = other.m_container_;
	}

	Stack &operator=(Stack &&other) {
		m_container_ = std::move(other.m_container_);
	}

	reference top() {
		return m_container_.back();
	}

	const_reference top() const {
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
		return m_container_.pop_back();
	}

	template <typename ... Args>
	void emplace(Args&& ... args) {
		return m_container_.emplace_back(std::forward<Args>(args)...);
	}

	void swap(Stack &other) {
		m_container_.swap(other.m_container_);
	}
}; // class Stack

} // namespace stl

#endif // _STACK_HPP__