#ifndef NAND2TETRIS_INCLUDE_SIMULATOR_HARDWARE_GATES_HPP
#define NAND2TETRIS_INCLUDE_SIMULATOR_HARDWARE_GATES_HPP

#include "Bit.hpp"

#include <bitset>
#include <array>
#include <span>
#include <tuple>
#include <type_traits>
#include <utility>

namespace hack::defs
{

namespace internal
{

/**
 * @brief Basic Single Bit Nand Gate
 *
 * @param pa
 * @param pb
 * @return Bit \f$\neg(p_a \wegde p_b)\f$
 */
constexpr auto nand_impl(Bit pa, Bit pb) -> Bit
{
  return ~(pa & pb);
}

template <size_t Size, typename Apply, size_t... ArgIndex, typename... Ts>
constexpr auto bit_apply(Apply apply, std::index_sequence<ArgIndex...> /**/, Ts&&... args)
  -> bit_array<Size>
{
  bit_array<Size> res;
  for (size_t it = 0; it < Size; ++it) {
    res[it] = apply(((args)[it])...);
  }
  return res;
}

template <size_t Size, typename Apply, typename... Ts>
constexpr auto bit_apply(Apply apply, Ts&&... args) -> bit_array<Size>
{
  return bit_apply<Size>(apply, std::make_index_sequence<sizeof...(args)>{},
                         std::forward<Ts>(args)...);
}

template <size_t Size>
constexpr auto nand_impl(bit_span<Size> pa, bit_span<Size> pb)
{
  return bit_apply<Size>([](auto in_1, auto in_2) { return nand_impl(in_1, in_2); }, pa, pb);
}

template <size_t Size>
constexpr auto nand_impl(const bit_array<Size>& pa, const bit_array<Size>& pb)
{
  return nand_impl<Size>(bit_span<Size>(pa), bit_span<Size>(pb));
}

template <typename T>
constexpr auto not_impl(const T& in) -> T
{
  return nand_impl(in, in);
}

template <typename T>
constexpr auto and_impl(const T& pa, const T& pb) -> T
{
  auto temp = nand_impl(pa, pb);
  return not_impl(temp);
}

template <typename T>
constexpr auto or_impl(const T& pa, const T& pb) -> T
{
  auto a_not = not_impl(pa);
  auto b_not = not_impl(pb);
  return nand_impl(a_not, b_not);
}

template <typename T>
constexpr auto xor_impl(const T& pa, const T& pb) -> T
{
  auto not_a    = not_impl(pa);
  auto not_b    = not_impl(pb);
  auto and_na_b = and_impl(not_a, pb);
  auto and_a_nb = and_impl(pa, not_b);
  return or_impl(and_a_nb, and_na_b);
}

}   //namespace internal

/**
 * @brief Nand Gate \f$\neg(pin_a \wedge pin_b)\f$
 *
 * @tparam T
 * @param pin_a Value at pin a
 * @param pin_b Value at pin b
 * @return T \f$\neg(pin_a \wedge pin_b)\f$
 */
template <BitOrBitArray T>
constexpr auto nand_gate(const T& pin_a, const T& pin_b) -> T
{
  return internal::nand_impl(pin_a, pin_b);
}

/**
 * @brief Not Gate \f$\neg pin_a\f$
 * @tparam T BitOrBitArray
 * @param pin_a Input Pin
 * @return T \f$\neg pin_a\f$
 */
template <BitOrBitArray T>
constexpr auto not_gate(const T& pin_a) -> T
{
  return internal::not_impl(pin_a);
}

/**
 * @brief AND Gate \f$\neg(pin_a \wedge pin_b)\f$
 *
 * @tparam T
 * @param pin_a Value at pin a
 * @param pin_b Value at pin b
 * @return T \f$\neg(pin_a \wedge pin_b)\f$
 */
template <BitOrBitArray T>
constexpr auto and_gate(const T& pin_a, const T& pin_b) -> T
{
  return internal::and_impl(pin_a, pin_b);
}

/**
 * @brief OR Gate \f$\neg(pin_a \vee pin_b)\f$
 *
 * @tparam T
 * @param pin_a Value at pin a
 * @param pin_b Value at pin b
 * @return T \f$\neg(pin_a \vee pin_b)\f$
 */
template <BitOrBitArray T>
constexpr auto or_gate(const T& pin_a, const T& pin_b) -> T
{
  return internal::or_impl(pin_a, pin_b);
}

template <BitOrBitArray T, typename... Ts>
constexpr auto or_gate(const T& pin_a, const T& pin_b, const Ts&... pins)
{
  return internal::or_impl(internal::or_impl(pin_a, pin_b), pins...);
}

template <BitOrBitArray T>
constexpr auto nor_gate(const T& pin_a, const T& pin_b) -> T
{
  auto or_v = or_gate(pin_a, pin_b);
  return not_gate(or_v);
}

/**
 * @brief XOR Gate \f$pin_a \oplus pin_b\f$
 *
 * @tparam T
 * @param pin_a Value at pin a
 * @param pin_b Value at pin b
 * @return T \f$pin_a \oplus pin_b\f$
 */
template <BitOrBitArray T>
constexpr auto xor_gate(const T& pin_a, const T& pin_b) -> T
{
  return internal::xor_impl(pin_a, pin_b);
}

/**
 * @brief Multiplexor
 * \f$(pin_a \wedge (\neg sel)) \vee (pin_b \wedge sel)\f$
 * equivalent to (!sel) ? pin_a : pin_b
 * @param pin_a Input 1
 * @param pin_b Input 2
 * @param sel   Selection Bit
 * @return Bit selected input bit.
 */
auto mux(const Bit& pin_a, const Bit& pin_b, const Bit& sel) -> Bit;

/**
 * @brief Demultiplexer
 *  (!sel) ? {in, 0} : {0,in}
 * @param in  input bit
 * @param sel selection bit
 * @return std::pair<Bit, Bit> (!sel) ? {in, 0} : {0, in}
 */
auto dmux(const Bit& in, const Bit& sel) -> std::pair<Bit, Bit>;

/**
 * @brief 8-way OR OR(in[0],in[1],...,in[7])
 * @param bits HalfWord length bit array
 * @return Bit \f$in[0] \vee in[1] \vee \cdots \vee in[7]\f$
 */
auto or8way(bit_span<HalfWordSize> bits) -> Bit;

/**
 * @brief Multiple Bit Multiplexer
 *
 * @tparam Size
 * @param pin_a
 * @param pin_b
 * @param sel
 * @return bit_array<Size>
 */
template <size_t Size>
auto mux(bit_span<Size> pin_a, bit_span<Size> pin_b, const Sel& sel) -> bit_array<Size>
{
  bit_array<Size> out;
  for (size_t it = 0; it < Size; ++it) {
    out[it] = mux(pin_a[it], pin_b[it], sel);
  }
  return out;
}

/**
 * @brief 4-Way 16-bit Multiplexer
 * out = a if sel == 00
 *       b if sel == 01
 *       c if sel == 10
 *       d if sel == 11
 * @param ins
 * @param sel
 * @return Register16
 */
auto mux4way(std::span<const Word, 4> ins, bit_span<2> sel) -> Word;

/**
 * @brief Implementation of 8-way 16-bit multiplexer
 * out = a if sel == 000
 *       b if sel == 001
 *       c if sel == 010
 *       d if sel == 011
 *       ....
 *       h if sel == 111
 * @param ins Array of 8 16-Bit Register
 * @param sel 3-Bit Selection Bit
 * @return
 */
auto mux8way(std::span<const Word, 8> ins, std::span<const Sel, 3> sel) -> Word; //NOLINT(readability-magic-numbers)

/**
 * @brief 4-way Demultiplexer
 * {a,b,c,d} = {in, 0, 0 ,0} if sel == 00
 *             {0, in, 0, 0} if sel == 01
 *             {0, 0, in, 0} if sel == 10
 *             {0, 0, 0, in} if sel == 11
 * @param in input bit
 * @param sel selection bits
 * @return bit_array<4>
 */
auto dmux4way(const Bit& in, std::span<const Sel, 2> sel) -> bit_array<4>;

/**
 * @brief 8-way Demultiplexer
 *
 * @param in input bit
 * @param sel selection bits
 * @return bit_array<HalfWord>
 */
auto dmux8way(const Bit& in, std::span<const Sel, 3> sel) -> bit_array<HalfWordSize>;

/**
 * @brief Half-Adder: Compute the sum of two bits.
 *
 * @param pin_a Input bit 1
 * @param pin_b Input bit 2
 * @return std::pair<Bit, Bit> sum, carry
 */
auto half_adder(const Bit& pin_a, const Bit& pin_b) -> std::pair<Bit, Bit>;

/**
 * @brief Full-Adder: Compute the sum of three bits.
 *
 * @param pin_a Input bit 1
 * @param pin_b Input bit 2
 * @param pin_c Input bit 3
 * @return std::pair<Bit, Bit> sum, carry
 */
auto full_adder(const Bit& pin_a, const Bit& pin_b, const Bit& pin_c) -> std::pair<Bit, Bit>;

/**
 * @brief Size-bit adder: Adds two size-bit values
 *
 * @tparam Size Bit size
 * @param pin_a Input 1
 * @param pin_b Input 2
 * @return bit_array<Size> size-bit sum ignoring the carry bit.
 */
template <size_t Size>
auto add(bit_span<Size> pin_a, bit_span<Size> pin_b) -> bit_array<Size>
{
  static_assert(Size > 2, "Size must be greater than 2");
  bit_array<Size> sum;

  Bit carry;

  std::tie(sum[0], carry) = half_adder(pin_a[0], pin_b[0]);

  for (size_t it = 1; it < pin_a.size(); ++it) {
    std::tie(sum[it], carry) = full_adder(pin_a[it], pin_b[it], carry);
  }

  return sum;
}

/**
 * @brief Incrementer
 *
 * @tparam Size
 * @param in Size-bit input
 * @return bit_array<Size>
 */
template <size_t Size>
auto inc(bit_span<Size> in) -> bit_array<Size>
{
  bit_array<Size> rhs = make_bit_array<Size, false>();

  rhs[0] = Bit(1);

  return add<Size>(in, rhs);
}

/**
 * @brief Arithmetic and Logical Unit
 * Computes one of the following functions where x is pin_a and y is pin_b:
 * x+y, x-y, y-x, 0, 1, -1, x, y, -x, -y, !x, !y,
 * x+1, y+1, x-1, y-1, x&y, x|y
 * on two 16-bit inputs,
 *
 * Implementation:
 * if (zx == 1) set x = 0        // 16-bit constant
 * if (nx == 1) set x = !x       // bitwise not
 * if (zy == 1) set y = 0        // 16-bit constant
 * if (ny == 1) set y = !y       // bitwise not
 * if (f == 1)  set out = x + y  // integer 2's complement addition
 * if (f == 0)  set out = x & y  // bitwise and
 * if (no == 1) set out = !out   // bitwise not
 * if (out == 0) set zr = 1
 * if (out < 0) set ng = 1
 * @param pin_a 16-bit input 1
 * @param pin_b 16-bit input 2
 * @param zx make x zero
 * @param nx negate x
 * @param zy make y zero
 * @param ny negate x
 * @param ff x+y if ff == 1 else x&y
 * @param no negate the output
 * @return std::tuple<Register16, Bit, Bit>
 */
auto alu(SpanWord pin_a, SpanWord pin_b, Bit zx, Bit nx, Bit zy, Bit ny, Bit ff,
                Bit no) -> std::tuple<Word, Bit, Bit>;

//template <BitArrayLike T>
//auto operator==(const T& lhs, const T& rhs) -> bool
//{
//  constexpr size_t Size = lhs.size();
//  for (size_t it = 0; it < Size; ++it) {
//    if (lhs[it] != rhs[it]) {
//      return false;
//    }
//  }
//  return true;
//}

}   //namespace hack::defs

#endif
