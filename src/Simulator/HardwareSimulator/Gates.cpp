
#include "HardwareSimulator/Gates.hpp"
#include "HardwareSimulator/Bit.hpp"

namespace hack::defs
{

auto mux(const Bit& pin_a, const Bit& pin_b, const Bit& sel) -> Bit
{
  //return (!sel) ? pin_a : pin_b;
  auto not_sel    = not_gate(sel);
  auto a_and_nsel = and_gate(pin_a, not_sel);
  auto b_and_sel  = and_gate(pin_b, sel);   //NOLINT(readability-suspicious-call-argument)
  auto out        = or_gate(a_and_nsel, b_and_sel);
  return out;
}

auto dmux(const Bit& in, const Bit& sel) -> std::pair<Bit, Bit>
{
  //if (!sel) {
  //  return {in, Bit(0)};
  //}
  //return {Bit(0), in};

  auto not_sel = not_gate(sel);
  auto aa_nsel = and_gate(in, not_sel);
  auto aa_sel  = and_gate(in, sel);
  return {aa_nsel, aa_sel};
}

auto or8way(bit_span<HalfWordSize> bits) -> Bit
{
  Bit out(0);
  for (const auto bit : bits) {
    out = or_gate(out, bit);
    //out |= bit;
  }
  return out;
}

auto mux4way(std::span<const Word, 4> ins, bit_span<2> sel) -> Word
{
  //a,b
  auto mux_ab = mux(std::span(ins[0]), std::span(ins[1]), sel[0]);
  //c,d
  auto mux_cd = mux(std::span(ins[2]), std::span(ins[3]), sel[0]);
  return mux<WordSize>(mux_ab, mux_cd, sel[1]);
};

//NOLINTNEXTLINE(readability-magic-numbers)
auto mux8way(std::span<const Word, 8> ins, std::span<const Sel, 3> sel) -> Word
{
  //a,b,c,d
  auto left = mux4way(std::span{ins}.first<4>(), std::span{sel}.first<2>());
  //e,f,g,h
  auto right = mux4way(std::span{ins}.last<4>(), std::span{sel}.first<2>());
  //combine
  return mux<WordSize>(left, right, sel[2]);
}

auto dmux4way(const Bit& in, std::span<const Sel, 2> sel) -> bit_array<4>
{
  /* 
    The index is opposite due to truth table of dmux4way.
    | sel[1] | sel[0] | a | b | c | d |
    |   0    |   0    | i | 0 | 0 | 0 |
    |   0    |   1    | 0 | i | 0 | 0 |
    |   1    |   0    | 0 | 0 | i | 0 |
    |   1    |   1    | 0 | 0 | 0 | i |

    When reading sel{0,1}, it is read as [0,1] -> [{index:0,value:0}, {index:1, value:1}]. 
    But the array representation should be [{index:0,value:1}, {index:1, value:0}]
    So, sel must be reverse before it is passed if read from string/file.
  */
  auto [oa, ob] = dmux(in, sel[1]);
  auto [a, b]   = dmux(oa, sel[0]);
  auto [c, d]   = dmux(ob, sel[0]);
  return {a, b, c, d};
}

auto dmux8way(const Bit& in, std::span<const Sel, 3> sel) -> bit_array<HalfWordSize>
{
  auto [oa, ob] = dmux(in, sel[2]);

  auto [a, b, c, d] = dmux4way(oa, std::span{sel}.first<2>());
  auto [e, f, g, h] = dmux4way(ob, std::span{sel}.first<2>());
  return {a, b, c, d, e, f, g, h};
}

auto half_adder(const Bit& pin_a, const Bit& pin_b) -> std::pair<Bit, Bit>
{
  auto sum   = xor_gate(pin_a, pin_b);
  auto carry = and_gate(pin_a, pin_b);
  return {sum, carry};
}

auto full_adder(const Bit& pin_a, const Bit& pin_b, const Bit& pin_c) -> std::pair<Bit, Bit>
{
  //sum of a and b
  auto [sum_ab, carry_ab] = half_adder(pin_a, pin_b);

  //sum of a, b and c
  auto [sum, carry_abc] = half_adder(sum_ab, pin_c);

  //carry bit
  auto carry = or_gate(carry_ab, carry_abc);
  return {sum, carry};
}

auto alu(SpanWord pin_a, SpanWord pin_b, Bit zx, Bit nx, Bit zy, Bit ny, Bit ff, Bit no)
  -> std::tuple<Word, Bit, Bit>
{
  //if zx == 1; x = 0
  auto px = mux<WordSize>(pin_a, FalseWord, zx);
  //if zy == 1; y = 0
  auto py = mux<WordSize>(pin_b, FalseWord, zy);   //NOLINT

  //!x
  auto not_xtemp = not_gate(px);
  //if nx == 1; x = !x
  px = mux<WordSize>(px, not_xtemp, nx);

  //!y
  auto not_ytemp = not_gate(py);
  //if ny == 1; y = !y
  py = mux<WordSize>(py, not_ytemp, ny);

  //if (ff==1) out = x + y
  //if (ff==0) out = x & y
  auto a_plus_b = add<WordSize>(px, py);
  auto a_and_b  = and_gate(px, py);
  auto f_out    = mux<WordSize>(a_and_b, a_plus_b, ff);

  //if (no == 1) set out = !out
  auto f_not = not_gate(f_out);
  auto out   = mux<WordSize>(f_out, f_not, no);

  //if (out ==0) set zr = 0
  //if (out < 0) set ng = 1
  auto l_zero = or8way(std::span{out}.first<HalfWordSize>());
  auto r_zero = or8way(std::span{out}.last<HalfWordSize>());

  auto nzr = or_gate(l_zero, r_zero);
  auto zr  = not_gate(nzr);

  auto ng = out.back();
  return std::make_tuple(out, zr, ng);
}

}   //namespace hack::defs