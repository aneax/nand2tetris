
#include "../runner.hpp"
#include <Instructions.hpp>
#include <Ast.hpp>
#include <Reader.hpp>
#include <Lexer.hpp>
#include <Parser.hpp>
#include <Visitors.hpp>
#include <string_view>
#include <boost/ut.hpp>
#include <fstream>
#include <fmt/core.h>

template <bool PostRun>
static void run_parser(std::string_view id)
{
  using namespace hack;
  using namespace boost::ut;
  FileReader reader(id);

  const auto& tokens = get_tokens(reader);
  auto        data   = parse(tokens);

  if constexpr (!PostRun) {
    auto actual = src_rewrite(data.stmts);
    //fmt::print("{}",actual);
    //std::ofstream out("pong.asm");
    //out << actual;
    //out.flush();
    auto required = std::string_view(reader.begin(), reader.end());
    expect((is_equal(required, actual)) >> fatal)
      << "Rewritten source must be equal to the source" << actual;
  }

  if constexpr (PostRun) {
    populate_predefined_symbols(&data);
    auto        vec          = generate_binary(data);
    auto        resolved_src = symbol_resolved_rewrite(data);
    std::string oname        = std::string(id);
    oname.append("_resolved");
    std::ofstream out(oname);
    out << resolved_src;
    out.flush();
  }
}

template <bool PostRun = false>
static void test_parser()
{
  namespace fs = std::filesystem;
  using namespace boost::ut;
  fmt::print("Root Path: {}\n", ASSEMBLER_TEST_INPUT_DIR);
  fs::path source = ASSEMBLER_TEST_INPUT_DIR;
  // source /= "rect/Rect.asm";
  if (fs::exists(source)) {
    run([](const fs::path& src) { run_parser<PostRun>(src.string()); }, source);
  } else {
    expect((false) >> fatal) << "Directory not found";
  }
}

#ifndef POST_ASSEMBLER_RUN
constexpr bool PostRun = false;
#else
constexpr bool PostRun = true;
#endif
//NOLINTNEXTLINE
int main()
{
  test_parser<PostRun>();
}
