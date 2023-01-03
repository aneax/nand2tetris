#include "JackCompiler/Parser.hpp"
#include "Characters.hpp"
#include "JackCompiler/AST.hpp"
#include "JackCompiler/Token.hpp"
#include "JackCompiler/Lexer.hpp"
#include "Reader.hpp"
#include <iostream>
#include <memory>
#include <stdexcept>

using TokenType = jack::TokenType;
using Token     = jack::Token;

using namespace jack;

inline auto is_class_decl(const Token& token) noexcept -> bool
{
  using T = TokenType;
  return is_type<T::Static, T::Field>(token.type());
}

inline auto is_subroutine_decl(const Token& token) noexcept -> bool
{
  using T = TokenType;
  return is_type<T::Constructor, T::Function, T::Method>(token.type());
}

template <typename T, typename... Ts>
constexpr auto is_any_one_of(T ty, Ts... tys) -> bool
{
  return ((ty == tys) || ...);
}

template <TokenType T>
concept IsClassVarToken = is_any_one_of(T, TokenType::Static, TokenType::Field);

template <TokenType T>
concept IsVarToken = IsClassVarToken<T> || (T == TokenType::Var);

template <TokenType T>
concept IsOpenBracket = is_any_one_of(T, TokenType::LeftBracket,
                                      TokenType::LeftParen,
                                      TokenType::LeftBrace);

template <TokenType T>
concept IsCloseBracket = is_any_one_of(T, TokenType::RightBracket,
                                       TokenType::RightParen,
                                       TokenType::RightBrace);

template <TokenType Type>
constexpr auto closer()
{
  if constexpr (Type == TokenType::LeftBrace) {
    return TokenType::RightBrace;
  }

  if constexpr (Type == TokenType::LeftParen) {
    return TokenType::RightParen;
  }

  if constexpr (Type == TokenType::LeftBracket) {
    return TokenType::RightBracket;
  }
  return TokenType::Eol;
}

template <TokenType Left, TokenType Right>
concept Closes = requires() { closer<Left>() == Right; };

class Parser
{
    using TokenVector = std::vector<Token>;

  public:
    Parser(hack::MemoryInput& input)
        : lex_(input)
        , tokens_(lex_.lex())
        , current_(tokens_.cbegin())
    {
    }

    auto parse() -> std::vector<ClassDecl>;

  private:
    auto token_() -> Token
    {
      auto tok = *current_;
      std::cout << "Consumed: " << tok << "\n";
      ++current_;
      return tok;
    }
    auto peek_() -> Token { return *current_; };
    auto peek_type_() -> TokenType { return peek_().type(); }

    template <TokenType... TTypes>
    auto match_() -> bool
    {
      static_assert(sizeof...(TTypes) > 0,
                    "Should have at least one template parameter");
      auto ctype = peek_type_();
      return ((ctype == TTypes) || ...);
    }

    template <TokenType... TTypes>
    auto require_(const Token& tok, std::string_view err_message) -> void
    {
      static_assert(sizeof...(TTypes) > 0,
                    "There must be at least one template parameter");
      auto statisfied = is_any_one_of(tok.type(), TTypes...);
      if (!statisfied) {
        std::stringstream err;
        err << "Expected one of [";
        ((err << TTypes << " "), ...);
        err << "]. Got " << tok << ". ";
        err << err_message << "\n";
        throw std::runtime_error(err.str());
      }
    }

    template <TokenType TType>
    auto consume_(std::string_view err_message) -> bool
    {
      auto tok = token_();
      require_<TType>(tok, "");
      return true;
    }

    auto parse_identifier_() -> Identifier;

    template <bool HasVoid = false>
    auto parse_type_() -> TypeDecl;

    template <TokenType... TTypes>
    requires(IsVarToken<TTypes> && ...)
    auto parse_var_decl_impl_(VarDeclVector& vars)
    {
      static_assert(sizeof...(TTypes) > 0, "Token type must be greater");

      auto tok = token_();
      require_<TTypes...>(tok, "Expected static or field");
      auto type_decl  = parse_type_();
      auto identifier = parse_identifier_();
      vars.emplace_back(std::make_unique<VarDecl>(tok, type_decl, identifier));
      while (peek_type_() != TokenType::Semicolon) {
        consume_<TokenType::Comma>("Expected Comma");
        auto identifier = parse_identifier_();
        vars.emplace_back(
          std::make_unique<VarDecl>(tok, type_decl, identifier));
      }
      (void)consume_<TokenType::Semicolon>("Expected semicolon");
    }

    auto parse_class_decl_() -> ClassDecl;
    auto parse_class_var_decl_(VarDeclVector& member_variables) -> void;
    auto parse_subroutine_decl_() -> SubRoutineDecl;
    auto parse_parameter_list_() -> ParameterList;
    auto parse_var_decl_() -> VarDeclVector;

    auto parse_statements_(TokenType until) -> Statements;
    auto parse_let_stmt_() -> UniqueStatement;
    auto parse_while_stmt_() -> UniqueStatement;
    auto parse_if_stmt_() -> UniqueStatement;
    auto parse_do_stmt_() -> UniqueStatement;
    auto parse_return_stmt_() -> UniqueStatement;
    auto parse_block_stmt_() -> UniqueStatement;

    auto parse_expression_() -> UniqueExpr;

    template <bool OnlyCalls = false>
    auto parse_call_() -> UniqueExpr;

    auto parse_call_arguments_() -> std::vector<UniqueExpr>;
    auto parse_unary_expr_() -> UniqueExpr;
    auto parse_or_expr_() -> UniqueExpr;
    auto parse_and_expr_() -> UniqueExpr;
    auto parse_equality_() -> UniqueExpr;
    auto parse_factor_expr_() -> UniqueExpr;
    auto parse_term_expr_() -> UniqueExpr;
    auto parse_comparison_expr_() -> UniqueExpr;
    auto parse_equality_expr_() -> UniqueExpr;
    auto parse_terminal_() -> UniqueExpr;
    auto parse_bracketed_expression_() -> UniqueExpr;

    template <TokenType Open, TokenType Close, typename Apply>
    requires IsOpenBracket<Open> && IsCloseBracket<Close>
          && Closes<Open, Close>
             auto parse_bracketed_(Apply apply) -> decltype(apply())
    {
      consume_<Open>("Expected open bracket");
      auto expr = apply();
      consume_<Close>("Expected close bracket");
      return expr;
    };

    jack::Lexer lex_;

    TokenVector tokens_;

    TokenVector::const_iterator current_;
};

auto Parser::parse_identifier_() -> Identifier
{
  auto identifier = token_();
  require_<TokenType::Identifier>(identifier, "Expected idenfitifer");
  return {identifier};
}

template <bool HasVoid>
auto Parser::parse_type_() -> TypeDecl
{
  auto type = token_();
  using T   = TokenType;
  //Class can be type which is denoted by Identifier.
  if constexpr (HasVoid) {
    require_<T::Int, T::Char, T::Boolean, T::Void, T::Identifier>(
      type, "Expected type identifier");
  } else {
    require_<T::Int, T::Char, T::Boolean, T::Identifier>(
      type, "Expected type identifier");
  }
  return {type};
}

auto Parser::parse_class_var_decl_(VarDeclVector& member_variables) -> void
{
  parse_var_decl_impl_<TokenType::Static, TokenType::Field>(member_variables);
}

auto Parser::parse_parameter_list_() -> ParameterList
{
  consume_<TokenType::LeftParen>("Expected '('");
  ParameterList parameters;
  if (!match_<TokenType::RightParen>()) {
    while (true) {
      auto type = parse_type_();
      auto name = parse_identifier_();
      parameters.emplace_back(type.token, type, name);
      if (!match_<TokenType::Comma>()) {
        break;
      }
    }
  }
  consume_<TokenType::RightParen>("Expected ')'");
  return std::move(parameters);
}

auto Parser::parse_subroutine_decl_() -> SubRoutineDecl
{
  auto tok = token_();
  require_<TokenType::Constructor, TokenType::Function, TokenType::Method>(
    tok, "Expected function identifier");
  auto return_type_decl = parse_type_<true>();
  auto function_name    = parse_identifier_();
  auto parameters       = parse_parameter_list_();
  auto function_body    = parse_block_stmt_();
  return {tok, return_type_decl, function_name, std::move(parameters),
          std::move(function_body)};
}

auto Parser::parse_statements_(TokenType until) -> Statements
{
  Statements stmts;

  auto next = peek_type_();
  while (next != until) {
    switch (next) {
      case TokenType::Var:
        {
          auto var_decls = parse_var_decl_();
          for (auto& var_decl : var_decls) {
            stmts.emplace_back(std::move(var_decl));
          }
          break;
        }
      case TokenType::Let:
        {
          auto let = parse_let_stmt_();
          stmts.emplace_back(std::move(let));
          break;
        }
      case TokenType::If:
        stmts.emplace_back(parse_if_stmt_());
        break;
      case TokenType::While:
        stmts.emplace_back(parse_while_stmt_());
        break;
      case TokenType::Do:
        stmts.emplace_back(parse_do_stmt_());
        break;
      case TokenType::Return:
        stmts.emplace_back(parse_return_stmt_());
        break;
      case TokenType::LeftBrace:
        stmts.emplace_back(parse_block_stmt_());
        break;
      default:
        throw std::runtime_error("Parse Error. Unreachable region");
    }
    next = peek_type_();
  }
  return stmts;
};

auto Parser::parse_block_stmt_() -> UniqueStatement
{
  auto block = parse_bracketed_<TokenType::LeftBrace, TokenType::RightBrace>(
    [&] { return parse_statements_(TokenType::RightBrace); });
  return std::make_unique<BlockStatement>(constants::NullToken,
                                          std::move(block));
}

auto Parser::parse_let_stmt_() -> UniqueStatement
{
  auto let_tok = token_();
  require_<TokenType::Let>(let_tok, "Expect 'let' keyword");
  auto       name  = parse_identifier_();
  UniqueExpr index = nullptr;
  if (match_<TokenType::LeftBracket>()) {
    index = parse_bracketed_<TokenType::LeftBracket, TokenType::RightBracket>(
      [&] { return parse_expression_(); });
  }
  consume_<TokenType::EQ>("Expected equal symbol");
  auto expr = parse_expression_();
  consume_<TokenType::Semicolon>("Expected semicolon");
  return std::make_unique<LetStatement>(let_tok, name, std::move(index),
                                        std::move(expr));
}

auto Parser::parse_if_stmt_() -> UniqueStatement
{
  auto if_tok = token_();
  require_<TokenType::If>(if_tok, "Expected 'if' keyword");
  auto cond       = parse_bracketed_expression_();
  auto then_block = parse_block_stmt_();

  UniqueStatement else_block;
  if (match_<TokenType::Else>()) {
    auto else_tok = token_();
    else_block    = std::move(parse_block_stmt_());
  }
  return std::make_unique<IfStatement>(
    if_tok, std::move(cond), std::move(then_block), std::move(else_block));
}

auto Parser::parse_while_stmt_() -> UniqueStatement
{
  auto tok = token_();
  require_<TokenType::While>(tok, "Expected 'while' keyword");
  auto expr = parse_bracketed_expression_();
  auto body = parse_block_stmt_();
  return std::make_unique<WhileStatement>(tok, std::move(expr),
                                          std::move(body));
}

auto Parser::parse_do_stmt_() -> UniqueStatement
{
  auto tok = token_();
  require_<TokenType::Do>(tok, "Expected 'do' statement");
  auto expr = parse_call_();
  consume_<TokenType::Semicolon>("Expected ';'");
  return std::make_unique<DoStatement>(tok, std::move(expr));
}

auto Parser::parse_return_stmt_() -> UniqueStatement
{
  auto       tok  = token_();
  UniqueExpr expr = nullptr;
  require_<TokenType::Return>(tok, "Expected return statement");
  if (!match_<TokenType::Semicolon>()) {
    expr = parse_expression_();
  }
  consume_<TokenType::Semicolon>("Expected  semicolon");
  return std::make_unique<ReturnStatement>(tok, std::move(expr));
}

auto Parser::parse_var_decl_() -> VarDeclVector
{
  VarDeclVector vars;
  parse_var_decl_impl_<TokenType::Var>(vars);
  return std::move(vars);
};

auto Parser::parse_class_decl_() -> ClassDecl
{
  auto tok = token_();
  require_<TokenType::Class>(tok, "Expected class ");
  auto identifier = parse_identifier_();
  consume_<TokenType::LeftBrace>("Expected left brace");
  VarDeclVector        var_decls;
  SubRoutineDeclVector sub_decls;

  TokenType next = peek_type_();
  while (next != TokenType::RightBrace) {
    switch (next) {
      case TokenType::Static:
      case TokenType::Field:
        parse_class_var_decl_(var_decls);
        break;
      case TokenType::Constructor:
      case TokenType::Function:
      case TokenType::Method:
        sub_decls.push_back(std::move(parse_subroutine_decl_()));
        break;
      default:
        throw std::runtime_error("Unreachable. TODO: Handle error");
    }
    next = peek_type_();
  }
  auto rb_token    = token_();
  auto class_token = Token(TokenType::Class, tok.begin(), rb_token.end());

  return {class_token, identifier, std::move(var_decls), std::move(sub_decls)};
}

auto Parser::parse_or_expr_() -> UniqueExpr
{
  auto expr = parse_and_expr_();
  while (match_<TokenType::Or>()) {
    auto tok   = token_();
    auto right = parse_and_expr_();
    expr =
      std::make_unique<BitWiseExpr>(tok, std::move(expr), std::move(right));
  }
  return expr;
}
auto Parser::parse_and_expr_() -> UniqueExpr
{
  auto expr = parse_equality_expr_();
  while (match_<TokenType::And>()) {
    auto tok   = token_();
    auto right = parse_equality_expr_();
    expr =
      std::make_unique<BitWiseExpr>(tok, std::move(expr), std::move(right));
  }
  return expr;
}

auto Parser::parse_equality_expr_() -> UniqueExpr
{
  auto expr = parse_comparison_expr_();
  while (match_<TokenType::EQ>()) {
    auto tok   = token_();
    auto right = parse_comparison_expr_();
    expr = std::make_unique<BinaryExpr>(tok, std::move(expr), std::move(right));
  }
  return expr;
}

auto Parser::parse_comparison_expr_() -> UniqueExpr
{
  auto expr = parse_term_expr_();
  while (match_<TokenType::LT, TokenType::GT>()) {
    auto tok   = token_();
    auto right = parse_term_expr_();
    expr = std::make_unique<BinaryExpr>(tok, std::move(expr), std::move(right));
  }
  return expr;
}

auto Parser::parse_term_expr_() -> UniqueExpr
{
  auto expr = parse_factor_expr_();
  while (match_<TokenType::Plus, TokenType::Minus>()) {
    auto tok   = token_();
    auto right = parse_factor_expr_();
    expr = std::make_unique<BinaryExpr>(tok, std::move(expr), std::move(right));
  }
  return expr;
}
auto Parser::parse_factor_expr_() -> UniqueExpr
{
  auto expr = parse_unary_expr_();
  while (match_<TokenType::Mul, TokenType::Div>()) {
    auto tok   = token_();
    auto right = parse_unary_expr_();
    expr = std::make_unique<BinaryExpr>(tok, std::move(expr), std::move(right));
  }
  return expr;
}
auto Parser::parse_unary_expr_() -> UniqueExpr
{
  if (match_<TokenType::Minus, TokenType::Not>()) {
    auto tok  = token_();
    auto expr = parse_unary_expr_();
    return std::make_unique<UnaryExpr>(tok, std::move(expr));
  }
  return parse_terminal_();
}

auto Parser::parse_call_arguments_() -> std::vector<UniqueExpr>
{
  std::vector<UniqueExpr> arguments;
  auto                    next = peek_type_();
  if (next != TokenType::RightParen) {
    while (true) {
      arguments.push_back(parse_expression_());
      if (!match_<TokenType::Comma>()) {
        break;
      }
      consume_<TokenType::Comma>("Expected comma");
    }
  }
  return arguments;
}

template <bool OnlyCalls>
auto Parser::parse_call_() -> UniqueExpr
{
  auto       identifier = parse_identifier_();
  UniqueExpr expr       = nullptr;
  while (true) {
    if (match_<TokenType::LeftParen>()) {
      auto args = parse_bracketed_<TokenType::LeftParen, TokenType::RightParen>(
        [&] { return parse_call_arguments_(); });
      expr = std::make_unique<CallExpr>(identifier.token, std::move(args));
    } else if (match_<TokenType::Dot>()) {
      consume_<TokenType::Dot>("Expected '.'");
      auto inner = parse_call_();
      expr = std::make_unique<GetExpr>(identifier.token, std::move(inner));
    } else if (!OnlyCalls && match_<TokenType::LeftBracket>()) {
      auto inner =
        parse_bracketed_<TokenType::LeftBracket, TokenType::RightBracket>(
          [&]() { return parse_expression_(); });
      expr =
        std::make_unique<GetElementExpr>(identifier.token, std::move(inner));
    } else {
      expr = std::make_unique<Variable>(identifier.token);
      break;
    }
  }
  return expr;
}

auto Parser::parse_bracketed_expression_() -> UniqueExpr
{
  auto expr = parse_bracketed_<TokenType::LeftParen, TokenType::RightParen>(
    [&]() { return parse_expression_(); });
  return expr;
}

auto Parser::parse_terminal_() -> UniqueExpr
{
  switch (peek_type_()) {
    case TokenType::Number:
      return std::make_unique<Number>(token_());
    case TokenType::Char:
      return std::make_unique<Char>(token_());
    case TokenType::True:
      return std::make_unique<Boolean>(token_());
    case TokenType::False:
      return std::make_unique<Boolean>(token_());
    case TokenType::Null:
      return std::make_unique<Null>(token_());
    case TokenType::This:
      return std::make_unique<This>(token_());
    case TokenType::LeftParen:
      return parse_bracketed_expression_();
    case TokenType::Identifier:
      {
        return parse_call_<false>();
      }
    default:
      throw std::runtime_error("Unreachable. TODO: Handle Error");
  }
}

auto Parser::parse_expression_() -> UniqueExpr
{
  return parse_or_expr_();
}

auto Parser::parse() -> std::vector<ClassDecl>
{
  std::vector<ClassDecl> decls;
  while (!match_<TokenType::Eol>()) {
    auto decl = parse_class_decl_();
    decls.push_back(std::move(decl));
  }
  return decls;
}

auto jack::do_parse(std::string_view file_path) -> std::vector<ClassDecl>
{
  auto   input = hack::FileReader(file_path);
  Parser parser(input);
  return parser.parse();
}
