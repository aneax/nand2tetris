#ifndef NAND2TETRIS_INCLUDE_ASMTRANSLATOR_VMPARSER_HPP
#define NAND2TETRIS_INCLUDE_ASMTRANSLATOR_VMPARSER_HPP

#include "VMData.hpp"

namespace hack::vasm
{

auto parse_bytecode(const fs::path& src) -> VMData;

}   //namespace hack::vasm

#endif