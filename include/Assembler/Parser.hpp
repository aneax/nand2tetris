#ifndef NAND2TETRIS_INCLUDE_PARSER_HPP
#define NAND2TETRIS_INCLUDE_PARSER_HPP

#include "FwdDecl.hpp"
#include <vector>

namespace hack
{

auto parse(const TokenVector& token) -> Data;
}   //namespace hack

#endif