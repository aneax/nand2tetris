#ifndef NAND2TETRIS_INCLUDE_COMPILER_PARSER_HPP
#define NAND2TETRIS_INCLUDE_COMPILER_PARSER_HPP

#include "JackCompiler/AST.hpp"
#include <string_view>

namespace jack
{

auto do_parse(std::string_view file_path) -> std::vector<ClassDecl>;

}   //namespace jack

#endif
