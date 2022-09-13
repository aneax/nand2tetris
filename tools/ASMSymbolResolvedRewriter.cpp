#include "Errors.hpp"
#include "Visitors.hpp"
#include "Ast.hpp"
#include "Reader.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Instructions.hpp"
#include "fmt/core.h"
#include <filesystem>
#include <fstream>

extern void asm_symbol_resolved_rewriter(const fs::path& src)
{
  using namespace hack;
  fs::path dest = fs::current_path();
  dest/=src.filename();
  dest.replace_filename(std::string("out_") + src.filename().string());
  FileReader  reader(src);
  const auto& tokens = get_tokens(reader);
  auto        data   = parse(tokens);

  populate_predefined_symbols(&data);
  auto          vec          = generate_binary(data);
  auto          resolved_src = symbol_resolved_rewrite(data);
  std::ofstream out(dest);
  out << resolved_src;
  out.flush();
}

auto main(int argc, const char** argv) -> int
{
  if (argc != 2) {
    throw hack::RuntimeError("Requires filename");
  }
  fs::path src(argv[1]);
  src= fs::canonical(src);
  fmt::print("{}/n", src.string());
  asm_symbol_resolved_rewriter(src);
  return 0;
}