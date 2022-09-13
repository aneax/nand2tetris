#ifndef NAND2TETRIS_INCLUDE_ASMTRANSLATOR_ASMGENERATOR_HPP
#define NAND2TETRIS_INCLUDE_ASMTRANSLATOR_ASMGENERATOR_HPP

#include "VMData.hpp"

namespace hack::vasm
{
void generate_asm(const VMData& data);
}   //namespace hack::vasm

#endif