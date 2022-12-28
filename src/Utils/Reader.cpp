#include "Reader.hpp"
#include <sstream>
#include <fstream>
namespace hack::internal
{
auto MemoryInput::bump(size_t count) noexcept -> bool
{
  bool ret = true;
  if (count > remaining()) {
    count = remaining();
    ret   = false;
  }

  for (size_t it = 0; it < count; ++it) {
    if (current_.data[it] == '\n') {
      ++current_.line;
      current_.column = 1;
    } else {
      ++current_.column;
    }
    current_.data += count;
  }
  return ret;
}

void MemoryInput::restart(size_t line, size_t column)
{
  assert(line != 0);
  assert(column != 0);

  current_.data   = begin_;
  current_.line   = line;
  current_.column = column;
}

auto read_file(std::string_view filename) -> std::string
{
  namespace fs = std::filesystem;
  fs::path source{filename};
  if (fs::exists(source)) {
    std::string buffer;
    {
      std::ifstream     file(source);
      std::stringstream buf;
      buf << file.rdbuf();
      buffer = buf.str();
    }
    return buffer;
  }

  std::string msg = "File not found: ";
  msg.append(filename);
  throw std::runtime_error(msg);
}
}   //namespace hack::internal

namespace hack
{
ReadInput::ReadInput(const fs::path& path)
    : StringInput(internal::read_file(path.string()), path.string())
{
}
}   //namespace hack