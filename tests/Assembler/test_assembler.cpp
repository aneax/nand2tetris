#include "../runner.hpp"
#include <Instructions.hpp>
#include <Characters.hpp>
#include <Ast.hpp>
#include <Reader.hpp>
#include <Lexer.hpp>
#include <Parser.hpp>
#include <Visitors.hpp>
#include <filesystem>
#include <sstream>
#include <string_view>
#include <boost/ut.hpp>
#include <fstream>
#include <fmt/core.h>
static void run_assemble(std::string_view src, std::string_view sol_src)
{
  using namespace hack;
  using namespace boost::ut;
  FileReader reader(src);

  const auto& tokens = get_tokens(reader);

  auto asm_data = parse(tokens);
  populate_predefined_symbols(&asm_data);
  auto result = generate_binary(asm_data);
  auto actual = to_string(result);

  //fmt::print("{}\n", actual);
  // std::string oname        = std::string(src);
  // oname.append("_hack");
  // std::ofstream out(oname);
  // out << actual;
  // out.flush();

  FileReader sol_reader(sol_src);
  auto       required = std::string_view(sol_reader.begin(), sol_reader.end());
  expect((is_equal(required, actual)) >> fatal) << "Rewritten source must be equal to the source ";
}

static void test_parser()
{
  namespace fs = std::filesystem;
  using namespace boost::ut;
  fmt::print("Root Path: {}\n", ASSEMBLER_TEST_INPUT_DIR);

  std::string actual = "";
  fs::path    source = ASSEMBLER_TEST_INPUT_DIR;
  source /= actual;

  fs::path result = ASSEMBLER_TEST_INPUT_DIR;
  result /= "../assembler";
  result /= actual;

  if (fs::exists(source)) {
    run(
      [&](const fs::path& src) {
        // fs::path sol_src = result;
        fs::path sol_src = fs::relative(result, src.parent_path());
        fs::path src_sol = fs::relative(src.parent_path(), result);
        sol_src = fs::weakly_canonical(src.parent_path() / sol_src / src_sol.stem());
        if (!fs::is_directory(src) && result.has_extension()) {
          sol_src.replace_extension(".hack");
        } else {
          sol_src /= src.filename();
          sol_src.replace_extension(".hack");
        }
        sol_src = fs::weakly_canonical(sol_src);
        run_assemble(src.string(), sol_src.string());
      },
      source);
  } else {
    expect((false) >> fatal) << "Directory not found";
  }
}

//NOLINTNEXTLINE
int main()
{
  test_parser();
}