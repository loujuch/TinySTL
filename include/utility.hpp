#ifndef _UTILITY_HPP__
#define _UTILITY_HPP__

namespace stl {

template <typename T1, typename T2>
struct Pair {
	T1 first;
	T2 second;

	Pair(T1 first_ = T1(), T2 second_ = T2()) :first(first_), second(second_) {
	}

	bool operator<(const Pair &p) {
		return first < p.first;
	}
};


} // namespace stl

#endif // _UTILITY_HPP__