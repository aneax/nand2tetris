
#include "FwdDecl.hpp"
#include "Lexer.hpp"
#include "Ast.hpp"
#include "Parser.hpp"
#include "Errors.hpp"
#include <cstddef>
#include <optional>

namespace hack
{

class Parser
{
  public:
    using T        = TokenType;
    using ST       = Statement::Type;
    using Iterator = TokenVector::iterator;
    Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)), current_(tokens_.begin()) {}

    auto parse() -> Data;

  private:
    auto require_match_(TokenType type, const char* error_message) -> Token;
    auto consume_() -> Token;
    auto advance_(size_t n = 1) -> void;
    void check_errors_() { errorHandler_.check_errors(); }

    template <typename... Ts>
    [[nodiscard]] auto match_(Ts... types) const -> bool;

    [[nodiscard]] auto match_(TokenType type) const -> bool;
    [[nodiscard]] auto is_end_() const noexcept -> bool;
    [[nodiscard]] auto current_type_() const noexcept -> TokenType;
    [[nodiscard]] auto current_token_() const noexcept -> Token;

    void register_error_(std::string&& error)
    {
      errorHandler_.log(std::forward<std::string>(error));
    }

    auto statements_() -> Statement;
    auto symbol_() -> Statement;
    auto annotation_() -> Statement;
    auto label_() -> Statement;
    auto expression_(size_t line) -> Statement;
    auto a_inst_() -> Statement;
    auto c_inst_() -> Statement;

    TokenVector     tokens_;
    SymbolTable     labels_;
    StatementVector stmts_;
    ErrorReporter   errorHandler_;
    Iterator        current_;
};

auto Parser::current_type_() const noexcept -> TokenType
{
  return current_->type();
}

auto Parser::current_token_() const noexcept -> Token
{
  return *(current_);
}

auto Parser::advance_(size_t n) -> void
{
  std::advance(current_, n);
}

auto Parser::consume_() -> Token
{
  auto val = current_;
  advance_();
  return *val;
}

auto Parser::match_(TokenType type) const -> bool
{
  if (is_end_()) {
    return false;
  }

  return type == current_type_();
}

template <typename... Ts>
auto Parser::match_(Ts... types) const -> bool
{
  return (match_(types) || ...);
}

auto Parser::statements_() -> Statement
{
  switch (current_type_()) {
    case T::At:
      return annotation_();
    case T::LParantheses:
      return label_();
    default:
      return c_inst_();
  }
}

auto Parser::is_end_() const noexcept -> bool
{
  return current_type_() == T::FEOF;
}

auto Parser::require_match_(TokenType type, const char* error_message) -> Token
{
  if (!match_(type)) {
    std::string msg = error_message;
    msg.append(". Before: ");
    msg.append(to_string(current_token_()));
    errorHandler_.log(std::move(msg));
    check_errors_();
    return current_token_();
  };
  return consume_();
}

/**
 * @brief  @value
 *
 *
 * @return Statement
 */

auto Parser::annotation_() -> Statement
{
  require_match_(T::At, "");
  if (match_(T::Number)) {
    return a_inst_();
  }

  return symbol_();
}

auto Parser::label_() -> Statement
{
  consume_();                //consume (
  auto token = consume_();   //label identifier
  auto stmt  = Statement{ST::Label, token};
  require_match_(T::RParantheses, "Expects )");
  if (labels_.contains(token.string())) {
    throw ParseError("Multiple Label definitions: " + token.string());
  }
  labels_.insert({token.string(), stmts_.size() - labels_.size()});
  return stmt;
}

auto Parser::a_inst_() -> Statement
{
  return {ST::AInst, consume_()};
}

auto Parser::c_inst_() -> Statement
{
  StatementVector children;

  auto   left = consume_();
  size_t line = left.begin().line;

  ST type = ST::Dest;

  if (match_(T::Equal)) {
    children.emplace_back(type, left);
    consume_();
    children.emplace_back(expression_(line));
  } else {
    children.emplace_back(ST::Comp, left);
  }

  if (match_(T::SemiColon)) {
    consume_();                                    //consume semicolon
    children.emplace_back(ST::Jump, consume_());   //consume jump
    assert(current_token_().line() != line && "Must be new line");
  }

  return {ST::CInst, left, std::move(children)};
}

auto Parser::expression_(size_t line) -> Statement
{
  Statement              parent;
  std::vector<Statement> children;

  auto begin = current_;
  auto end   = begin;
  while (line == current_token_().line() && !match_(T::SemiColon) && !is_end_()) {
    //if (match_(T::Identifier, T::Number)) {
    //  children.emplace_back(ST::Expr, consume_());
    //} else {
    //  //is operator
    //  if (!children.empty()) {
    //    parent = Statement(ST::BinaryOp, consume_());
    //  } else {
    //    parent = Statement(ST::UnaryOp, consume_());
    //  }
    //  auto expr = expression_(line);
    //  children.push_back(expr);
    //}
    end = current_;
    advance_();
  }

  Token token(begin->type(), begin->begin(), end->end());
  //if (children.size() == 1 && parent.is_invalid()) {
  //  return children[0];
  //}

  //parent.set_children(std::move(children));

  return {ST::Comp, token};
}

auto Parser::symbol_() -> Statement
{
  return Statement{ST::Symbol, consume_()};
}

auto Parser::parse() -> Data
{
  while (!is_end_()) {
    stmts_.push_back(statements_());
  }

  return {.stmts = std::move(stmts_), .symbols = std::move(labels_)};
}

auto parse(const TokenVector& token) -> Data
{
  Parser parser(token);
  return parser.parse();
}
}   //namespace hack