#ifndef NAND2TETRIS_INCLUDE_SIMULATOR_HARDWARE_MEMORY_HPP
#define NAND2TETRIS_INCLUDE_SIMULATOR_HARDWARE_MEMORY_HPP

#include "Bit.hpp"
#include "Gates.hpp"
#include <stdexcept>
#include <type_traits>

namespace hack::defs
{

static constexpr size_t RAM8AddressSize   = 3;
static constexpr size_t RAM64AddressSize  = 6;
static constexpr size_t RAM512AddressSize = 9;
static constexpr size_t RAM4KAddressSize  = 12;
static constexpr size_t RAM16KAddressSize = 14;
static constexpr size_t MemoryAddressSize = 15;
static constexpr size_t ScreenAddressSize = 13;
//Combinational chips respond to different combinations of their inputs without
//delay, except for the time it takes their inner chip-parts to complete the
//computation.

//Sequential chips' output depents not only on the inputs in the current time
//but also on the inputs and outputs that have been processed previously.

//How to model the progression of time? ->  Clock, that generates an ongoing
//train of signals, tick and tock. The time between the beginning of a tick and
//the end of the subsequent tock is called a cycle.

/**
 * @brief One Time Latch
 *
 * @param state feedback-loop state bit
 * @param in input bit
 * @return Bit output bit
 */
inline auto one_time_latch(Bit& state, const Bit& in) -> Bit
{
  state = or_gate(in, state);
  return state;
}

/**
 * @brief Set-reset flip flop (Not gated)
 * if set == 0 and reset == 0, out == state
 * if reset == 1 and set == 0, out == 0
 * if reset == 0 and set == 1, out == 1
 * if reset == 1 and set == 1 -> undefined(metastability)
 * @param state internal state
 * @param reset reset bit
 * @param set   set bit
 * @return Bit output bit
 */
inline auto set_reset_flip_flop(Bit& state, Bit reset, Bit set) -> Bit
{
  auto xv = nor_gate(state, set);
  state   = nor_gate(reset, xv);
  return state;
}

/**
 * @brief Data Flip-flops
 * @details An elementary time-dependent logic gate that can flip and flop
 * between two stable states: representing 0 and representing 1. It is
 * fundamental block from which all memory devices are constructed.
 *
 */
//struct DFF {
//    Bit state;
//};

using DFF             = Bit;
using BitRegister     = DFF;
using Register        = std::array<BitRegister, WordSize>;
using SpanRegister    = std::span<const BitRegister, WordSize>;
using MutSpanRegister = std::span<BitRegister, WordSize>;

template <typename Impl, size_t NumberOfImplRegisters,
          size_t RAMAddressingSize>   //NOLINT
struct RAMx {
    using element_type                  = Impl;
    static constexpr size_t Size        = NumberOfImplRegisters;
    static constexpr size_t AddressSize = RAMAddressingSize;

    static constexpr auto total() -> size_t
    {
      if constexpr (std::is_same_v<Impl, Register>) {
        return NumberOfImplRegisters;
      } else {
        return Impl::total() * NumberOfImplRegisters;
      }
    }

    using RAM = std::array<Impl, NumberOfImplRegisters>;
    RAM data;
};

using RAM8   = RAMx<Register, 8, RAM8AddressSize>;   //NOLINT
using RAM64  = RAMx<RAM8, 8, RAM64AddressSize>;      //NOLINT
using RAM512 = RAMx<RAM64, 8, RAM512AddressSize>;    //NOLINT
using RAM4K  = RAMx<RAM512, 8, RAM4KAddressSize>;    //NOLINT
using RAM8K  = RAMx<RAM4K, 2, ScreenAddressSize>;
using RAM16K = RAMx<RAM4K, 4, RAM16KAddressSize>;
using ROM32K = RAMx<RAM4K, 8, MemoryAddressSize>;   //NOLINT

//Data Flip-flop
auto dff(DFF& dff, const Bit& in) -> Bit;

/**
 * @brief 1-bit register
 * @details if (load(t)) then out(t+1) = in(t)
            else              out(t+1) = out(t)
 * @param state
 * @param in
 * @param load
 * @return Bit
 */
auto bit_register(BitRegister& bit_register, const Bit& in, const Bit& load)
  -> Bit;

template <size_t Size>
auto register_n(Register& reg, bit_span<Size> in, const Bit& load)
  -> bit_array<Size>
{
  //if (load == BitTrue) {
  //  state = in;
  //}
  bit_array<Size> out;
  for (size_t it = 0; it < Size; ++it) {
    out[it] = bit_register(reg[it], in[it], load);
  }
  return out;
}

auto ram8(RAM8& ram, SpanWord in, Bit load, bit_span<RAM8AddressSize> address)
  -> bit_array<WordSize>;

auto ram64(RAM64& ram, SpanWord in, Bit load,
           bit_span<RAM64AddressSize> address) -> bit_array<WordSize>;

auto ram512(RAM512& ram, SpanWord in, Bit load,
            bit_span<RAM512AddressSize> address) -> bit_array<WordSize>;

auto ram4k(RAM4K& ram, SpanWord in, Bit load,
           bit_span<RAM4KAddressSize> address) -> bit_array<WordSize>;

auto ram16k(RAM16K& ram, SpanWord in, Bit load,
            bit_span<RAM16KAddressSize> address) -> bit_array<WordSize>;

/**
 * @brief Instruction Memory (ROM32K)
 *
 * Emits the 16-bit value stored in the address selected by the \param address
 * input.
 * @param rom rom state
 * @param address address to be loaded
 * @return Register16 value in the loaded address (represents CPU instruction
 * input)
 */
auto rom32k(ROM32K& rom, SpanWord in, bit_span<MemoryAddressSize> address,
            Bit load) -> Word;

}   //namespace hack::defs

#endif
