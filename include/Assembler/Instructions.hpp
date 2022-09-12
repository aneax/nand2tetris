#ifndef NAND2TETRIS_INCLUDE_DRIVER_HPP
#define NAND2TETRIS_INCLUDE_DRIVER_HPP
#include "FwdDecl.hpp"
#include <cstdint>

namespace hack
{

namespace constants
{
static constexpr size_t ROW0  = 0b101010;   //0
static constexpr size_t ROW1  = 0b111111;   //1
static constexpr size_t ROW2  = 0b111010;   //-1
static constexpr size_t ROW3  = 0b001100;   //D
static constexpr size_t ROW4  = 0b110000;   //A , M
static constexpr size_t ROW5  = 0b001101;   //!D
static constexpr size_t ROW6  = 0b110001;   //!A, !M
static constexpr size_t ROW7  = 0b001111;   //-D
static constexpr size_t ROW8  = 0b110011;   //-A,-M
static constexpr size_t ROW9  = 0b011111;   //D+1
static constexpr size_t ROW10 = 0b110111;   //A+1, m+1
static constexpr size_t ROW11 = 0b001110;   //D-1
static constexpr size_t ROW12 = 0b110010;   //A-1,M-1
static constexpr size_t ROW13 = 0b000010;   //D+A,D+M
static constexpr size_t ROW14 = 0b010011;   //D-A,D-M
static constexpr size_t ROW15 = 0b000111;   //A-D,M-D
static constexpr size_t ROW16 = 0b000000;   //D&A,D&M
static constexpr size_t ROW17 = 0b010101;   //D|A,D|M

static constexpr size_t VJGT = 0b001;
static constexpr size_t VJEQ = 0b010;
static constexpr size_t VJNE = 0b101;
static constexpr size_t VJLT = 0b100;

static constexpr size_t VNULL = 0b000;

static constexpr size_t DestM = 0b001;
static constexpr size_t DestD = 0b010;
static constexpr size_t DestA = 0b100;
}   //namespace constants

void populate_predefined_symbols(Data* data);
auto get_a_inst(uint64_t value) -> Bit;
auto get_c_inst(bool type_a, uint64_t cmd, uint64_t dest, uint64_t jmp) -> Bit;
auto to_string(const BitVector& vec) -> std::string;
}   //namespace hack

#endif