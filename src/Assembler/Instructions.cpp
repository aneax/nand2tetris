#include "Instructions.hpp"
#include "FwdDecl.hpp"
#include "Lexer.hpp"
#include "Ast.hpp"

namespace hack
{
auto get_a_inst(uint64_t value) -> Bit
{
  return {value};
}

auto get_c_inst(bool type_a, uint64_t cmd, uint64_t dest, uint64_t jmp) -> Bit
{
  using namespace constants;
  Bit bit(0);
  bit |= CInstBegin;   //first 3 bits
  bit <<= CInstASize;
  bit |= static_cast<uint64_t>(type_a);
  bit <<= CInstCompSize;
  bit |= cmd;
  bit <<= CInstDestSize;
  bit |= dest;
  bit <<= CInstJumpSize;
  bit |= jmp;
  return bit;
}

static auto prepare_jump_table() -> JumpTable
{
  using T = TokenType;
  using namespace constants;
  JumpTable table;

  table.insert({T::JGT, VJGT});
  table.insert({T::JEQ, VJEQ});
  table.insert({T::JLT, VJLT});
  table.insert({T::JGE, VJGT | VJEQ});
  table.insert({T::JLE, VJLT | VJEQ});
  table.insert({T::JNE, VJNE});
  table.insert({T::JMP, VJLT | VJEQ | VJGT});
  return table;
}

static auto prepare_dest_table() -> DestTable
{
  DestTable table;
  using namespace constants;
  table.insert({"M", DestM});
  table.insert({"A", DestA});
  table.insert({"D", DestD});
  table.insert({"DM", DestD | DestM});
  table.insert({"MD", DestD | DestM});
  table.insert({"AM", DestA | DestM});
  table.insert({"AD", DestA | DestD});
  table.insert({"ADM", DestA | DestD | DestM});
  return table;
}

static auto prepare_comp_table() -> CompTable
{
  CompTable table;
  auto      insert_comp = [&](std::string_view str, bool isa, size_t comp) {
    table.insert({
      str,
      {isa, comp}
    });
  };

  using namespace constants;
  //a = 0
  insert_comp("0", false, ROW0);
  insert_comp("1", false, ROW1);
  insert_comp("-1", false, ROW2);
  insert_comp("D", false, ROW3);
  insert_comp("A", false, ROW4);
  insert_comp("!D", false, ROW5);
  insert_comp("!A", false, ROW6);
  insert_comp("-D", false, ROW7);
  insert_comp("-A", false, ROW8);
  insert_comp("D+1", false, ROW9);
  insert_comp("A+1", false, ROW10);
  insert_comp("D-1", false, ROW11);
  insert_comp("A-1", false, ROW12);
  insert_comp("D+A", false, ROW13);
  insert_comp("D-A", false, ROW14);
  insert_comp("A-D", false, ROW15);
  insert_comp("D&A", false, ROW16);
  insert_comp("D|A", false, ROW17);
  //a = 1
  insert_comp("M", true, ROW4);
  insert_comp("!M", true, ROW6);
  insert_comp("M+1", true, ROW10);
  insert_comp("M-1", true, ROW12);
  insert_comp("D+M", true, ROW13);
  insert_comp("D-M", true, ROW14);
  insert_comp("M-D", true, ROW15);
  insert_comp("D&M", true, ROW16);
  insert_comp("D|M", true, ROW17);

  return table;
}

static void predefined_symbols(SymbolTable& table)
{
  using namespace constants;
  auto insert = [&table](const std::string& key, size_t value) { table.insert({key, value}); };
  insert("R0", R0);
  insert("R1", R1);
  insert("R2", R2);
  insert("R3", R3);
  insert("R4", R4);
  insert("R5", R5);
  insert("R6", R6);
  insert("R7", R7);
  insert("R8", R8);
  insert("R9", R9);
  insert("R10", R10);
  insert("R11", R11);
  insert("R12", R12);
  insert("R13", R13);
  insert("R14", R14);
  insert("R15", R15);

  insert("SCREEN", SCREEN);
  insert("KBD", KBD);

  insert("SP", SP);
  insert("LCL", LCL);
  insert("ARG", ARG);
  insert("THIS", THIS);
  insert("THAT", THAT);
}

void populate_predefined_symbols(Data* data)
{
  data->comp_table = prepare_comp_table();
  data->dest_table = prepare_dest_table();
  data->jump_table = prepare_jump_table();
  predefined_symbols(data->symbols);
}

auto to_string(const BitVector& vec) -> std::string
{
  std::stringstream ss;
  for (const auto& bit : vec) {
    ss << bit << "\n";
  }
  return ss.str();
}
}   //namespace hack