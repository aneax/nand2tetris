#ifndef NAND2TETRIS_INCLUDE_UTILS_CONSTANTSTRING_HPP
#define NAND2TETRIS_INCLUDE_UTILS_CONSTANTSTRING_HPP

#include <array>
#include <algorithm>

namespace hack
{
template <size_t N>
struct ConstantString {
    constexpr ConstantString(const char (&str)[N])   //NOLINT
    {
      std::copy_n(str, N, value.begin());
    }

    std::array<char, N> value;
};

}   //namespace hack
#endif
