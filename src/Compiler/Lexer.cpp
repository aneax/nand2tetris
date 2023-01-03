#include "JackCompiler/Lexer.hpp"
#include "Characters.hpp"
#include "Elements.hpp"
#include "JackCompiler/Token.hpp"
#include "Reader.hpp"
#include "tao/pegtl/file_input.hpp"
#include <stdexcept>
#include <tao/pegtl/contrib/analyze.hpp>
#include <tao/pegtl/contrib/trace.hpp>
#include "Grammar.hpp"
#include <iostream>
namespace jack
{

auto populate_keywords() -> std::unordered_map<std::string_view, TokenType>
{
  static std::unordered_map<std::string_view, TokenType> kwds = {
    {      "class",       TokenType::Class},
    {"constructor", TokenType::Constructor},
    {     "method",      TokenType::Method},
    {   "function",    TokenType::Function},
    {        "int",         TokenType::Int},
    {    "boolean",     TokenType::Boolean},
    {       "char",        TokenType::Char},
    {       "void",        TokenType::Void},
    {        "var",         TokenType::Var},
    {     "static",      TokenType::Static},
    {      "field",       TokenType::Field},
    {        "let",         TokenType::Let},
    {         "do",          TokenType::Do},
    {         "if",          TokenType::If},
    {       "else",        TokenType::Else},
    {      "while",       TokenType::While},
    {     "return",      TokenType::Return},
    {       "true",        TokenType::True},
    {      "false",       TokenType::False},
    {       "null",        TokenType::Null},
    {       "this",        TokenType::This},
  };
  return kwds;
}

Lexer::Lexer(MemoryInput& input)
    : data_(input)
    , start_(data_.current())
    , keywords_(populate_keywords())
{
}

auto Lexer::advance_(size_t n) -> bool
{
  return data_.bump(n);
}

auto Lexer::peek(size_t n) const noexcept -> char
{
  if (n > remaining()) {
    return '\0';
  }
  return current().data[n];
}

auto Lexer::current() const noexcept -> MemoryInput::iterator
{
  return data_.current();
}

auto Lexer::remaining() const noexcept -> size_t
{
  return data_.remaining();
}

[[nodiscard]] auto Lexer::is_end() const noexcept -> bool
{
  return data_.is_end();
}

auto Lexer::until_(char ch) -> void
{
  while ((peek() != ch) && advance_()) {}
}

auto Lexer::consume_whitespace_() -> void
{
  while (hack::is_white_space(peek())) {
    advance_();
  }
  start_ = current_();
}

auto Lexer::match_number_() -> TokenType
{
  while (hack::is_digit(peek())) {
    advance_();
  }
  return TokenType::Number;
}

auto Lexer::match_string_() -> TokenType
{
  while (hack::is_identifier(peek())) {
    advance_();
  }

  auto lexmee = std::string_view(start_.data, current().data);
  if (auto it = keywords_.find(lexmee); it != keywords_.end()) {
    return it->second;
  }

  return TokenType::Identifier;
}

auto Lexer::scan_() -> TokenType
{
  consume_whitespace_();
  char ch = peek();
  advance_();
  using T = TokenType;
  using namespace hack::elements;

  switch (ch) {
    case Plus:
      return T::Plus;
    case Minus:
      return T::Minus;
    case Asterisk:
      return T::Mul;
    case Slash:
      {
        char cur = peek();
        if (cur == Slash || cur == Asterisk) {
          do {
            if (cur == Slash) {
              //consume until eol
              until_('\n');
            } else if (cur == Asterisk) {
              //consume until "*,/"
              advance_();
              do {
                cur = peek();
              } while (advance_()
                       && !(peek() == Slash && cur == Asterisk));   //NOLINT
            }
            //consumes eol or Slash
          } while (advance_() && (peek() == Slash));
          return scan_();
        }
        return T::Div;
      }
    case Ampersand:
      return T::And;
    case Pipe:
      return T::Or;
    case AngleOpen:
      return T::LT;
    case AngleClose:
      return T::GT;
    case DoubleQuote:
      {
        until_(DoubleQuote);
        advance_();
        return T::Char;
      }
    case Equals:
      return T::EQ;
    case Tilde:
      return T::Not;
    case Dot:
      return T::Dot;
    case Comma:
      return T::Comma;
    case ParenOpen:
      return T::LeftParen;
    case ParenClose:
      return T::RightParen;
    case BraceOpen:
      return T::LeftBrace;
    case BraceClose:
      return T::RightBrace;
    case BracketOpen:
      return T::LeftBracket;
    case BracketClose:
      return T::RightBracket;
    case Semicolon:
      return T::Semicolon;
    default:
      if (hack::is_digit(ch)) {
        return match_number_();
      } else if (hack::is_alpha(ch)) {
        return match_string_();
      } else if (is_end()) {
        return T::Eol;
      } else {
        std::stringstream ss;
        ss << "Unexpected character: " << *(current_().data)
           << " at:" << current_().line << ":" << current_().column << "\n";
        throw std::runtime_error(ss.str());
      }
  }
  return TokenType::Null;
}

auto Lexer::current_() const noexcept -> MemoryInput::iterator
{
  return data_.current();
}

auto Lexer::next() -> Token
{
  start_ = current_();
  return {scan_(), start_, current_()};
}

auto Lexer::lex() -> std::vector<Token>
{
  std::vector<Token> tokens;
  while (!is_end() || peek() != '\0') {
    tokens.push_back(next());
  }
  return tokens;
}

auto do_lexing_v2(std::string_view file_path) -> void
{
  auto  input = hack::FileReader(file_path);
  Lexer lexer(input);
  auto  apply = [](auto& lexer, const Token& token) {
    std::cout << token << "\n";
  };

  lexer.act_only(apply);
}

auto do_lexing(std::string_view file_path) -> void
{
  using namespace hack::grammar;
  auto inp = file_input<>(file_path);

  std::stringstream ss;
  auto              apply = [&ss](std::string_view type, const auto& input) {
    //ss << "<" << type << ">" << input.string() << "<" << type << ">\n";
    std::cout << "<" << type << ">" << input.string() << "<" << type << ">\n";
  };

  try {
    //if (analyze<hack::grammar_v2::Statements>() != 0) {
    //  std::cerr << "Cycles without progress detected!\n";
    //} else {
    //  std::cerr << "Cycles not found!\n";
    //}

    //standard_trace<Tokens>(inp);
    ///Parse
    inp.restart();
    //parse<hack::grammar::Tokens, TokenAction>(inp, apply);
    parse<hack::grammar_v2::Statements>(inp);
    if (inp.current() != inp.end()) {
      std::stringstream err;
      err << "Parser didn't reach eol/f. Parsing have failed inp "
          << inp.position() << ":" << inp.current() << "\n";
      throw std::runtime_error(err.str());
    }
  } catch (const parse_error& e) {
    const auto  pos  = e.positions().front();
    std::string what = "parse error: ";
    what.append(e.what());
    what.append(" at ");
    what.append(inp.line_at(pos));
    what.append(" ");

    throw std::runtime_error(what);
  }
}
}   //namespace jack
