#ifndef NAND2TETRIS_INCLUDE_SIMULATOR_HARDWARE_PARSER_HPP
#define NAND2TETRIS_INCLUDE_SIMULATOR_HARDWARE_PARSER_HPP

#include "Bit.hpp"
#include <string_view>
#include <vector>

namespace hack::defs
{
enum class ParseFrom {
  File,
  Memory
};

auto parse(std::string_view file_path, ParseFrom type = ParseFrom::File)
  -> std::vector<Word>;
}   //namespace hack::defs

#endif