#include "Visitors.hpp"
#include "Ast.hpp"
#include "Instructions.hpp"

namespace hack
{

class SymbolResolvedRewriter : public ConstStmtVisitor<SymbolResolvedRewriter>
{
    using Base                       = ConstStmtVisitor<SymbolResolvedRewriter>;
    using Type                       = Statement::Type;
    const SymbolTable* symbols_      = nullptr;
    size_t             startAddress_ = constants::SymbolStartAddress;
    std::stringstream  ss_;

  public:
    SymbolResolvedRewriter() = default;

    auto string() const noexcept -> std::string { return ss_.str(); }

    void start(const Data& data)
    {
      symbols_   = &data.symbols;
      for (const auto& stmt : data.stmts) {
        Visit(&stmt);
      }
    }

    //NOLINTNEXTLINE
    void VisitLabel(const Statement* stmt)
    {
      //ss_ << "(" << symbols_->at(stmt->token().string()) << ")\n";
    }

    //NOLINTNEXTLINE
    void VisitSymbol(const Statement* stmt) { ss_ << "@" << symbols_->at(stmt->token().string()) << "\n"; }

    //NOLINTNEXTLINE
    void VisitAInst(const Statement* stmt) { ss_ << "@" << stmt->token().string_view() << "\n"; }

    //NOLINTNEXTLINE
    void VisitCInst(const Statement* stmt)
    {
      using Type = Statement::Type;
      //ss_ << stmt->token().string_view();
      const auto& children = stmt->children();
      auto        begin    = children.begin();
      auto        end      = children.end();
      if (begin->isa(Type::Dest)) {
        ss_ << begin->token().string_view() << "=";
        ++begin;
      }

      for (; begin != end; ++begin) {
        if (begin->isa(Type::Comp)) {
          VisitExpr(begin.base());
        }

        if (begin->isa(Type::Jump)) {
          VisitJump(begin.base());
        }
      }
      ss_ << "\n";
    }

    //NOLINTNEXTLINE
    void VisitExpr(const Statement* stmt) { ss_ << stmt->token().string_view(); }

    //NOLINTNEXTLINE
    void VisitJump(const Statement* stmt)
    {
      using T = TokenType;
      ss_ << ";" << stmt->token().string_view();
    }
};

auto symbol_resolved_rewrite(Data& data) -> std::string
{
  SymbolResolvedRewriter writer;
  writer.start(data);
  return writer.string();
}

}   //namespace hack