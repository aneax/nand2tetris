#include "Errors.hpp"
#include "Lexer.hpp"
#include <sstream>
#include <fmt/core.h>
namespace hack
{
void ErrorReporter::dump_() const
{
  fmt::print("Encountered Errors!!!\n");
  for (const auto& error : errors_) {
    fmt::print("{}\n", error);
  }
}

void ErrorReporter::check_errors() const
{
  if (has_errors_()) {
    dump_();
    throw ParseError("Parsing errors encountered.");
  }
}

auto max_arg_exceeded_error(const Token& token, size_t length, size_t max) -> std::string
{
  std::stringstream ss;
  ss << "Max Number of Supported Argument Exceeded at: " << token.begin() << ". Supported: " << max
     << " Actual: " << length << "\n";
  return ss.str();
}

auto condition_empty_error(const Token& token) -> std::string
{
  std::stringstream ss;
  ss << "Condition Empty at: " << token.begin() << ". Required at least one condition.\n";
  return ss.str();
}

auto missing_argument_error(const Token& token) -> std::string
{
  std::stringstream ss;
  ss << "Missing argument. Expects an expression before " << token << "\n";
  return ss.str();
}
}   //namespace hack