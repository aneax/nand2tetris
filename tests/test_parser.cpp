
#include "fmt/core.h"
#include "runner.hpp"
#include <Ast.hpp>
#include <Reader.hpp>
#include <Lexer.hpp>
#include <Parser.hpp>
#include <Visitors.hpp>
#include <string_view>
#include <boost/ut.hpp>
#include <fstream>
static void run_parser(std::string_view id)
{
  using namespace hack;
  using namespace boost::ut;
  FileReader reader(id);

  const auto& tokens          = get_tokens(reader);
  const auto [stmts, symbols] = parse(tokens);

  auto actual = src_rewrite(stmts);
  //fmt::print("{}",actual);
  //std::ofstream out("pong.asm");
  //out << actual;
  //out.flush();
  auto required = std::string_view(reader.begin(), reader.end());
  expect((is_equal(required, actual)) >> fatal) << "Rewritten source must be equal to the source";
}

static void test_parser()
{
  namespace fs = std::filesystem;
  using namespace boost::ut;
  fmt::print("Root Path: {}\n", ASSEMBLER_TEST_INPUT_DIR);
  fs::path source = ASSEMBLER_TEST_INPUT_DIR;
  //source /= "pong/";
  if (fs::exists(source)) {
    run(source.string(), [](std::string_view data) { run_parser(data); });
  } else {
    expect((false) >> fatal) << "Directory not found";
  }
}

//NOLINTNEXTLINE
int main()
{
  test_parser();
}