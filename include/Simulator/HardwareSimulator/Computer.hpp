#ifndef NAND2TETRIS_INCLUDE_SIMULATOR_HARDWARE_COMPUTER_HPP
#define NAND2TETRIS_INCLUDE_SIMULATOR_HARDWARE_COMPUTER_HPP

#include "Bit.hpp"
#include "Gates.hpp"
#include "Memory.hpp"
#include <algorithm>
namespace hack::defs
{
static constexpr size_t InstMSBLocation          = 15;
static constexpr size_t CInstAFieldLocation      = 12;
static constexpr size_t CInstZXLocation          = 11;
static constexpr size_t CInstNXLocation          = 10;
static constexpr size_t CInstZYLocation          = 9;
static constexpr size_t CInstNYLocation          = 8;
static constexpr size_t CInstFFLocation          = 7;
static constexpr size_t CInstNOLocation          = 6;
static constexpr size_t CInstLoadAddressLocation = 5;
static constexpr size_t CInstLoadDataLocation    = 4;
static constexpr size_t CInstLoadMemoryLocation  = 3;
static constexpr size_t CInstJLTLocation         = 2;
static constexpr size_t CInstJEQLocation         = 1;
static constexpr size_t CInstJGTLocation         = 0;

using Screen   = RAM8K;
using Keyboard = Word;

struct Memory {
    RAM16K   ram;
    Screen   screen;
    Keyboard keyboard;
};

using ProgramCounter = Word;

struct CPU {
    Register       reg_a;   ///A-register
    Register       reg_d;   ///D-register
    Word           alu_out;
    Word           address;
    ProgramCounter pc;   ///ProgramCounter
    Bit            write_m;
};

struct Computer {
    CPU    cpu;
    Memory memory;
    ROM32K rom;
};

auto screen(Screen& screen, SpanWord input, Bit load,
            bit_span<ScreenAddressSize> address) -> Word;

auto keyboard(const Keyboard& bit, Bit load) -> Word;

/**
 * @brief Program Counter
 *
 * t = current cycle (tick/tock)
 * if (reset[t]) out[t+1] = 0
 * else if (load[t]) out[t+1] = in
 * else if (inc[t]) out[t+1]=out[t]+1
 * else out[t+1] = out[t]
 *
 * @param in    input 16-bit
 * @param load  load bit
 * @param inc   increment bit
 * @param reset reset bit
 * @return bit_array<Word>
 */
auto program_counter(ProgramCounter& pc, SpanWord in, Bit load, Bit inc,
                     Bit reset) -> Word;

/**
 * @brief Hack Computer's Memory.
 * It represents the complete address space of the memory including RAM and
 memory-mapped I/O.
 * Facilitates read and write operations:
 *  * Read: out(t) = Memory[address(t)](t)
 *  * Write: if (load(t-1)) then Memory[address(t-1)](t) = in(t-1)
 * i.e. the chip always outputs the value stored at the memory
 * location specified by address. If load == 1, the in value is loaded
 * into the memory location specified by address. This value becomes
 * available through the out output from the next time step onward.

 * @param in
 * @param load
 * @param address
 * @return auto
 */
auto memory(Memory& mem, SpanWord in, Bit load,
            bit_span<MemoryAddressSize> address) -> Word;

/**
 * @brief Implementation of the Hack CPU.
 * It consists of the ALU, two registers A and D, and a program counter PC.
 *
 * @param[in] input input value (content of RAM[A])
 * @param[in] instruction instruction for execution.
 * @param[in] reset reset or continue.
 * @return Memory value output, should write, address in data memory, address of
 * next instruction
 */
auto cpu(CPU& cpu_state, bit_span<WordSize> memory_input,
         bit_span<WordSize> instruction, Bit reset) -> void;

/**
 * @brief Hack Computer
 * If reset is 0, the program stored in the computer's ROM executes.
 * If reset is 1, the execution of the program restarts.
 *
 * @param reset Reset Bit
 */
auto computer(Computer& comp_state, Bit reset) -> void;

}   //namespace hack::defs

#endif
