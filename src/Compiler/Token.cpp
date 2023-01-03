#include "JackCompiler/Token.hpp"

namespace jack
{
//NOLINTNEXTLINE
std::ostream& operator<<(std::ostream& out, const Token& token)
{
  auto begin = token.begin();
  auto end   = token.end();
  out << token.type() << "<" << begin.line << ":" << begin.column << ", "
      << end.line << ":" << end.column << "> " << token.string_view();
  return out;
}

//NOLINTNEXTLINE
std::ostream& operator<<(std::ostream& out, TokenType type)
{
#define TOK(ID)       \
  case TokenType::ID: \
    {                 \
      out << #ID;     \
      break;          \
    }
  switch (type) {
    TOK(Class);
    TOK(Constructor);
    TOK(Function);
    TOK(Method);
    TOK(Field);
    TOK(Static);
    TOK(Var);
    TOK(True);
    TOK(False);
    TOK(Null);
    TOK(This);
    TOK(Let);
    TOK(Do);
    TOK(If);
    TOK(Else);
    TOK(While);
    TOK(Return);
    TOK(Plus);
    TOK(Minus);
    TOK(Mul);
    TOK(Div);
    TOK(And);
    TOK(Or);
    TOK(LT);
    TOK(GT);
    TOK(EQ);
    TOK(Not);
    TOK(Dot);
    TOK(Comma);
    TOK(Int);
    TOK(Char);
    TOK(Boolean);
    TOK(Void);
    TOK(LeftParen);
    TOK(RightParen);
    TOK(LeftBrace);
    TOK(RightBrace);
    TOK(LeftBracket);
    TOK(RightBracket);
    TOK(Semicolon);
    TOK(Identifier);
    TOK(Eol);
    TOK(Number);
  }

  return out;
}
}   //namespace jack
