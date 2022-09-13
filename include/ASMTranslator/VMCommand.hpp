#ifndef NAND2TETRIS_INCLUDE_ASMTRANSLATOR_VMCOMMAND_HPP
#define NAND2TETRIS_INCLUDE_ASMTRANSLATOR_VMCOMMAND_HPP

#include <cstdint>
#include <string_view>
#include <ostream>

namespace hack::vasm
{
class VMCommand
{
  public:
    enum class Command {
      Arithmetic,
      Logical,
      Push,
      Pop,
      Label,
      Goto,
      If,
      Function,
      Return,
      Call
    };

    VMCommand(Command cmd, const std::string_view str = {}, int64_t index = 0)
        : command_(cmd)
        , arg1_(std::string(str))
        , arg2_(index)
    {
    }
    friend std::ostream& operator<<(std::ostream& out, const VMCommand& cmd);   //NOLINT
    friend std::ostream& operator<<(std::ostream& out, Command cmd);            //NOLINT

    [[nodiscard]] auto type() const noexcept -> Command { return command_; };
    [[nodiscard]] auto arg1() const noexcept -> std::string_view { return arg1_; }
    [[nodiscard]] auto arg2() const noexcept -> int64_t { return arg2_; }

  private:
    Command     command_;
    std::string arg1_;
    int64_t     arg2_ = 0;
};

}   //namespace hack::vasm

#endif