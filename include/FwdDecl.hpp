#ifndef NAND2TETRIS_INCLUDE_FWDDECL_HPP
#define NAND2TETRIS_INCLUDE_FWDDECL_HPP

#include "SymbolTable.hpp"
#include <vector>

namespace hack
{

class Statement;
using StatementVector = std::vector<Statement>;

class Token;
using TokenVector = std::vector<Token>;

class SymbolTable;

struct Data {
    StatementVector stmts;
    SymbolTable     symbols;
};

}   //namespace hack

#endif