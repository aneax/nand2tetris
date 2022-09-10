#include "Iterator.hpp"
#include <sstream>
namespace hack
{
auto operator<<(std::ostream& out, Iterator iterator) -> std::ostream&
{
  out << iterator.line << ":" << iterator.column << "";
  return out;
}

auto to_string(Iterator iterator) -> std::string
{
  std::stringstream ss;
  ss << iterator;
  return ss.str();
}
}   //namespace hack