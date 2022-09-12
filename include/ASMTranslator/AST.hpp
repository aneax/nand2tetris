#ifndef NAND2TETRIS_INCLUDE_ASMTRANSLATOR_AST_HPP
#define NAND2TETRIS_INCLUDE_ASMTRANSLATOR_AST_HPP

#include <string>

namespace hack::vasm
{
class VMCommand
{
  public:
    enum class Command {
      Arithmetic,
      Push,
      Pop,
      Label,
      Goto,
      If,
      Function,
      Return,
      Call
    };

    VMCommand(Command cmd) : command_(cmd) {}
    
  private:
    Command     command_;
    std::string arg1_;
    int64_t     arg2_ = 0;
};
}   //namespace hack::vasm

#endif