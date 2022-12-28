
#include "HardwareSimulator/Computer.hpp"
#include "HardwareSimulator/Bit.hpp"
#include "HardwareSimulator/Gates.hpp"
#include "HardwareSimulator/Memory.hpp"
#include <stdexcept>

namespace hack::defs
{

auto program_counter(ProgramCounter& pc, SpanWord in, Bit load, Bit increment,
                     Bit reset) -> Word
{
  //increment
  auto incrd_value = inc<WordSize>(std::span{pc});
  auto inc_out     = mux<WordSize>({pc}, {incrd_value}, increment);

  //load
  auto load_out = mux<WordSize>(inc_out, in, load);

  //reset
  auto reset_out = mux<WordSize>(load_out, make_bit_array<WordSize>(), reset);

  pc = reset_out;
  return pc;
}

auto screen(Screen& screen, SpanWord input, Bit load,
            bit_span<ScreenAddressSize> address) -> Word
{
  //throw std::runtime_error("Unimplemented function: screen");
  return FalseWord;
}

auto keyboard(Keyboard& bit, Bit load) -> Word
{
  //throw std::runtime_error("Unimplemented function: screen");
  return FalseWord;
}

auto memory(Memory& mem, SpanWord in, Bit load,
            bit_span<MemoryAddressSize> address) -> Word
{
  //find out which memory sector is to be accessed
  auto [ram_a, ram_b, is_screen, is_keybd] =
    dmux4way(load, std::span{address}.last<2>());

  //is it in RAM?
  auto in_ram = or_gate(ram_a, ram_b);

  //ram output
  auto ram_out =
    ram16k(mem.ram, in, in_ram, address.first<RAM16KAddressSize>());

  //screen out
  auto screen_out =
    screen(mem.screen, in, is_screen, address.first<ScreenAddressSize>());

  //keyboard out
  auto key_out = keyboard(mem.keyboard, is_keybd);

  //
  auto out = mux4way(
    {
      std::array<Word, 4>{ram_out, ram_out, screen_out, key_out}
  },
    address.last<2>());

  return out;
}

auto cpu(CPU& cpu_state, bit_span<WordSize> memory_input,
         bit_span<WordSize> instruction, Bit reset) -> void
{
  /*If the instruction is A-instruction, it should be loaded to A-register.
  A-instruction has MSB of 0. MSB is the last entry in the instruction.
  If the instruction is C-instruction, it is interpreted as control instruction
 (1xxaccccccdddjjj) 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 1  x  x a c1
 c2 c3 c4 c5 c6 d1 d2 d3 j1 j2 j3
  * 'a' determines whether the ALU input is fed from A-register value or from
 incoming M value (input).
  * 'cccccc' define ALU operations
  * 'ddd' define destination to store computed value
  * 'jjj' define next jump destination
  */
  //Most significant bit denoting type of instruction.
  auto is_c_inst = instruction[InstMSBLocation];
  auto is_a_inst = not_gate(is_c_inst);

  //if instruction[5] is 1, destination is also A-register.
  auto is_dest_a = instruction[CInstLoadAddressLocation];
  //so, instruction is loaded if it is A-instruction or has destination as
  //A-register in previous instruction.
  auto save_a = or_gate(is_a_inst, is_dest_a);

  //D-register contains result of previous instruction.
  //Bit representing store to D-register
  auto is_dest_d = instruction[CInstLoadDataLocation];
  //If the instruction is C-instruction and dest_d_register is set, then
  //D-register must be loaded.
  auto load_d = and_gate(is_c_inst, is_dest_d);

  //Either value of A-register or the value from the memory is selected to pass
  //to the ALU
  auto a_field = instruction[CInstAFieldLocation];
  //Load from memory? Must be C-instruction and 'a' should be set.
  auto load_m = and_gate(is_c_inst, a_field);

  //alu input is either value in A-register or Memory Input
  auto mux_alu = mux<WordSize>(cpu_state.reg_a, memory_input, load_m);

  //alu operations
  auto [alu_o, is_zero, is_neg] =
    alu(cpu_state.reg_d, mux_alu, instruction[CInstZXLocation],
        instruction[CInstNXLocation], instruction[CInstZYLocation],
        instruction[CInstNYLocation], instruction[CInstFFLocation],
        instruction[CInstNOLocation]);
  cpu_state.alu_out = alu_o;

  //bit_array<MemoryAddressSize> memory_address;
  //std::copy(cpu_state.reg_a.begin(),
  //         cpu_state.reg_a.begin() + MemoryAddressSize,
  //         memory_address.begin());
  //TICK PART
  //Instruction bits are interpreted as a binary value that should be loaded in
  //A-register. If instruction is not A-instruction, load alu_out (A-register
  //acts as data-register), else load 'instruction' (A-register is address
  //register).
  auto mux_input =
    mux<WordSize>(std::span{cpu_state.alu_out}, instruction, is_a_inst);

  //address stored in A-register
  auto reg_a = register_n<WordSize>(cpu_state.reg_a, {mux_input}, save_a);
  //Load D-register
  auto reg_d =
    register_n<WordSize>(cpu_state.reg_d, {cpu_state.alu_out}, load_d);

  cpu_state.address = cpu_state.reg_a;
  //should write to M.
  auto dest_m       = instruction[CInstLoadMemoryLocation];
  cpu_state.write_m = and_gate(is_c_inst, dest_m);

  //is non-zero
  auto is_nzero = not_gate(is_zero);

  //is positive
  auto is_pos = not_gate(is_neg);

  //is positive non-zero
  auto is_pos_nz = and_gate(is_pos, is_nzero);

  auto jmp = [is_c_inst](Bit jmp_bit, Bit condition_bit) -> Bit {
    auto has_jmp    = and_gate(is_c_inst, jmp_bit);
    auto should_jmp = and_gate(condition_bit, has_jmp);
    return should_jmp;
  };

  //JGT? if cmp > 0 (i.e. positive non-zero)
  auto is_gt = jmp(instruction[CInstJGTLocation], is_pos_nz);
  //JEQ? if cmp == 0
  auto is_eq = jmp(instruction[CInstJEQLocation], is_zero);
  //JGT? if cmp < 0
  auto is_lt = jmp(instruction[CInstJLTLocation], is_neg);

  //Is any of the jump condition is satisfied
  auto must_jmp = or_gate(is_gt, is_eq, is_lt);

  //update program counter.
  //if jmp is true, counter must jump to the address in A-register
  //increment can always be set to true as load and reset will overwrite it and
  //is also default. resest is given as external input
  [[maybe_unused]] auto pc =
    program_counter(cpu_state.pc, cpu_state.reg_a, must_jmp, BitTrue, reset);
}

/**
 * @brief Hack Computer
 * If reset is 0, the program stored in the computer's ROM executes.
 * If reset is 1, the execution of the program restarts.
 *
 * @param reset Reset Bit
 */
auto computer(Computer& comp_state, Bit reset) -> void
{
  auto instruction = rom32k(
    comp_state.rom, FalseWord,
    std::span{comp_state.cpu.pc}.template first<MemoryAddressSize>(), BitFalse);
  cpu(comp_state.cpu, comp_state.cpu.alu_out, instruction, reset);
  comp_state.cpu.alu_out =
    memory(comp_state.memory, {comp_state.cpu.alu_out}, comp_state.cpu.write_m,
           std::span{comp_state.cpu.address}.first<MemoryAddressSize>());
}

}   //namespace hack::defs
