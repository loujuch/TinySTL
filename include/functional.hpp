#ifndef _FUNCTIONAL_HPP__
#define _FUNCTIONAL_HPP__

#include <stdint.h>

namespace stl {

template <typename Arg, typename Res>
struct UnaryFunction {
public:
	using argument_type = Arg;
	using result_type = Res;
}; // struct UnaryFunction

template <typename Arg1, typename Arg2, typename Res>
struct BinaryFunction {
public:
	using first_argument_type = Arg1;
	using second_argument_type = Arg2;
	using result_type = Res;
}; // struct BinaryFunction

template <typename T>
struct equal_to :public BinaryFunction<T, T, bool> {
	bool operator()(const T &a, const T &b) const {
		return a == b;
	}
}; // struct equal_to

template <typename T>
struct not_equal_to :public BinaryFunction<T, T, bool> {
	bool operator()(const T &a, const T &b) const {
		return a != b;
	}
}; // struct not_equal_to

template <typename T>
struct less :public BinaryFunction<T, T, bool> {
	bool operator()(const T &a, const T &b) const {
		return a < b;
	}
}; // struct less

template <typename T>
struct less_equal :public BinaryFunction<T, T, bool> {
	bool operator()(const T &a, const T &b) const {
		return a <= b;
	}
}; // struct less_equal

template <typename T>
struct greater :public BinaryFunction<T, T, bool> {
	bool operator()(const T &a, const T &b) const {
		return a > b;
	}
}; // struct greater

template <typename T>
struct greater_equal :public BinaryFunction<T, T, bool> {
	bool operator()(const T &a, const T &b) const {
		return a >= b;
	}
}; // struct greater_equal

template <typename T>
struct Identity :public UnaryFunction<T, T> {
	T operator()(const T &t) const {
		return t;
	}
};

template <typename T>
struct plus :public BinaryFunction<T, T, T> {
	T operator()(const T &t1, const T &t2) const {
		return t1 + t2;
	}
};

template <typename T>
struct minus :public BinaryFunction<T, T, T> {
	T operator()(const T &t1, const T &t2) const {
		return t1 - t2;
	}
};

template <typename T>
struct multiplies :public BinaryFunction<T, T, T> {
	T operator()(const T &t1, const T &t2) const {
		return t1 * t2;
	}
};

template <typename T>
struct divides :public BinaryFunction<T, T, T> {
	T operator()(const T &t1, const T &t2) const {
		return t1 / t2;
	}
};

template <typename T>
struct modulus :public BinaryFunction<T, T, T> {
	T operator()(const T &t1, const T &t2) const {
		return t1 % t2;
	}
};

template <typename T>
struct negate :public UnaryFunction<T, T> {
	T operator()(const T &t) const {
		return -t;
	}
};

template <typename Pred>
class unary_negate :public UnaryFunction<typename Pred::argument_type, bool> {
protected:
	Pred m_pred_;
public:
	explicit unary_negate(const Pred &pred) :m_pred_(pred) {
	}

	bool operator()(const typename Pred::argument_type &arg) {
		return !m_pred_(arg);
	}
}; // class unary_negate

template <typename Pred>
unary_negate<Pred> not1(const Pred &pred) {
	return unary_negate<Pred>(pred);
}

template <typename Pred>
class binary_negate :public BinaryFunction<typename Pred::first_argument_type,
	typename Pred::second_argument_type, bool> {
protected:
	Pred m_pred_;
public:
	explicit binary_negate(const Pred &pred) :m_pred_(pred) {
	}

	bool operator()(const typename Pred::first_argument_type &arg1,
		const typename Pred::second_argument_type &arg2) {
		return !m_pred_(arg1, arg2);
	}
}; // class binary_negate

template <typename Pred>
binary_negate<Pred> not2(const Pred &pred) {
	return binary_negate<Pred>(pred);
}

template <typename Key>
struct stlHash {
	size_t operator()(const Key &key) {
		return key.hash();
	}
};

template <>
struct stlHash<char> {
	size_t operator()(char key) {
		size_t value = key;
		value = value | (value << 8);
		value = value | (value << 16);
		if(sizeof(size_t) == 64) {
			value = value | (value << 32);
		}
		return value;
	}
};

template <>
struct stlHash<int16_t> {
	size_t operator()(int16_t key) {
		size_t value = key;
		value = value | (value << 16);
		if(sizeof(size_t) == 64) {
			value = value | (value << 32);
		}
		return value;
	}
};

template <>
struct stlHash<uint16_t> {
	size_t operator()(uint16_t key) {
		size_t value = key;
		value = value | (value << 16);
		if(sizeof(size_t) == 64) {
			value = value | (value << 32);
		}
		return value;
	}
};

template <>
struct stlHash<int32_t> {
	size_t operator()(int32_t key) {
		size_t value = key;
		if(sizeof(size_t) == 64) {
			value = value | (value << 32);
		}
		return value;
	}
};

template <>
struct stlHash<uint32_t> {
	size_t operator()(uint32_t key) {
		size_t value = key;
		if(sizeof(size_t) == 64) {
			value = value | (value << 32);
		}
		return value;
	}
};

template <>
struct stlHash<int64_t> {
	size_t operator()(int64_t key) {
		size_t value = key;
		return value;
	}
};

template <>
struct stlHash<uint64_t> {
	size_t operator()(uint64_t key) {
		size_t value = key;
		return value;
	}
};

} // namespace stl

#endif // _FUNCTIONAL_HPP__