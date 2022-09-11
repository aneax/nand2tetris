#ifndef NAND2TETRIS_INCLUDE_FWDDECL_HPP
#define NAND2TETRIS_INCLUDE_FWDDECL_HPP

#include <vector>
#include <bitset>
#include <unordered_map>

namespace hack
{

enum class TokenType;

class Statement;
using StatementVector = std::vector<Statement>;

class Token;
using TokenVector = std::vector<Token>;
using SymbolTable = std::unordered_map<std::string, size_t>;

static constexpr size_t BitSize = 16;

using Bit       = std::bitset<BitSize>;
using BitVector = std::vector<Bit>;
using JumpTable = std::unordered_map<TokenType, size_t>;
using DestTable = std::unordered_map<std::string_view, size_t>;
using CompTable = std::unordered_map<std::string_view, std::pair<bool, size_t>>;

struct Data {
    StatementVector stmts;
    SymbolTable     symbols;
    JumpTable       jump_table;
    DestTable       dest_table;
    CompTable       comp_table;
};

namespace constants
{

//Registers
static constexpr size_t R0  = 0;
static constexpr size_t R1  = 1;
static constexpr size_t R2  = 2;
static constexpr size_t R3  = 3;
static constexpr size_t R4  = 4;
static constexpr size_t R5  = 5;
static constexpr size_t R6  = 6;
static constexpr size_t R7  = 7;
static constexpr size_t R8  = 8;
static constexpr size_t R9  = 9;
static constexpr size_t R10 = 10;
static constexpr size_t R11 = 11;
static constexpr size_t R12 = 12;
static constexpr size_t R13 = 13;
static constexpr size_t R14 = 14;
static constexpr size_t R15 = 15;

//Memory Maps
static constexpr size_t SCREEN = 16384;
static constexpr size_t KBD    = 24576;

//Keywords
static constexpr size_t SP   = 0;
static constexpr size_t LCL  = 1;
static constexpr size_t ARG  = 2;
static constexpr size_t THIS = 3;
static constexpr size_t THAT = 4;

//Bit Size
static constexpr size_t InstIdentifier = 1;
static constexpr size_t AInstSize      = 15;
static constexpr size_t CInstASize     = 1;
static constexpr size_t CInstBegin     = 7;
static constexpr size_t CInstCompSize  = 6;
static constexpr size_t CInstDestSize  = 3;
static constexpr size_t CInstJumpSize  = 3;

//
static constexpr size_t SymbolStartAddress = 16;
}   //namespace constants

}   //namespace hack

#endif