#ifndef NAND2TETRIS_INCLUDE_UTILS_TRAITS_HPP
#define NAND2TETRIS_INCLUDE_UTILS_TRAITS_HPP

#include <type_traits>
namespace hack
{

template <typename T, typename... Ts>
using is_any_one_of = std::disjunction<std::is_same<T, Ts>...>;

template <typename T, typename... Ts>
constexpr bool is_any_one_of_v = is_any_one_of<T, Ts...>::value;   //NOLINT

template <typename T, typename... Ts>
using is_all_of = std::conjunction<std::is_same<T, Ts>...>;

template <typename T, typename... Ts>
constexpr bool is_all_of_v = is_all_of<T, Ts...>::value;   //NOLINT

template <typename T>
struct make_const_pointer {
    using type = typename std::add_pointer<typename std::add_const<T>::type>::type;
};

template <typename T>
struct make_const_reference {
    using type = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
};
}   //namespace hack

#endif