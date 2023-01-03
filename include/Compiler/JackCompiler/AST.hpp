#ifndef NAND2TETRIS_INCLUDE_COMPILER_AST_HPP
#define NAND2TETRIS_INCLUDE_COMPILER_AST_HPP

#include "JackCompiler/Token.hpp"
#include <vector>
#include <memory>

namespace jack
{

namespace internal
{

template <typename T>
struct SingleVisitor {
    virtual void visit(T*) = 0;
};

template <typename... T>
struct Visitor : public SingleVisitor<T>... {
    using SingleVisitor<T>::visit...;
};
}   //namespace internal

struct BinaryExpr;
struct Boolean;
struct BitWiseExpr;
struct Number;
struct Null;
struct This;
struct CallExpr;
struct GetExpr;
struct GetElementExpr;
struct Variable;
struct Char;
struct UnaryExpr;

using ExpressionVisitor =
  internal::Visitor<BinaryExpr, BitWiseExpr, CallExpr, GetExpr, GetElementExpr,
                    Number, Char, This, Null, Variable, Boolean, UnaryExpr>;

struct BlockStatement;
struct LetStatement;
struct VarDecl;
struct IfStatement;
struct WhileStatement;
struct DoStatement;
struct ReturnStatement;

using StatementVisitor =
  internal::Visitor<ReturnStatement, BlockStatement, LetStatement, VarDecl,
                    IfStatement, WhileStatement, DoStatement>;

struct Expr {
    Expr(Token tok) : token(tok) {}

    Token token;

    [[nodiscard]] auto type() const noexcept -> TokenType
    {
      return token.type();
    }

    [[nodiscard]] auto isa(TokenType ty) const noexcept -> bool
    {
      return ty == type();
    }

    virtual void accept(ExpressionVisitor& visitor) = 0;
    //virtual void accept(const ExpressionVisitor& visitor) = 0;
    virtual ~Expr() = default;
};

struct Statement {
    Token token;
    Statement(Token tok) : token(tok) {}

    virtual void accept(StatementVisitor& visitor) = 0;
    //virtual void accept(const StatementVisitor& visitor) = 0;
    virtual ~Statement() = default;
};

using UniqueStatement = std::unique_ptr<Statement>;
using Statements      = std::vector<UniqueStatement>;
using UniqueExpr      = std::unique_ptr<Expr>;

template <typename T>
struct VisitableExpr : public Expr {
    using Expr::Expr;
    void accept(ExpressionVisitor& visitor) override
    {
      visitor.visit(static_cast<T*>(this));
    }
};

template <typename T>
struct VisitableStmt : public Statement {
    using Statement::Statement;
    void accept(StatementVisitor& visitor) override
    {
      visitor.visit(static_cast<T*>(this));
    }
};

template <typename Derived>
struct BinaryExprImpl : VisitableExpr<Derived> {
    UniqueExpr lhs;
    UniqueExpr rhs;
    BinaryExprImpl(Token tok, UniqueExpr left, UniqueExpr right)
        : VisitableExpr<Derived>(tok)
        , lhs(std::move(left))
        , rhs(std::move(right))
    {
    }
};

template <typename Derived>
struct UnaryExprImpl : VisitableExpr<Derived> {
    UniqueExpr expr;
    UnaryExprImpl(Token tok, UniqueExpr exp)
        : VisitableExpr<Derived>(tok)
        , expr(std::move(exp))
    {
    }
};

struct BitWiseExpr : BinaryExprImpl<BitWiseExpr> {
    using BinaryExprImpl<BitWiseExpr>::BinaryExprImpl;
};

struct BinaryExpr : BinaryExprImpl<BinaryExpr> {
    using BinaryExprImpl<BinaryExpr>::BinaryExprImpl;
};

struct UnaryExpr : UnaryExprImpl<UnaryExpr> {
    using UnaryExprImpl<UnaryExpr>::UnaryExprImpl;
};

struct CallExpr : VisitableExpr<CallExpr> {
    std::vector<UniqueExpr> arguments;
    CallExpr(Token tok, std::vector<UniqueExpr> arguments)
        : VisitableExpr<CallExpr>(tok)
        , arguments(std::move(arguments))
    {
    }
};

struct GetExpr : VisitableExpr<GetExpr> {
    UniqueExpr call;
    GetExpr(Token source, UniqueExpr expr)
        : VisitableExpr<GetExpr>(source)
        , call(std::move(expr))
    {
    }
};

struct GetElementExpr : UnaryExprImpl<GetElementExpr> {
    using UnaryExprImpl<GetElementExpr>::UnaryExprImpl;
};

struct Boolean : VisitableExpr<Boolean> {
    using VisitableExpr<Boolean>::VisitableExpr;
};

struct Char : VisitableExpr<Char> {
    using VisitableExpr<Char>::VisitableExpr;
};

struct Number : VisitableExpr<Number> {
    using VisitableExpr<Number>::VisitableExpr;
};

struct Variable : VisitableExpr<Variable> {
    using VisitableExpr<Variable>::VisitableExpr;
};

struct Null : VisitableExpr<Null> {
    using VisitableExpr<Null>::VisitableExpr;
};

struct This : VisitableExpr<This> {
    using VisitableExpr<This>::VisitableExpr;
};

using ExpressionList = std::vector<Expr>;

template <typename Derived>
struct Common {
    using type = Derived;

    Token token;
    Common(Token token) : token(token){};
};

struct Identifier : Common<Identifier> {
    using Common<Identifier>::Common;
};
using IdentifierVector = std::vector<Identifier>;

struct TypeDecl : Common<TypeDecl> {
    using Common<TypeDecl>::Common;
};

struct VarDecl : VisitableStmt<VarDecl> {
    TypeDecl   type_decl;
    Identifier var_name;
    VarDecl(Token token, TypeDecl type, Identifier name)
        : VisitableStmt<VarDecl>(token)
        , type_decl(type)
        , var_name(name)
    {
    }
};

struct Parameter : VarDecl {
    using VarDecl::VarDecl;
};

using ParameterList = std::vector<Parameter>;

struct BlockStatement : VisitableStmt<BlockStatement> {
    Statements stmts;
    BlockStatement(Token tok, Statements statements)
        : VisitableStmt<BlockStatement>(tok)
        , stmts(std::move(statements))
    {
    }

    BlockStatement() : VisitableStmt<BlockStatement>(constants::NullToken) {}
};

struct LetStatement : VisitableStmt<LetStatement> {
    Identifier name;
    UniqueExpr index;
    UniqueExpr expr;
    LetStatement(Token tok, Identifier id, UniqueExpr index, UniqueExpr expr)
        : VisitableStmt<LetStatement>(tok)
        , name(id)
        , index(std::move(index))
        , expr(std::move(expr))
    {
    }
};

struct IfStatement : VisitableStmt<IfStatement> {
    UniqueExpr      cond;
    UniqueStatement then_block;
    UniqueStatement else_block;
    IfStatement(Token tok, UniqueExpr condition, UniqueStatement b_then,
                UniqueStatement b_else)
        : VisitableStmt<IfStatement>(tok)
        , cond(std::move(condition))
        , then_block(std::move(b_then))
        , else_block(std::move(b_else))
    {
    }
};
struct WhileStatement : VisitableStmt<WhileStatement> {
    UniqueExpr      condition;
    UniqueStatement block;
    WhileStatement(Token tok, UniqueExpr cond, UniqueStatement body)
        : VisitableStmt<WhileStatement>(tok)
        , condition(std::move(cond))
        , block(std::move(body))
    {
    }
};

struct DoStatement : VisitableStmt<DoStatement> {
    UniqueExpr callexpr;
    DoStatement(Token tok, UniqueExpr expr)
        : VisitableStmt<DoStatement>(tok)
        , callexpr(std::move(expr))
    {
    }
};
struct ReturnStatement : VisitableStmt<ReturnStatement> {
    UniqueExpr expr;
    ReturnStatement(Token tok, UniqueExpr expr)
        : VisitableStmt<ReturnStatement>(tok)
        , expr(std::move(expr))
    {
    }
};

struct SubRoutineDecl {
    Token           routine_type;
    TypeDecl        return_type;
    Identifier      function_name;
    ParameterList   parameters;
    UniqueStatement function_body;

    SubRoutineDecl(Token token, TypeDecl rtype, Identifier name,
                   ParameterList parameters, UniqueStatement fbody)
        : routine_type(token)
        , return_type(rtype)
        , function_name(name)
        , parameters(std::move(parameters))
        , function_body(std::move(fbody))
    {
    }
};

using VarDeclVector        = std::vector<std::unique_ptr<VarDecl>>;
using SubRoutineDeclVector = std::vector<SubRoutineDecl>;

struct ClassDecl {
    Token                token;
    Identifier           identifier;
    VarDeclVector        var_decls;
    SubRoutineDeclVector sub_decls;
    ClassDecl(Token token, Identifier identifier, VarDeclVector&& vars,
              SubRoutineDeclVector&& subs)
        : token(token)
        , identifier(identifier)
        , var_decls(std::move(vars))
        , sub_decls(std::move(subs))
    {
    }
};

}   //namespace jack

#endif   //!NAND2TETRIS_INCLUDE_COMPILER_AST_HPP
