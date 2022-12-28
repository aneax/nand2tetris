#ifndef NAND2TETRIS_INCLUDE_COMMON_TOKEN_BASE_HPP
#define NAND2TETRIS_INCLUDE_COMMON_TOKEN_BASE_HPP

#include "Iterator.hpp"
#include <type_traits>
#include <utility>

namespace hack
{

struct DefaultValueHolder {
  public:
    constexpr DefaultValueHolder(Iterator begin, Iterator end) : mbegin(begin), mend(end) {}
    [[nodiscard]] auto begin() const noexcept -> Iterator { return mbegin; }
    [[nodiscard]] auto end() const noexcept -> Iterator { return mend; }
    [[nodiscard]] auto line() const noexcept -> size_t { return mbegin.line; }
    [[nodiscard]] auto string_view() const noexcept -> std::string_view
    {
      return {mbegin.data, mend.data};
    }
    [[nodiscard]] auto string() const noexcept -> std::string { return {mbegin.data, mend.data}; }

    Iterator mbegin;
    Iterator mend;
};

template <typename TokenType, typename ValueHolder = DefaultValueHolder>
requires std::is_enum_v<TokenType>
class TokenBase : public ValueHolder
{
  public:
    template <typename... Args>
    constexpr TokenBase(TokenType token_type, Args&&... data)
        : ValueHolder(std::forward<Args>(data)...)
        , type_(token_type)

    {
    }

    [[nodiscard]] auto type() const -> TokenType { return type_; }

    void update_type(TokenType type) { type_ = type; }

  private:
    TokenType type_;
};
using Location = hack::DefaultValueHolder;
}   //namespace hack

#endif