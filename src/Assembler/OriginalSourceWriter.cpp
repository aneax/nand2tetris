#include "Ast.hpp"
#include "Lexer.hpp"
#include <fmt/core.h>

#include <sstream>
namespace hack
{

class SrcRewriter : public ConstStmtVisitor<SrcRewriter>
{
    using Base = ConstStmtVisitor<SrcRewriter>;
    std::stringstream ss_;

  public:
    SrcRewriter() = default;

    void dump() const noexcept { fmt::print("{}", ss_.str()); }

    auto string() const noexcept -> std::string { return ss_.str(); }

    void Visit(const StatementVector& stmts)
    {
      for (const auto& stmt : stmts) {
        Base::Visit(&stmt);
      }
    }

    //NOLINTNEXTLINE
    void VisitLabel(const Statement* stmt) { ss_ << "(" << stmt->token().string_view() << ")\n"; }

    //NOLINTNEXTLINE
    void VisitSymbol(const Statement* stmt) { ss_ << "@" << stmt->token().string_view() << "\n"; }

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
          //ss_ << ";";
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

auto src_rewrite(const StatementVector& stmts) -> std::string
{
  SrcRewriter writer;
  writer.Visit(stmts);
  return writer.string();
}

}   //namespace hack
