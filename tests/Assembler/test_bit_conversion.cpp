
#include <Instructions.hpp>
#include <Ast.hpp>
#include <boost/ut.hpp>
using namespace boost::ut;
using namespace hack;
static void tests()
{
  "a_instructions"_test = []() {
    Bit required("0000000000010001");
    Bit actual = get_a_inst(17);
    expect((actual == required) >> fatal) << "Must be equal";
  };

  "c_instructions"_test = [] {
    Bit required("1110111010100000");
    Bit actual = get_c_inst(false, 0b111010, 0b100, 0b000);
    expect((actual == required) >> fatal) << actual;
  };

  "c_instructions"_test = [] {
    Bit required("1110011111010110");
    Bit actual = get_c_inst(false, 0b011111, 0b010, 0b110);
    expect((actual == required) >> fatal) << actual;
  };
}

static void test_table()
{
  Data asm_data;
  populate_predefined_symbols(&asm_data);
  auto&  comp_table = asm_data.comp_table;
  auto&  dest_table = asm_data.dest_table;
  auto&  jmp_table  = asm_data.jump_table;
  bool   isa;
  size_t comp;
  size_t dest;
  size_t jump = hack::constants::VNULL;
  "D=M"_test  = [&]() {
    std::tie(isa, comp) = comp_table.at("M");
    dest                = dest_table.at("D");
    Bit bit             = get_c_inst(isa, comp, dest, jump);
    Bit required("1111110000010000");
    expect((bit == required) >> fatal);
  };
}

//NOLINTNEXTLINE
int main()
{
  tests();
  test_table();
}