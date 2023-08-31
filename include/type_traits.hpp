#ifndef _TYPE_TRAITS_HPP__
#define _TYPE_TRAITS_HPP__

#include <stdint.h>

namespace stl {

template<bool, typename Ta, typename Tb>
struct IfThenElse;

template<typename Ta, typename Tb>
struct IfThenElse<true, Ta, Tb> {
	using result = Ta;
}; // struct IfThenElse<true, Ta, Tb>

template<typename Ta, typename Tb>
struct IfThenElse<false, Ta, Tb> {
	using result = Tb;
}; // struct IfThenElse<false, Ta, Tb>

struct true_type {};
struct false_type {};

template<typename T>
struct type_traits {
	using has_trivial_default_constructor = false_type;
	using has_trivial_copy_constructor = false_type;
	using has_trivial_assignment_operator = false_type;
	using has_trivial_destructor = false_type;
	using is_POD_type = false_type;
}; // struct type_traits<T>

template<>
struct type_traits<bool> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<bool>

template<>
struct type_traits<char> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<char>

template<>
struct type_traits<unsigned char> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<unsigned char>

template<>
struct type_traits<signed char> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<signed char>

template<>
struct type_traits<wchar_t> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<wchar_t>

template<>
struct type_traits<int16_t> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<int16_t>

template<>
struct type_traits<uint16_t> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<uint16_t>

template<>
struct type_traits<int32_t> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<int32_t>

template<>
struct type_traits<uint32_t> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<uint32_t>

template<>
struct type_traits<int64_t> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<int64_t>

template<>
struct type_traits<uint64_t> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<uint64_t>

template<>
struct type_traits<float> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<float>

template<>
struct type_traits<double> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<double>

template<>
struct type_traits<long double> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<long double>

template<typename T>
struct type_traits<T *> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<T *>

template<typename T>
struct type_traits<const T *> {
	using has_trivial_default_constructor = true_type;
	using has_trivial_copy_constructor = true_type;
	using has_trivial_assignment_operator = true_type;
	using has_trivial_destructor = true_type;
	using is_POD_type = true_type;
}; // struct type_traits<const T *>

}
#endif // _TYPE_TRAITS_HPP__