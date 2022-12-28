#ifndef NAND2TETRIS_INCLUDE_ASSEMBLER_TOKEN_HPP
#define NAND2TETRIS_INCLUDE_ASSEMBLER_TOKEN_HPP
#include "FwdDecl.hpp"
#include <Reader.hpp>
#include <TokenBase.hpp>
#include <string_view>
#include <vector>
#include <string>
namespace hack
{

enum class TokenType {
  Label,
  Symbol,
  Equal,
  Add,
  Sub,
  And,
  Or,
  Not,
  At,
  SemiColon,
  Identifier,
  Number,
  JGT,
  JLT,
  JGE,
  JLE,
  JEQ,
  JNE,
  JMP,
  M,    //M Register
  A,    //A Register
  D,    //D Register
  DM,   //D and M
  AM,
  AD,
  ADM,
  DNULL,
  LParantheses,
  RParantheses,
  NewLine,
  FEOF
};


class Token : public TokenBase<TokenType>
{
    using TokenBase<TokenType>::TokenBase;
};

auto operator<<(std::ostream& out, const TokenType& token) -> std::ostream&;
auto operator<<(std::ostream& out, const Token& token) -> std::ostream&;
auto to_string(const Token& token) -> std::string;

auto get_tokens(FileReader& reader) -> TokenVector;
auto max_arg_exceeded_error(const Token& token, size_t length, size_t max) -> std::string;
auto condition_empty_error(const Token& token) -> std::string;
auto missing_argument_error(const Token& token) -> std::string;
namespace constants
{
static constexpr std::string_view EmptyLiteral = "Empty";
static constexpr Token EmptyToken(TokenType::NewLine, Iterator(EmptyLiteral.data(), 1, 1),
                                  Iterator(EmptyLiteral.data() + EmptyLiteral.size(), 1, 3));
}   //namespace constants
}   //namespace hack
#endif