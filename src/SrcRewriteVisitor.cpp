#include "Ast.hpp"
#include "Errors.hpp"
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
      ss_ << stmt->token().string_view();
      Visit(stmt->children());
      ss_ << "\n";
    }

    //NOLINTNEXTLINE
    void VisitExpr(const Statement* stmt) { ss_ << stmt->token().string_view(); }

    //NOLINTNEXTLINE
    void VisitBinaryOp(const Statement* stmt)
    {
      using T = TokenType;
      switch (stmt->token().type()) {
        case T::Add:
        case T::Sub:
        case T::And:
        case T::Or:
        case T::Not:
          Base::Visit(&stmt->child(0));
          ss_ << stmt->token().string_view();
          Base::Visit(&stmt->child(1));
          break;
        case T::Equal:
          ss_ << stmt->token().string_view();
          break;
        default:
          ss_ << "Error: " << stmt->token().string_view();
          //throw RuntimeError("Op: Unhandled Operator");
      }
    }

    //NOLINTNEXTLINE
    void VisitUnaryOp(const Statement* stmt)
    {
      using T = TokenType;

      ss_ << stmt->token().string_view();
      Visit(stmt->children());
    }
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
