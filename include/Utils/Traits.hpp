#ifndef NAND2TETRIS_INCLUDE_UTILS_TRAITS_HPP
#define NAND2TETRIS_INCLUDE_UTILS_TRAITS_HPP

#include <type_traits>
namespace hack
{

template <typename T, typename... Ts>
using is_any_one_of = std::disjunction<std::is_same<T, Ts>...>;

template <typename T, typename... Ts>
constexpr bool is_any_one_of_v = is_any_one_of<T, Ts...>::value;   //NOLINT

}   //namespace hack

#endif