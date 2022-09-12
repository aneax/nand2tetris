#include "FwdDecl.hpp"
#include "Ast.hpp"
#include "Instructions.hpp"
#include "Lexer.hpp"
#include "Visitors.hpp"
#include <Errors.hpp>
#include <cstdint>
#include <sstream>
#include <fmt/format.h>

namespace hack
{

class Assembler : public ConstStmtVisitor<Assembler, Bit>
{
    using Base              = ConstStmtVisitor<Assembler>;
    using Type              = Statement::Type;
    SymbolTable* symbols_   = nullptr;
    JumpTable*   jmpTable_  = nullptr;
    DestTable*   destTable_ = nullptr;
    CompTable*   compTable_ = nullptr;

    size_t startAddress_ = constants::SymbolStartAddress;

  public:
    Assembler() = default;

    auto start(Data& data) -> BitVector
    {
      symbols_   = &data.symbols;
      compTable_ = &data.comp_table;
      destTable_ = &data.dest_table;
      jmpTable_  = &data.jump_table;

      BitVector bit_vec;
      bit_vec.reserve(data.stmts.size());
      for (const auto& stmt : data.stmts) {
        if (!stmt.isa(Type::Label)) {
          bit_vec.push_back(this->Visit(&stmt));
        }
      }
      startAddress_ = constants::SymbolStartAddress;
      return bit_vec;
    }

    //NOLINTNEXTLINE
    Bit VisitLabel(const Statement* stmt) { throw RuntimeError("Logically unreachable region"); }

    //NOLINTNEXTLINE
    Bit VisitSymbol(const Statement* stmt)
    {
      const Token& token = stmt->token();
      std::string  id    = token.string();
      auto         it    = symbols_->find(id);

      size_t addr;
      if (it == symbols_->end()) {
        addr = startAddress_;
        symbols_->insert({id, startAddress_++});
      } else {
        addr = it->second;
      }
      return get_a_inst(addr);
    }

    //NOLINTNEXTLINE
    Bit VisitAInst(const Statement* stmt)
    {
      uint64_t value = stoul(stmt->token().string(), nullptr);
      Bit      out   = get_a_inst(value);
      return out;
    }

    //NOLINTNEXTLINE
    Bit VisitCInst(const Statement* stmt)
    {
      size_t dest = constants::VNULL;
      size_t jmp  = constants::VNULL;
      size_t comp;
      bool   isa;

      using Type           = Statement::Type;
      const auto& children = stmt->children();
      auto        begin    = children.begin();
      auto        end      = children.end();
      if (begin->isa(Type::Dest)) {
        dest = destTable_->at(begin->token().string_view());
        ++begin;
      }

      for (const auto& child : stmt->children()) {
        if (child.isa(Type::Comp)) {
          std::tie(isa, comp) = VisitComp(&child);
        }

        if (child.isa(Type::Jump)) {
          jmp = jmpTable_->at(child.token().type());
        }
      }
      Bit bit = get_c_inst(isa, comp, dest, jmp);
      return bit;
    }

    //NOLINTNEXTLINE
    auto VisitComp(const Statement* stmt) -> std::pair<bool, size_t>
    {
      return compTable_->at(stmt->token().string_view());
    }
};

auto generate_binary(Data& data) -> BitVector
{
  static Assembler assembler;
  return assembler.start(data);
}

}   //namespace hack
