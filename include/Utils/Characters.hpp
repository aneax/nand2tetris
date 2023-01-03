#ifndef NAND2TETRIS_INCLUDE_UTILS_CHARACTERS_HPP
#define NAND2TETRIS_INCLUDE_UTILS_CHARACTERS_HPP

#include "Reader.hpp"
#include <string_view>
namespace hack
{
auto is_white_space(char ch) -> bool;
auto is_alpha(char ch) -> bool;
auto is_digit(char ch) -> bool;
auto is_operator(char ch) -> bool;
auto is_identifier(char ch) -> bool;
auto is_alpha_numeric(char ch) -> bool;
auto is_equal(std::string_view lhs, std::string_view rhs) -> bool;
auto is_equal_detailed(const MemoryInput& lhs, const MemoryInput& rhs)
  -> std::tuple<bool, std::string, std::string>;
}   //namespace hack

#endif
