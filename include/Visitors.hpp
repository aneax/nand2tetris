#ifndef NAND2TETRIS_INCLUDE_VISITORS_HPP
#define NAND2TETRIS_INCLUDE_VISITORS_HPP

#include "FwdDecl.hpp"
#include <string>

namespace hack
{
auto src_rewrite(const StatementVector& stmts) -> std::string;
auto symbol_resolved_rewrite(Data& data) -> std::string;
auto generate_binary(Data& data) -> BitVector;
}   //namespace hack

#endif