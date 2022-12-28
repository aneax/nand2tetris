
#include "../runner.hpp"
#include "Characters.hpp"
#include "Reader.hpp"
#include <VMParser.hpp>
#include <ASMGenerator.hpp>
#include <boost/ut.hpp>
// #include <fmt/format.h>

using namespace boost::ut;
using namespace hack::vasm;
static void run_local(const fs::path& vm_src, const fs::path& required_src)
{
  VMData data = parse_bytecode(vm_src);
  generate_asm(data);

  hack::FileReader actual(data.dest);
  hack::FileReader required(required_src);

  auto res = hack::is_equal_detailed(actual, required);
  if (std::get<0>(res)) {
    expect(true);
  } else {
    expect((false) >> fatal) << "Result deviated at: \n"
                             << std::get<1>(res) << std::get<2>(res) << "\n";
  }
}

static void run_test()
{
  namespace fs = std::filesystem;
  using namespace boost::ut;
  // fmt::print("Root Path: {}\n", TEST_BYTECODE_SRC_DIR);

  std::string actual{""};   //NOLINT
  fs::path    source = TEST_BYTECODE_SRC_DIR;
  source /= "src/";
  source /= actual;

  fs::path result = TEST_BYTECODE_SRC_DIR;
  result /= "output/";
  result /= actual;

  if (fs::exists(source)) {
    run<false>(
      [&](const fs::path& src) {
        if (src.extension() == ".vm") {
          // fmt::print("Source: {}\n", src.string());
          fs::path required_src = result / src.filename();
          required_src.replace_extension(".asm");
          run_local(src, required_src);
        }
      },
      source);
  } else {
    expect((false) >> fatal) << "Directory not found";
  }
}

//NOLINTNEXTLINE
int main()
{
  run_test();
}