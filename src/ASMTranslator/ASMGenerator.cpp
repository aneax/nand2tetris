#include "ASMGenerator.hpp"
#include "Errors.hpp"
#include "VMData.hpp"
#include <Traits.hpp>
#include <filesystem>
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

    template <typename Apply>
    auto push_pop_(Apply apply) -> void;

    template <Segment Type>
    auto push_(size_t index, std::string_view src) -> void;

    template <Segment Type>
    auto pop_(size_t index, std::string_view src) -> void;

    auto binary_(std::string_view op) -> void;
    auto unary_(std::string_view op) -> void;
    auto logical_(std::string_view op) -> void;

    template <typename T>
    requires is_any_one_of_v<T, std::string_view, size_t>
    inline void a_inst_(T value)
    {
      ss_ << "@" << value << "\n";
    }

    template <typename T>
    requires is_any_one_of_v<T, std::string_view, size_t>
    inline void c_inst_(T src, std::string_view dest, std::string_view jump = "",
                        std::string_view comment = "")
    {
      if (!dest.empty()) {
        ss_ << dest << "=";
      }
      ss_ << src;
      if (!jump.empty()) {
        ss_ << ";" << jump;
      }
      if (!comment.empty()) {
        ss_ << "//" << comment;
      }

      ss_ << "\n";
    }

    inline auto sp_inc_() -> void
    {
      a_inst_("SP"sv);
      //c_inst_("M+1"sv, "M", "", "SP++");
      ss_ << "M=M+1 // SP++\n";
    }

    inline auto sp_dec_() -> void
    {
      ss_ << "@SP\n";
      ss_ << "M=M-1 // SP--\n";
    }

    inline auto sp_load_(std::string_view dest) -> void
    {
      ss_ << "A=M\n";
      ss_ << "D=";
      ss_ << dest << " // D=*SP\n";
    }

    inline auto write_(std::string_view dest, std::string_view src) -> void
    {
      ss_ << dest << "=" << src << "\n";
    }

    fs::path                        outfilename_;
    std::string                     filename_;
    VMCommandVector::const_iterator current_;
    VMCommandVector::const_iterator end_;
    SegmentMap                      segments_;
    OperatorMap                     operators_;
    std::stringstream               ss_;
    size_t logicalLabelCount_ = 0;
};

template <Segment Type>
auto ASMGenerator::push_(size_t index, [[maybe_unused]] std::string_view src) -> void
{
  if constexpr (Type == Segment::Constant) {
    ss_ << "@" << index << "\n";
    ss_ << "D=A"
        << "\n";
  } else if constexpr (Type == Segment::Pointer) {
    ss_ << "@" << src << "\n";
    ss_ << "D=A"
        << "\n";
  } else if constexpr (Type == Segment::Static) {
    ss_ << "@" << src << "." << index << "\n";
    ss_ << "D=A"
        << "\n";
  } else {
    ss_ << "@" << index << "\n";
    ss_ << "D=A";
    ss_ << "@" << src << "\n";
    ss_ << "D=D+A\n";
  }

  ss_ << "@SP\n";
  ss_ << "A=M\n";
  ss_ << "M=D\n";
  ss_ << "@SP\n";
  ss_ << "M=M+1\n";
}

template <Segment Type>
auto ASMGenerator::pop_(size_t index, std::string_view src) -> void
{
  if constexpr (Type == Segment::Constant) {
    throw RuntimeError("There is no pop constant");
  } else if constexpr (Type == Segment::Pointer) {
    ss_ << "@" << src << "\n";
    ss_ << "D=A"
        << "\n";
  } else if constexpr (Type == Segment::Static) {
    ss_ << "@" << src << "." << index << "\n";
    ss_ << "D=A"
        << "\n";
  } else {
    ss_ << "@" << src << "\n";
    ss_ << "A=A+" << index << "\ns";
  }

  ss_ << "@SP\n";
  ss_ << "A=M\n";
  ss_ << "M=D\n";
  ss_ << "@SP\n";
  ss_ << "M=M+1\n";
}

auto ASMGenerator::binary_(std::string_view op) -> void
{
  // decrease stack pointer
  ss_ << "@SP\n";
  ss_ << "M=M-1 // SP--\n";
  // load pointed value to data memory
  ss_ << "A=M\n";
  ss_ << "D=M // D=*SP\n";
  // point to SP-1
  ss_ << "@SP\n";
  ss_ << "A=M-1\n";
  // do the operation
  ss_ << "M=" << op << " // M=*SP\n";
  // didn't decrease twice, so no need to increase.

  /*
  @SP;A=M-1;M=D+M; == @SP; M=M-1; A=M; M=D+M; @SP; M=M+1"
  */
}

auto ASMGenerator::unary_(std::string_view op) -> void
{
  // point to SP-1
  ss_ << "@SP\n";
  ss_ << "A=M-1 // SP--\n";
  // do th operation
  ss_ << "M=" << op << "M // M=*SP\n";
  // didn't change the SP at all, so no need to increase.
}

auto ASMGenerator::logical_(std::string_view op) -> void 
{
  // decrease stack pointer
  ss_ << "@SP\n";
  ss_ << "M=M-1 // SP--\n";
  // load pointed value to data memory
  ss_ << "A=M\n";
  ss_ << "D=M // D=*SP\n";
  // point to SP-1
  ss_ << "@SP\n";
  ss_ << "M=M-1\n";
  // do subtract operation
  ss_ << "A=M\n";
  ss_ << "D=M-D\n";
  // op with jump to LL<count>True if true
  ss_ << "@LL" << logicalLabelCount_ << "True" << "\n";
  ss_ << "D;" << op << "\n";
  // continue with false branch
  ss_ << "@SP\n";
  ss_ << "A=M\n";
  ss_ << "M=0 // false\n";
  // unconditional jump to merge branch
  ss_ << "@LL" << logicalLabelCount_ << "Merge\n"; 
  ss_ << "0;JMP\n";
  // true branch label
  ss_ << "(LL" << logicalLabelCount_ << "True)\n";
  ss_ << "@SP\n";
  ss_ << "A=M\n";
  ss_ << "M=-1 // true\n";
  // merge branch label
  ss_ << "(LL" << logicalLabelCount_++ << "Merge)\n";
  // increase SP.
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

      apply.template operator()<Segment::Constant>(TempIndex + index, "");
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

void ASMGenerator::start()
{
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
      case CMD::Goto:
      case CMD::If:
      case CMD::Function:
      case CMD::Return:
      case CMD::Call:
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