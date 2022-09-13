#include "Characters.hpp"
#include "Reader.hpp"
#include <sstream>
#include <string_view>
#include <tuple>

namespace hack
{

auto is_white_space(char ch) -> bool
{
  return (' ' == ch) || ('\n' == ch) || ('\r' == ch) || ('\t' == ch) || ('\b' == ch) || ('\v' == ch)
      || ('\f' == ch);
}

auto is_alpha(char ch) -> bool
{
  return (ch == '_') || (ch == '.') || (ch == '$') || ((ch >= 'a') && (ch <= 'z'))
      || ((ch >= 'A') && (ch <= 'Z'));
}

auto is_digit(char ch) -> bool
{
  return (ch >= '0') && (ch <= '9');
}

auto is_operator(char ch) -> bool
{
  return ('+' == ch) || ('-' == ch) || ('=' == ch) || ('!' == ch) || ('(' == ch) || (')' == ch)
      || ('@' == ch) || ('&' == ch) || ('|' == ch);
}

auto is_alpha_numeric(char ch) -> bool
{
  return is_digit(ch) || is_alpha(ch);
}

auto is_equal_detailed(const MemoryInput& lhs, const MemoryInput& rhs)
  -> std::tuple<bool, std::string, std::string>
{
  auto consume_whitespace = [](std::string_view::const_iterator current) {
    while (is_white_space(*current) || *current == '/') {
      if (*current == '/') {
        while (*current != '\n') {
          ++current;
        }
      }
      ++current;
    }
    return current;
  };

  auto get_current_string = [](const MemoryInput& reader) -> std::string {
    std::stringstream ss;
    ss << reader.source() << ":" << reader.line() << ":" << reader.column() << "\n";
    return ss.str();
  };

  const auto* lcurrent = lhs.begin();
  const auto* rcurrent = rhs.begin();
  const auto* lend     = lhs.end();
  const auto* rend     = rhs.end();

  auto make_return = [&](bool is_equal) {
    return std::make_tuple(is_equal, get_current_string(lhs), get_current_string(rhs));
  };

  for (;; ++lcurrent, ++rcurrent) {
    lcurrent = consume_whitespace(lcurrent);
    rcurrent = consume_whitespace(rcurrent);

    if (lcurrent == lend && rcurrent == rend) {
      return make_return(true);
    }

    if (*lcurrent != *rcurrent) {
      return make_return(false);
    }
  }
}

auto is_equal(std::string_view lhs, std::string_view rhs) -> bool
{
  auto res = is_equal_detailed(lhs, rhs);
  return std::get<0>(res);
}
}   //namespace hack