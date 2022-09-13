#include "VMCommand.hpp"

namespace hack::vasm
{
//NOLINTNEXTLINE
std::ostream& operator<<(std::ostream& out, const VMCommand& cmd)
{
  switch (cmd.type()) {
    case VMCommand::Command::Arithmetic:
    case VMCommand::Command::Logical:
    case VMCommand::Command::Return:
      out << cmd.arg1();
      break;
    case VMCommand::Command::Push:
    case VMCommand::Command::Pop:
    case VMCommand::Command::Function:
    case VMCommand::Command::Call:
      out << cmd.type() << " " << cmd.arg1() << " " << cmd.arg2();
      break;
    case VMCommand::Command::Label:
    case VMCommand::Command::Goto:
    case VMCommand::Command::If:
      out << cmd.type() << " " << cmd.arg1();
      break;
  }
  return out;
}

//NOLINTNEXTLINE
std::ostream& operator<<(std::ostream& out, VMCommand::Command cmd)
{
  switch (cmd) {
    case VMCommand::Command::Arithmetic:
      out << "arithmetic";
      break;
    case VMCommand::Command::Logical:
      out << "logical";
      break;
    case VMCommand::Command::Push:
      out << "push";
      break;
    case VMCommand::Command::Pop:
      out << "pop";
      break;
    case VMCommand::Command::Label:
      out << "label";
      break;
    case VMCommand::Command::Goto:
      out << "goto";
      break;
    case VMCommand::Command::If:
      out << "if-goto";
      break;
    case VMCommand::Command::Function:
      out << "function";
      break;
    case VMCommand::Command::Return:
      out << "return";
      break;
    case VMCommand::Command::Call:
      out << "call";
      break;
  }
  return out;
}
}   //namespace hack::vasm