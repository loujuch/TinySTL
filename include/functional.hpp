#ifndef _FUNCTIONAL_HPP__
#define _FUNCTIONAL_HPP__

namespace stl {

template <typename Arg, typename Res>
struct UnqryFunction {
	using argument = Arg;
	using result = Res;
}; // struct UnqryFunction

template <typename Arg1, typename Arg2, typename Res>
struct BinaryFunction {
	using first_argument_type = Arg1;
	using second_argument_type = Arg2;
	using result_type = Res;
}; // struct BinaryFunction

template <typename T>
struct equal_to :public BinaryFunction<T, T, bool> {
	bool operator()(const T &a, const T &b) {
		return a == b;
	}
}; // struct equal_to

template <typename T>
struct not_equal_to :public BinaryFunction<T, T, bool> {
	bool operator()(const T &a, const T &b) {
		return a != b;
	}
}; // struct not_equal_to

template <typename T>
struct less :public BinaryFunction<T, T, bool> {
	bool operator()(const T &a, const T &b) {
		return a < b;
	}
}; // struct less

template <typename T>
struct less_equal :public BinaryFunction<T, T, bool> {
	bool operator()(const T &a, const T &b) {
		return a <= b;
	}
}; // struct less_equal

template <typename T>
struct greater :public BinaryFunction<T, T, bool> {
	bool operator()(const T &a, const T &b) {
		return a > b;
	}
}; // struct greater

template <typename T>
struct greater_equal :public BinaryFunction<T, T, bool> {
	bool operator()(const T &a, const T &b) {
		return a >= b;
	}
}; // struct greater_equal

} // namespace stl

#endif // _FUNCTIONAL_HPP__