#include "Errors.hpp"
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
}   //namespace hack