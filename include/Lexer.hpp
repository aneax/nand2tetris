#ifndef NAND2TETRIS_INCLUDE_TOKEN_HPP
#define NAND2TETRIS_INCLUDE_TOKEN_HPP
#include "FwdDecl.hpp"
#include "Reader.hpp"
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

class Token
{
  public:
    constexpr Token(TokenType token_type, Iterator begin, Iterator end)
        : type_(token_type)
        , begin_(begin)
        , end_(end)
    {
    }

    [[nodiscard]] auto type() const -> TokenType { return type_; }

    [[nodiscard]] auto string_view() const -> std::string_view
    {
      return std::string_view{begin_.data, end_.data};
    }

    [[nodiscard]] auto string() const noexcept -> std::string { return {begin_.data, end_.data}; }

    [[nodiscard]] auto begin() const noexcept -> Iterator { return begin_; }
    [[nodiscard]] auto end() const noexcept -> Iterator { return end_; }
    [[nodiscard]] auto line() const noexcept -> size_t { return begin_.line; }

    void set_type(TokenType type) { type_ = type; }

  private:
    TokenType type_;
    Iterator  begin_;
    Iterator  end_;
};

auto operator<<(std::ostream& out, const TokenType& token) -> std::ostream&;
auto operator<<(std::ostream& out, const Token& token) -> std::ostream&;
auto to_string(const Token& token) -> std::string;

auto get_tokens(FileReader& reader) -> TokenVector;
auto is_equal(std::string_view lhs, std::string_view rhs) -> bool;
namespace constants
{
static constexpr std::string_view EmptyLiteral = "Empty";
static constexpr Token EmptyToken(TokenType::NewLine, Iterator(EmptyLiteral.data(), 1, 1),
                                  Iterator(EmptyLiteral.data() + EmptyLiteral.size(), 1, 3));
}   //namespace constants
}   //namespace hack
#endif