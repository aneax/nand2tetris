#ifndef NAND2TETRIS_INCLUDE_VISITORS_HPP
#define NAND2TETRIS_INCLUDE_VISITORS_HPP

#include "FwdDecl.hpp"
#include <string>

namespace hack
{
auto src_rewrite(const StatementVector& stmts) -> std::string;
}   //namespace hack

#endif