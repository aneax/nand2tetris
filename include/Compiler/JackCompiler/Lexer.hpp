#ifndef NAND2TETRIS_INCLUDE_COMPILER_LEXER_HPP
#define NAND2TETRIS_INCLUDE_COMPILER_LEXER_HPP

#include "Token.hpp"
#include <Reader.hpp>
#include <unordered_map>
#include <vector>

namespace jack
{

using MemoryInput = hack::MemoryInput;
class Lexer
{
  public:
    Lexer(MemoryInput&);
    auto next() -> Token;
    auto lex() -> std::vector<Token>;

    [[nodiscard]] auto peek(size_t n = 0) const noexcept -> char;
    [[nodiscard]] auto current() const noexcept -> MemoryInput::iterator;
    [[nodiscard]] auto remaining() const noexcept -> size_t;
    [[nodiscard]] auto is_end() const noexcept -> bool;

    template <typename Apply>
    auto act_only(Apply apply) -> void
    {
      while (!is_end()) {
        apply(*this, next());
      }
    }

  private:
    [[nodiscard]] auto current_() const noexcept -> MemoryInput::iterator;

    auto consume_whitespace_() -> void;
    auto until_(char ch) -> void;
    auto advance_(size_t n = 1) -> bool;
    auto scan_() -> TokenType;
    auto match_string_() -> TokenType;
    auto match_number_() -> TokenType;

    MemoryInput                                           data_;
    Iterator                                              start_;
    const std::unordered_map<std::string_view, TokenType> keywords_;
};

auto do_lexing(std::string_view file_path) -> void;
auto do_lexing_v2(std::string_view file_path) -> void;
}   //namespace jack

#endif
