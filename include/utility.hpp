#ifndef _UTILITY_HPP__
#define _UTILITY_HPP__

namespace stl {

template<typename T>
struct remove_reference {
	using type = T;
};

template<typename T>
struct remove_reference<T &> {
	using type = T;
};

template<typename T>
struct remove_reference<T &&> {
	using type = T;
};

template <typename T>
typename remove_reference<T>::type &&move(T &&t) {
	return static_cast<typename remove_reference<T>::type &&>(t);
}

template <typename T1, typename T2>
struct Pair {
	T1 first;
	T2 second;

	explicit Pair(T1 first_ = T1(), T2 second_ = T2()) :first(first_), second(second_) {
	}

	Pair(const Pair &p) :first(p.first), second(p.second) {
	}

	Pair(Pair &&p) :first(move(p.first)), second(move(p.second)) {
	}

	bool operator<(const Pair &p) {
		return first < p.first;
	}
};

} // namespace stl

#endif // _UTILITY_HPP__