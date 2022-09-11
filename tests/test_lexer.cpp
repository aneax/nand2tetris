#include "runner.hpp"
#include <Reader.hpp>
#include <Lexer.hpp>
#include <string_view>
#include <boost/ut.hpp>

extern void run_lexer(std::string_view id)
{
  using namespace hack;
  using namespace boost::ut;
  //std::string id("var d = a+b- c;");
  //std::string id("var //a+b - c;");
  FileReader reader(id);

  const auto& tokens = get_tokens(reader);
  for (const auto& token : tokens) {
    std::cout << token << "\n";
  }
  std::cout << "\n\n";
}

extern void test_lexer()
{
  namespace fs = std::filesystem;
  using namespace boost::ut;
  fmt::print("Root Path: {}\n", ASSEMBLER_TEST_INPUT_DIR);
  fs::path source = ASSEMBLER_TEST_INPUT_DIR;
  source /= "add/";
  if (fs::exists(source)) {
    run([](const fs::path& src) { run_lexer(src.string()); }, source);
  } else {
    expect((false) >> fatal) << "Directory not found";
  }
}

//NOLINTNEXTLINE
int main()
{
  test_lexer();
}