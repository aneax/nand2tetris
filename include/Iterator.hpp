#ifndef NAND2TETRIS_INCLUDE_ITERATOR_HPP
#define NAND2TETRIS_INCLUDE_ITERATOR_HPP


#include <cstddef>
#include <cassert>
#include <ostream>

namespace hack
{
struct Iterator {
    using difference_type = char;
    using value_type      = char;
    using pointer         = char*;
    constexpr Iterator(const char* in, size_t in_line, size_t in_column)
        : data(in)
        , line(in_line)
        , column(in_column)
    {
      assert(in_line != 0);
      assert(in_column != 0);
    }
    const char* data   = nullptr;
    size_t      line   = 1;
    size_t      column = 1;
};

auto operator<<(std::ostream& out, Iterator iterator) -> std::ostream&;
auto to_string(Iterator iterator) -> std::string;
}   //namespace hack
#endif