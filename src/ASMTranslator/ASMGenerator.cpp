#include "ASMGenerator.hpp"
#include "Errors.hpp"
#include "VMData.hpp"
#include <Traits.hpp>
#include <cstddef>
#include <filesystem>
#include <initializer_list>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <fstream>

namespace hack::vasm
{

enum class Segment {
  Constant,
  Local,
  Static,
  Argument,
  This,
  That,
  Pointer,
  Temp
};

enum class Operators {
  Add,
  Sub,
  Neg,
  Not,
  LT,
  GT,
  Eq,
  And,
  Or
};

static constexpr size_t TempLoc = 5;
//static constexpr size_t ThisLoc =

using SegmentMap  = std::unordered_map<std::string_view, Segment>;
using OperatorMap = std::unordered_map<std::string_view, Operators>;
auto populate_segment_map() -> SegmentMap
{
  SegmentMap map;
  map.insert({"constant", Segment::Constant});
  map.insert({"local", Segment::Local});
  map.insert({"static", Segment::Static});
  map.insert({"argument", Segment::Argument});
  map.insert({"this", Segment::This});
  map.insert({"that", Segment::That});
  map.insert({"pointer", Segment::Pointer});
  map.insert({"temp", Segment::Temp});
  return map;
}

auto populate_operator_map() -> OperatorMap
{
  OperatorMap map;
  map.insert({"add", Operators::Add});
  map.insert({"sub", Operators::Sub});
  map.insert({"neg", Operators::Neg});
  map.insert({"eq", Operators::Eq});
  map.insert({"gt", Operators::GT});
  map.insert({"lt", Operators::LT});
  map.insert({"and", Operators::And});
  map.insert({"or", Operators::Or});
  map.insert({"not", Operators::Not});
  return map;
}
using namespace std::string_view_literals;
class ASMGenerator
{
  public:
    using CMD = VMCommand::Command;
    ASMGenerator(const VMData& data)
        : outfilename_(data.dest)
        , filename_(outfilename_.filename().replace_extension())
        , current_(data.commands.begin())
        , end_(data.commands.end())
        , segments_(populate_segment_map())
        , operators_(populate_operator_map())
    {
      outfilename_.replace_extension(".asm");
    }
    void start();
    void dump();

  private:
    void translate_push_();
    void translate_pop_();
    void translate_arith_();
    void translate_logical_();
    void translate_if_();
    void translate_label_();
    void translate_function_();
    void translate_call_();
    void translate_goto_();
    void translate_return_();

    template <typename Apply>
    auto push_pop_(Apply apply) -> void;

    template <Segment Type>
    auto push_(size_t index, std::string_view src) -> void;

    template <Segment Type>
    auto pop_(size_t index, std::string_view src) -> void;

    auto goto_(std::string_view label) -> void;
    auto label_(std::string_view label) -> void;
    auto binary_(std::string_view op) -> void;
    auto unary_(std::string_view op) -> void;
    auto logical_(std::string_view op) -> void;

    fs::path                        outfilename_;
    std::string                     filename_;
    VMCommandVector::const_iterator current_;
    VMCommandVector::const_iterator end_;
    SegmentMap                      segments_;
    OperatorMap                     operators_;
    std::stringstream               ss_;
    size_t                          logicalLabelCount_  = 0;
    size_t                          functionLabelCount_ = 0;
    std::string_view                currentFunction_{};
};

template <Segment Type>
auto ASMGenerator::push_(size_t index, [[maybe_unused]] std::string_view src) -> void
{
  if constexpr (Type == Segment::Pointer) {
    ss_ << "@" << src << "\n";
    ss_ << "D=M\n";
    ss_ << "@SP\n";
    ss_ << "A=M\n";
    ss_ << "M=D\n";
    ss_ << "@SP\n";
    ss_ << "M=M+1\n";
  } else {
    if constexpr (Type == Segment::Constant) {
      ss_ << "@" << index << "\n";
      ss_ << "D=A\n";
    } else if constexpr (Type == Segment::Static) {
      ss_ << "@" << src << "." << index << "\n";
      ss_ << "D=M\n";
    } else if constexpr (Type == Segment::Temp) {
      ss_ << "@" << index << "\n";
      ss_ << "D=M\n";
    } else {
      //point to index
      ss_ << "@" << index << "\n";
      //load constant
      ss_ << "D=A\n";
      //point to src= [LCL, ARG, THIS, THAT]
      ss_ << "@" << src << "\n";
      //get the actual memory location
      ss_ << "A=D+M\n";
      ss_ << "D=M\n";
    }

    ss_ << "@SP\n";
    ss_ << "A=M\n";
    ss_ << "M=D\n";
    ss_ << "@SP\n";
    ss_ << "M=M+1\n";
  }
}

template <Segment Type>
auto ASMGenerator::pop_(size_t index, std::string_view src) -> void
{
  if constexpr (Type == Segment::Pointer) {
    ss_ << "@SP\n";
    ss_ << "M=M-1\n";
    ss_ << "A=M\n";
    ss_ << "D=M\n";
    ss_ << "@" << src << "\n";
    ss_ << "M=D\n";
  } else {
    if constexpr (Type == Segment::Constant) {
      throw RuntimeError("There is no pop constant");
    } else if constexpr (Type == Segment::Static) {
      ss_ << "@" << src << "." << index << "\n";
      ss_ << "D=A"
          << "\n";
    } else if constexpr (Type == Segment::Temp) {
      ss_ << "@" << index << "\n";
      ss_ << "D=A\n";
    } else {
      //point to index
      ss_ << "@" << index << "\n";
      //load constant
      ss_ << "D=A\n";
      //point to src= [LCL, ARG, THIS, THAT]
      ss_ << "@" << src << "\n";
      //get the actual memory location
      ss_ << "D=D+M\n";
    }

    //common portion
    //store address in R15
    ss_ << "@R15\n";
    ss_ << "M=D\n";

    //point to SP
    ss_ << "@SP\n";
    //decrease it
    ss_ << "M=M-1\n";
    //point to the memory location
    ss_ << "A=M\n";
    //load the data
    ss_ << "D=M\n";
    //load the address from R15
    ss_ << "@R15\n";
    ss_ << "A=M\n";
    //save to the memory address
    ss_ << "M=D\n";
    //SP is already decreased.
  }
}

auto ASMGenerator::binary_(std::string_view op) -> void
{
  //decrease stack pointer
  ss_ << "@SP\n";
  ss_ << "M=M-1 // SP--\n";
  //load pointed value to data memory
  ss_ << "A=M\n";
  ss_ << "D=M // D=*SP\n";
  //point to SP-1
  ss_ << "@SP\n";
  ss_ << "A=M-1\n";
  //do the operation
  ss_ << "M=" << op << " // M=*SP\n";
  //didn't decrease twice, so no need to increase.

  /*
  @SP;A=M-1;M=D+M; == @SP; M=M-1; A=M; M=D+M; @SP; M=M+1"
  */
}

auto ASMGenerator::unary_(std::string_view op) -> void
{
  //point to SP-1
  ss_ << "@SP\n";
  ss_ << "A=M-1 // SP--\n";
  //do th operation
  ss_ << "M=" << op << "M // M=*SP\n";
  //didn't change the SP at all, so no need to increase.
}

auto ASMGenerator::logical_(std::string_view op) -> void
{
  //decrease stack pointer
  ss_ << "@SP\n";
  ss_ << "M=M-1 // SP--\n";
  //load pointed value to data memory
  ss_ << "A=M\n";
  ss_ << "D=M // D=*SP\n";
  //point to SP-1
  ss_ << "@SP\n";
  ss_ << "M=M-1\n";
  //do subtract operation
  ss_ << "A=M\n";
  ss_ << "D=M-D\n";
  //op with jump to LL<count>True if true
  ss_ << "@LL" << logicalLabelCount_ << "True"
      << "\n";
  ss_ << "D;" << op << "\n";
  //continue with false branch
  ss_ << "@SP\n";
  ss_ << "A=M\n";
  ss_ << "M=0 // false\n";
  //unconditional jump to merge branch
  ss_ << "@LL" << logicalLabelCount_ << "Merge\n";
  ss_ << "0;JMP\n";
  //true branch label
  ss_ << "(LL" << logicalLabelCount_ << "True)\n";
  ss_ << "@SP\n";
  ss_ << "A=M\n";
  ss_ << "M=-1 // true\n";
  //merge branch label
  ss_ << "(LL" << logicalLabelCount_++ << "Merge)\n";
  //increase SP.
  ss_ << "@SP\n";
  ss_ << "M=M+1\n";
}
template <typename Apply>
auto ASMGenerator::push_pop_(Apply apply) -> void
{
  Segment seg   = segments_.at(current_->arg1());
  auto    index = current_->arg2();
  switch (seg) {
    case Segment::Constant:
      apply.template operator()<Segment::Constant>(index, "");
      break;
    case Segment::Local:
      apply.template operator()<Segment::Local>(index, "LCL");
      break;
    case Segment::Static:
      apply.template operator()<Segment::Static>(index, filename_);
      break;
    case Segment::Argument:
      apply.template operator()<Segment::Argument>(index, "ARG");
      break;
    case Segment::This:
      apply.template operator()<Segment::This>(index, "THIS");
      break;
    case Segment::That:
      apply.template operator()<Segment::This>(index, "THAT");
      break;
    case Segment::Pointer:
      if (index == 0U) {
        apply.template operator()<Segment::Pointer>(0, "THIS");
      } else {
        apply.template operator()<Segment::Pointer>(0, "THAT");
      }
      break;
    case Segment::Temp:
      static constexpr size_t TempIndex = 5;

      apply.template operator()<Segment::Temp>(TempIndex + index, "");
      break;
  }
}

void ASMGenerator::translate_push_()
{
  auto apply = [&]<Segment T>(size_t index, std::string_view src) { push_<T>(index, src); };
  push_pop_(apply);
}

void ASMGenerator::translate_pop_()
{
  auto apply = [&]<Segment T>(size_t index, std::string_view src) { pop_<T>(index, src); };
  push_pop_(apply);
}

auto ASMGenerator::goto_(std::string_view label) -> void
{
  ss_ << "@" << label << "\n";
  ss_ << "0;JMP\n";
}

auto ASMGenerator::label_(std::string_view label) -> void
{
  ss_ << "(" << label << ")\n";
}

void ASMGenerator::translate_arith_()
{
  Operators op = operators_.at(current_->arg1());
  switch (op) {
    case Operators::Add:
      binary_("D+M");
      break;
    case Operators::Sub:
      binary_("M-D");
      break;
    case Operators::LT:
      logical_("JLT");
      break;
    case Operators::GT:
      logical_("JGT");
      break;
    case Operators::Eq:
      logical_("JEQ");
      break;
    case Operators::And:
      binary_("D&M");
      break;
    case Operators::Or:
      binary_("D|M");
      break;
    case Operators::Neg:
      unary_("-");
      break;
    case Operators::Not:
      unary_("!");
      break;
  }
}

void ASMGenerator::translate_if_()
{
  ss_ << "@SP\n";
  ss_ << "M=M-1\n";
  ss_ << "A=M\n";
  ss_ << "D=M\n";
  ss_ << "@" << current_->arg1() << "\n";
  ss_ << "D;JNE\n";
}

void ASMGenerator::translate_label_()
{
  label_(current_->arg1());
}

void ASMGenerator::translate_function_()
{
  label_(current_->arg1());
  for (size_t arg = 0; arg < current_->arg2(); ++arg) {
    ss_ << "@SP\n";
    ss_ << "A=M\n";   //load SP location to D
    ss_ << "M=0\n";
    ss_ << "@SP\n";
    ss_ << "M=M+1\n";   //initialize variable
  }
}

void ASMGenerator::translate_call_()
{
  //push return address
  ss_ << "@Return" << current_->arg1() << "_" << functionLabelCount_ << "\n";
  ss_ << "D=A\n";
  ss_ << "@SP\n";
  ss_ << "A=M\n";
  ss_ << "M=D\n";
  ss_ << "@SP\n";
  ss_ << "M=M+1\n";
  //push LCL, ARG, THIS, THAT
  constexpr std::array<std::string_view, 4> Savee = {"LCL", "ARG", "THIS", "THAT"};
  for (const auto& src : Savee) {
    ss_ << "@" << src << "\n";
    ss_ << "D=M\n";
    ss_ << "@SP\n";
    ss_ << "A=M\n";
    ss_ << "M=D\n";
    ss_ << "@SP\n";
    ss_ << "M=M+1\n";
  }
  //ARG = SP - 5 - nArgs
  ss_ << "@SP\n";
  ss_ << "D=M\n";
  ss_ << "@5\n";
  ss_ << "D=D-A\n";
  ss_ << "@" << current_->arg2() << "\n";
  ss_ << "D=D-A\n";
  ss_ << "@ARG\n";
  ss_ << "M=D\n";

  //LCL=SP
  ss_ << "@SP\n";
  ss_ << "D=M\n";
  ss_ << "@LCL\n";
  ss_ << "M=D\n";

  //goto functionName
  goto_(current_->arg1());

  ////return address label
  ss_ << "(Return" << current_->arg1() << "_" << functionLabelCount_++ << ")\n";
}
void ASMGenerator::translate_goto_()
{
  goto_(current_->arg1());
}
void ASMGenerator::translate_return_()
{
  //temp = LCL
  ss_ << "@LCL\n";
  ss_ << "D=M\n";
  ss_ << "@R15\n";
  ss_ << "M=D\n";

  //get return address = retAddr = *(temp - 5)
  ss_ << "@R15\n";
  ss_ << "D=M\n";
  ss_ << "@5\n";
  ss_ << "A=D-A\n";   //temp -5
  ss_ << "D=M\n";     //D=*(temp-5)
  ss_ << "@R14\n";
  ss_ << "M=D\n";   //return address in R14

  //save result of the function *ARG=pop()
  ss_ << "@SP\n";
  ss_ << "A=M-1\n";
  ss_ << "D=M\n";
  ss_ << "@ARG\n";
  ss_ << "A=M\n";
  ss_ << "M=D\n";

  //Moves SP to after return value
  ss_ << "@ARG\n";
  ss_ << "D=M+1\n";
  ss_ << "@SP\n";
  ss_ << "M=D\n";

  //restore callee save registers
  //dest = *(temp - index)
  using namespace std::string_view_literals;
  for (const auto& src : std::initializer_list<std::pair<std::string_view, size_t>>{
         {"THAT"sv, 1},
         {"THIS"sv, 2},
         { "ARG"sv, 3},
         { "LCL"sv, 4}
  }) {
    ss_ << "@R15\n";
    ss_ << "D=M\n";
    ss_ << "@" << src.second << "\n";
    ss_ << "A=D-A\n";
    ss_ << "D=M\n";
    ss_ << "@" << src.first << "\n";
    ss_ << "M=D\n";
  }

  //goto return address
  ss_ << "@R14\n";
  ss_ << "A=M\n";
  ss_ << "0;JMP\n";
}

void ASMGenerator::start()
{
  //ss_ << "@256\n";
  //ss_ << "D=A\n";
  //ss_ << "@SP\n";
  //ss_ << "M=D\n";

  for (; current_ != end_; ++current_) {
    ss_ << "// " << *current_ << "\n";
    switch (current_->type()) {
      case CMD::Arithmetic:
      case CMD::Logical:
        {
          translate_arith_();
          break;
        }
      case CMD::Push:
        {
          translate_push_();
          break;
        }
      case CMD::Pop:
        {
          translate_pop_();
          break;
        }
      case CMD::Label:
        {
          translate_label_();
          break;
        }
      case CMD::Goto:
        {
          translate_goto_();
          break;
        }
      case CMD::If:
        {
          translate_if_();
          break;
        }
      case CMD::Function:
        {
          translate_function_();
          break;
        }
      case CMD::Return:
        {
          translate_return_();
          break;
        }
      case CMD::Call:
        {
          translate_call_();
          break;
        }
        break;
    }
  }
  ss_ << "(END)\n";
  ss_ << "@END\n";
  ss_ << "0;JMP\n";
}

void ASMGenerator::dump()
{
  std::ofstream out(outfilename_);
  out << ss_.rdbuf();
}

void generate_asm(const VMData& data)
{
  ASMGenerator generator(data);
  generator.start();
  generator.dump();
}
}   //namespace hack::vasm