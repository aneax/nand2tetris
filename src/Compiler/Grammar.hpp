#ifndef NAND2TETRIS_SOURCE_COMPILER_GRAMMAR_HPP
#define NAND2TETRIS_SOURCE_COMPILER_GRAMMAR_HPP

#include "Elements.hpp"
#include <ConstantString.hpp>
#include <tao/pegtl/rules.hpp>
#include <tao/pegtl.hpp>

namespace hack::grammar
{

using namespace TAO_PEGTL_NAMESPACE;
struct CloseParantheses : one<elements::ParenClose> {
    //static constexpr TokenType type = TokenType::ParenClose;
};
struct OpenParantheses : one<elements::ParenOpen> {
};
struct OpenBraces : one<elements::BraceOpen> {
};
struct CloseBraces : one<elements::BraceClose> {
};
struct OpenBrackets : one<elements::BracketOpen> {
};
struct CloseBrackets : one<elements::BracketClose> {
};
struct AngleOpen : one<elements::AngleOpen> {
};
struct AngleClose : one<elements::AngleClose> {
};
struct Comma : one<elements::Comma> {
};
struct Semicolon : one<elements::Semicolon> {
};
struct Colon : one<elements::Colon> {
};
struct Dot : one<elements::Dot> {
};
struct Equals : one<elements::Equals> {
};
struct Plus : one<elements::Plus> {
};
struct Minus : one<elements::Minus> {
};
struct Slash : one<elements::Slash> {
};
struct Asterisk : one<elements::Asterisk> {
};
struct Ampersand : one<elements::Ampersand> {
};
struct Tilde : one<elements::Tilde> {
};
struct Pipe : one<elements::Pipe> {
};
struct SingleQuote : one<elements::SingleQuote> {
};
struct DoubleQuote : one<elements::DoubleQuote> {
};
struct Not : one<elements::Exclamation> {
};

struct Class : TAO_PEGTL_STRING("class") {
};
struct Constructor : TAO_PEGTL_STRING("constructor") {
};
struct Method : TAO_PEGTL_STRING("method") {
};
struct Function : TAO_PEGTL_STRING("function") {
};
struct Int : TAO_PEGTL_STRING("int") {
};
struct Boolean : TAO_PEGTL_STRING("boolean") {
};
struct Char : TAO_PEGTL_STRING("char") {
};
struct Void : TAO_PEGTL_STRING("void") {
};
struct Var : TAO_PEGTL_STRING("var") {
};
struct Static : TAO_PEGTL_STRING("static") {
};
struct Field : TAO_PEGTL_STRING("field") {
};
struct Let : TAO_PEGTL_STRING("let") {
};
struct Do : TAO_PEGTL_STRING("do") {
};
struct If : TAO_PEGTL_STRING("if") {
};
struct Else : TAO_PEGTL_STRING("else") {
};
struct While : TAO_PEGTL_STRING("while") {
};
struct Return : TAO_PEGTL_STRING("return") {
};
struct True : TAO_PEGTL_STRING("true") {
};
struct False : TAO_PEGTL_STRING("false") {
};
struct Null : TAO_PEGTL_STRING("null") {
};
struct This : TAO_PEGTL_STRING("this") {
};
// clang-format on

//Constants
//- Integers
//- Boolean
//- String
//Identifiers
//- [A-Za-z0-9_]
//
struct Space : plus<space> {
};

//TODO: handle / for division
struct CommentA : seq<Slash, until<eolf>> {
};
struct CommentB : seq<Slash, Asterisk, until<seq<Asterisk, Slash>>> {
};
struct CommentImpl : if_then_else<seq<Slash, at<Slash>>, CommentA, CommentB> {
};

struct Comment : seq<at<Slash, sor<Slash, Asterisk>>, CommentImpl> {
};

struct Keywords : pad<sor<Class, Constructor, Method, Function, Int, Boolean,
                          Char, Void, Var, Static, Field, Let, Do, If, Else,
                          While, Return, True, False, Null, This>,
                      Space> {
};

struct Symbols : pad<sor<CloseParantheses, OpenParantheses, OpenBraces,
                         CloseBraces, OpenBrackets, CloseBrackets, AngleOpen,
                         AngleClose, Comma, Semicolon, Colon, Dot, Equals, Plus,
                         Minus, Slash, Asterisk, Ampersand, Tilde, Pipe, Not>,
                     Space> {
};

//NOLINTNEXTLINE
struct Integer : plus<digit> {
};

struct PaddedInteger : pad<Integer, Space> {
};

struct String : until<at<DoubleQuote>> {
};

struct PaddedString : pad<seq<DoubleQuote, String, DoubleQuote>, Space> {
};

struct Identifier : pad<identifier, Space> {
};

struct Tokens : star<sor<Space, Comment, Keywords, Symbols, PaddedInteger,
                         Identifier, PaddedString, eol>> {
};

template <typename Rule>
struct TokenAction : nothing<Rule> {
};

enum class TokenClass { Keyword, Symbol, Integer, Identifier, String };

template <ConstantString Str>
struct TokenActionBase {
    template <typename ActionInput, typename Callable>
    static void apply(const ActionInput& in, Callable callable)
    {
      callable(std::string_view(Str.value.begin(), Str.value.end()), in);
    }
};

template <>
struct TokenAction<Keywords> : TokenActionBase<"keyword"> {
};

template <>
struct TokenAction<Symbols> : TokenActionBase<"symbol"> {
};

template <>
struct TokenAction<Integer> : TokenActionBase<"integer"> {
};

template <>
struct TokenAction<String> : TokenActionBase<"string"> {
};

template <>
struct TokenAction<Identifier> : TokenActionBase<"identifier"> {
};
}   //namespace hack::grammar

namespace hack::grammar_v2
{

namespace hg = hack::grammar;
using namespace TAO_PEGTL_NAMESPACE;

struct Statements;
struct Expression;
struct SubRoutineCall;

template <typename Rule>
struct Paranthesized : seq<hg::OpenParantheses, Rule, hg::CloseParantheses> {
};

template <typename Rule>
struct Bracketed : seq<hg::OpenBrackets, Rule, hg::CloseBrackets> {
};

template <typename Rule>
struct Braced : seq<hg::OpenBraces, Rule, hg::CloseBraces> {
};

struct VarName : hg::Identifier {
};

struct ClassName : hg::Identifier {
};

struct SubRoutineName : hg::Identifier {
};

struct TypeDecl : sor<hg::Int, hg::Char, hg::Boolean, ClassName> {
};

struct VarDecl
    : seq<hg::Var, TypeDecl, list<VarName, hg::Comma>, hg::Semicolon> {
};

struct ClassVarDecl : seq<sor<hg::Static, hg::Field>, TypeDecl,
                          list<VarName, hg::Comma>, hg::Semicolon> {
};

struct SubRoutineBody : Braced<star<sor<VarDecl, Statements>>> {
};

struct ParameterList : list<seq<TypeDecl, VarName>, hg::Comma> {
};

struct SubRoutineDecl : seq<sor<hg::Constructor, hg::Function, hg::Method>,
                            sor<hg::Void, TypeDecl>, SubRoutineName,
                            Paranthesized<opt<ParameterList>>, SubRoutineBody> {
};

struct ClassDecl : seq<hg::Class, hg::Identifier,
                       Braced<star<sor<ClassVarDecl, SubRoutineDecl>>>> {
};

////Statements

struct StatementBody : Braced<Statements> {
};

struct doStatement : seq<hg::Do, SubRoutineCall, hg::Comma> {
};

struct returnStatement : seq<hg::Return, opt<Expression>, hg::Semicolon> {
};

struct whileStatement
    : seq<hg::While, Paranthesized<Expression>, StatementBody> {
};

struct ifStatement : seq<hg::If, Paranthesized<Expression>, StatementBody,
                         opt<hg::Else, StatementBody>> {
};

struct letStatement : seq<hg::Let, VarName, opt<Bracketed<Expression>>,
                          hg::Equals, Expression, hg::Semicolon> {
};

struct Statement : sor<letStatement, ifStatement, whileStatement, doStatement,
                       returnStatement> {
};

struct Statements : star<Statement, eol> {
};

//////Expressions

struct ConstantKeywords : sor<hg::True, hg::False, hg::Null, hg::This> {
};

struct UnaryOperator : sor<hg::Minus, hg::Tilde> {
};

struct Operators
    : sor<hg::Plus, hg::Minus, hg::Asterisk, hg::Slash, hg::Ampersand, hg::Pipe,
          hg::AngleOpen, hg::AngleClose, hg::Equals> {
};

struct ExpressionList : opt<list<Expression, hg::Comma>> {
};

struct SubRoutineCall
    : sor<seq<SubRoutineName, Paranthesized<ExpressionList>>,
          seq<sor<ClassName, VarName>, hg::Dot, SubRoutineName,
              Paranthesized<ExpressionList>>> {
};

struct Term
    : sor<hg::PaddedInteger, hg::PaddedString, ConstantKeywords,
          seq<VarName, opt<Bracketed<Expression>>>, seq<UnaryOperator, Term>,
          seq<SubRoutineCall>, Paranthesized<Expression>> {
};

struct Expression : list<Term, Operators> {
};

}   //namespace hack::grammar_v2

#endif   //!NAND2TETRIS_SOURCE_COMPILER_HPP
