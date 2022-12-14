#ifndef MINI_CUSTOM_INCLUDE_AST_HPP
#define MINI_CUSTOM_INCLUDE_AST_HPP

#include "FwdDecl.hpp"
#include "Errors.hpp"
#include "Lexer.hpp"
#include "Traits.hpp"
namespace hack
{

class Statement;
using StatementVector = std::vector<Statement>;

class Statement
{
  public:
    enum class Type {
      Invalid,
      Label,
      Symbol,
      AInst,
      CInst,
      Dest,
      Comp,
      Jump
    };

    Statement() : Statement(Type::Invalid, constants::EmptyToken) {}
    Statement(Type type, Token token) : Statement(type, token, {}) {}
    Statement(Type type, Token token, StatementVector children)
        : type_(type)
        , token_(token)
        , children_(std::move(children))
    {
    }

    void set_children(StatementVector children) { children_ = std::move(children); }

    [[nodiscard]] auto child(size_t index) const -> const Statement& { return children_.at(index); }
    [[nodiscard]] auto token() const noexcept -> const Token& { return token_; }
    [[nodiscard]] auto node_type() const noexcept -> Type { return type_; }
    [[nodiscard]] auto children() const noexcept -> StatementVector { return children_; }
    [[nodiscard]] auto is_invalid() const noexcept -> bool { return type_ == Type::Invalid; }
    [[nodiscard]] auto isa(Type type) const noexcept -> bool { return type_ == type; }

  private:
    Type  type_;
    Token token_;

    StatementVector children_;
};

template <template <typename> typename Pointer, typename ImplClass, typename ReturnType = void,
          typename... Params>
class VisitorBase
{
  public:
    using PtrType = typename Pointer<Statement>::type;
#define DISPATCH(NAME) \
  case T::NAME:        \
    return static_cast<ImplClass*>(this)->Visit##NAME(stmt, std::forward<Params>(params)...)
    //NOLINTNEXTLINE
    auto Visit(PtrType stmt, Params... params) -> ReturnType
    {
      using T = Statement::Type;
      switch (stmt->node_type()) {
        DISPATCH(Label);
        DISPATCH(Symbol);
        DISPATCH(AInst);
        DISPATCH(CInst);
        default:
          throw RuntimeError("Unreachable region.");
      }
    }

    //No Fallbacks.

#undef DISPATCH
};

template <typename ImplClass, typename ReturnType = void, typename... Params>
class StatementVisitor : public VisitorBase<std::add_pointer, ImplClass, ReturnType, Params...>
{
};

template <typename ImplClass, typename ReturnType = void, typename... Params>
class ConstStmtVisitor : public VisitorBase<make_const_pointer, ImplClass, ReturnType, Params...>
{
};
}   //namespace hack

#endif