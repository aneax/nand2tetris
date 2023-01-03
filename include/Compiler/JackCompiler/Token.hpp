#ifndef NAND2TETRIS_INCLUDE_COMPILER_TOKEN_HPP
#define NAND2TETRIS_INCLUDE_COMPILER_TOKEN_HPP

#include "TokenBase.hpp"
#include <Iterator.hpp>

namespace jack
{

using Iterator = hack::Iterator;

enum class TokenType {
  Class,
  Constructor,
  Function,
  Method,
  Field,
  Static,
  Var,
  True,
  False,
  Null,
  This,
  Let,
  Do,
  If,
  Else,
  While,
  Return,
  Identifier,
  Number,
  Plus,
  Minus,
  Mul,
  Div,
  And,
  Or,
  LT,
  GT,
  EQ,
  Not,
  Dot,
  Comma,
  Int,
  Char,
  Boolean,
  Void,
  LeftParen,
  RightParen,
  LeftBrace,
  RightBrace,
  LeftBracket,
  RightBracket,
  Semicolon,
  Eol
};

using Token = hack::TokenBase<TokenType>;

namespace constants
{
static constexpr std::string_view NullLiteral("NULL");
static constexpr Token
  NullToken(TokenType::Null, Iterator(NullLiteral.data(), 1, 1),
            Iterator(NullLiteral.data() + NullLiteral.size(), 1, 4));
}   //namespace constants
inline auto empty_token() -> Token
{
  return constants::NullToken;
}
std::ostream& operator<<(std::ostream& out, const Token& token);   //NOLINT
std::ostream& operator<<(std::ostream& out, TokenType type);       //NOLINT

template <TokenType... Types>
auto is_type(TokenType type) noexcept -> bool
{
  return ((Types == type) || ...);
}

}   //namespace jack

#endif
