
#include "HardwareSimulator/Bit.hpp"
#include "HardwareSimulator/Gates.hpp"
//#include "fmt/format.h"
#include <ranges>
#include <iostream>
#include <algorithm>
#include <boost/ut.hpp>
#include <vector>

using namespace boost::ut;
using namespace hack::defs;

template <typename T>
constexpr auto get_true_false() -> std::pair<T, T>
{
  T false_value;
  T true_value;
  if constexpr (std::is_same_v<T, Bit>) {
    true_value  = T(1);
    false_value = T(0);
  } else {
    std::fill(std::begin(true_value), std::end(true_value), Bit(1));
    std::fill(std::begin(false_value), std::end(false_value), Bit(0));
  }
  return std::make_pair(true_value, false_value);
}

template <typename T>
static void test_basic_gates()
{
  //internal::Gate gate(inputs_00, inputs_01);

  T true_v;
  T false_v;
  std::tie(true_v, false_v) = get_true_false<T>();

  "nand"_test = [&]() {
    expect((nand_gate(false_v, false_v) == true_v) >> fatal)
      << " 0 nand 0 = 1 \n";
    expect((nand_gate(false_v, true_v) == true_v) >> fatal)
      << " 0 nand 1 = 1 \n";
    expect((nand_gate(true_v, false_v) == true_v) >> fatal)
      << " 1 nand 0 = 1 \n";
    expect((nand_gate(true_v, true_v) == false_v) >> fatal)
      << " 1 nand 1 = 0 \n";
  };

  "not"_test = [&]() {
    expect((not_gate(false_v) == true_v) >> fatal) << " not 0  = 1 \n";
    expect((not_gate(true_v) == false_v) >> fatal) << " not 1  = 0 \n";
  };

  "and"_test = [&]() {
    expect((and_gate(false_v, false_v) == false_v) >> fatal)
      << " 0 and 0 = 0 \n";
    expect((and_gate(false_v, true_v) == false_v) >> fatal)
      << " 0 and 1 = 0 \n";
    expect((and_gate(true_v, false_v) == false_v) >> fatal)
      << " 1 and 0 = 0 \n";
    expect((and_gate(true_v, true_v) == true_v) >> fatal) << " 1 and 1 = 1 \n";
  };

  "or"_test = [&]() {
    expect((or_gate(false_v, false_v) == false_v) >> fatal) << " 0 or 0 = 0 \n";
    expect((or_gate(false_v, true_v) == true_v) >> fatal) << " 0 or 1 = 1 \n";
    expect((or_gate(true_v, false_v) == true_v) >> fatal) << " 1 or 0 = 1 \n";
    expect((or_gate(true_v, true_v) == true_v) >> fatal) << " 1 or 1 = 1 \n";
  };

  "xor"_test = [&]() {
    expect((xor_gate(false_v, false_v) == false_v) >> fatal)
      << " 0 or 0 = 0 \n";
    expect((xor_gate(false_v, true_v) == true_v) >> fatal) << " 0 or 1 = 1 \n";
    expect((xor_gate(true_v, false_v) == true_v) >> fatal) << " 1 or 0 = 1 \n";
    expect((xor_gate(true_v, true_v) == false_v) >> fatal) << " 1 or 1 = 0 \n";
  };
}

void test_mux_variants()
{
  using Bit4 = bit_array<4>;

  "mux"_test = [&]() {
    static const std::vector<Bit4> Data = {
      {0, 0, 0, 0},
      {0, 0, 1, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 1},
      {1, 0, 0, 1},
      {1, 0, 1, 0},
      {1, 1, 0, 1},
      {1, 1, 1, 1},
    };

    for (const auto& item : Data) {
      expect((mux(item[0], item[1], item[2]) == item[3]) >> fatal) << "";
    }
  };

  "mux4way"_test = [] {
    using InputType = std::tuple<std::array<Word, 4>, Sel2, Word>;
    static const std::vector<InputType> Data = {
      {std::array<Word, 4>{
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
       Sel2{0, 0}, Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {std::array<Word, 4>{
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
       Sel2{0, 1}, Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {std::array<Word, 4>{
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
       Sel2{1, 0}, Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {std::array<Word, 4>{
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
       Sel2{1, 1}, Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {std::array<Word, 4>{
 Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0},
 Word{1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0},
 Word{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
 Word{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}},
       Sel2{0, 0}, Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0}},
      {std::array<Word, 4>{
 Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0},
 Word{1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0},
 Word{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
 Word{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}},
       Sel2{0, 1}, Word{1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0}},
      {std::array<Word, 4>{
 Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0},
 Word{1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0},
 Word{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
 Word{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}},
       Sel2{1, 0}, Word{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}},
      {std::array<Word, 4>{
 Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0},
 Word{1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0},
 Word{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
 Word{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}},
       Sel2{1, 1}, Word{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}},
    };

    const size_t iteration = Data.size();
    for (size_t it = 0; it < iteration; ++it) {
      const auto& [ins, r_sel, out] = Data[it];

      //The supplied sel is in reverse order. Must be reversed. @see dmux4way
      auto sel = r_sel;
      std::reverse(sel.begin(), sel.end());
      auto actual = mux4way({ins}, {sel});

      expect((actual == out)) << "Input:" << it << ".Required:" << out
                              << ", Actual:" << actual << "\n";
    }
  };

  "mux8way"_test = [&] {
    using InputType = std::tuple<std::array<Word, HalfWordSize>, Sel3, Word>;
    //// clang-format off
    [[maybe_unused]] static const std::vector<InputType> Data = {
      {std::array<Word, 8>{
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
       Sel3{0, 0, 0}, Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
       Sel3{0, 0, 1}, Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
       Sel3{0, 1, 0}, Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
       Sel3{0, 1, 1}, Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
       Sel3{1, 0, 0}, Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
       Sel3{1, 0, 1}, Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
       Sel3{1, 1, 0}, Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
       Sel3{1, 1, 1}, Word{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0},
         Word{0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1},
         Word{0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0},
         Word{0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1},
         Word{0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0},
         Word{0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
         Word{0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0},
         Word{1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1}},
       Sel3{0, 0, 0}, Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0},
         Word{0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1},
         Word{0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0},
         Word{0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1},
         Word{0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0},
         Word{0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
         Word{0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0},
         Word{1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1}},
       Sel3{0, 0, 1}, Word{0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0},
         Word{0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1},
         Word{0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0},
         Word{0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1},
         Word{0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0},
         Word{0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
         Word{0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0},
         Word{1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1}},
       Sel3{0, 1, 0}, Word{0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0},
         Word{0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1},
         Word{0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0},
         Word{0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1},
         Word{0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0},
         Word{0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
         Word{0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0},
         Word{1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1}},
       Sel3{0, 1, 1}, Word{0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0},
         Word{0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1},
         Word{0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0},
         Word{0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1},
         Word{0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0},
         Word{0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
         Word{0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0},
         Word{1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1}},
       Sel3{1, 0, 0}, Word{0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0},
         Word{0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1},
         Word{0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0},
         Word{0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1},
         Word{0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0},
         Word{0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
         Word{0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0},
         Word{1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1}},
       Sel3{1, 0, 1}, Word{0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1}},
      {std::array<Word, 8>{
         Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0},
         Word{0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1},
         Word{0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0},
         Word{0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1},
         Word{0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0},
         Word{0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
         Word{0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0},
         Word{1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1}},
       Sel3{1, 1, 0}, Word{0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0}},
      {
       std::array<Word, 8>{
          Word{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0},
          Word{0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1},
          Word{0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0},
          Word{0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1},
          Word{0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0},
          Word{0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1},
          Word{0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0},
          Word{1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1}},
       Sel3{1, 1, 1},
       Word{1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1},
       }
    };

    const size_t iteration = Data.size();
    for (size_t it = 0; it < iteration; ++it) {
      const auto& [ins, r_sel, out] = Data[it];

      //The supplied sel is in reverse order. Must be reversed. @see dmux4way
      auto sel = r_sel;
      std::reverse(sel.begin(), sel.end());
      auto actual = mux8way(ins, sel);

      expect((actual == out)) << "Input:" << it << ".Required:" << out
                              << ", Actual:" << actual << "\n";
    }
  };
}

void test_dmux_variants()
{
  "dmux"_test = [&]() {
    using Data = std::tuple<Bit, Bit, Bit, Bit>;

    std::vector<Data> data = {
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {1, 0, 1, 0},
      {1, 1, 0, 1},
    };

    for (const auto& item : data) {
      const auto& [in, sel, ra, rb] = item;

      auto [actual_a, actual_b] = dmux(in, sel);
      expect((std::tie(ra, rb) == std::tie(actual_a, actual_b)) >> fatal) << "";
    }
  };

  "dmux4way"_test = [&] {
    using Input               = std::pair<Bit, bit_array<2>>;
    std::vector<Input> inputs = {
      {0, {0, 0}},
      {0, {0, 1}},
      {0, {1, 0}},
      {0, {1, 1}},
      {1, {0, 0}},
      {1, {0, 1}},
      {1, {1, 0}},
      {1, {1, 1}},
    };
    std::vector<bit_array<4>> results = {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
    };

    expect((inputs.size() == results.size()) >> fatal)
      << "Input and Result size must be equal";

    size_t sz = inputs.size();
    for (size_t it = 0; it < sz; ++it) {
      auto& [in, sel] = inputs[it];
      auto& required  = results[it];

      std::reverse(sel.begin(), sel.end());

      auto actual = dmux4way(in, sel);

      for (size_t bit = 0; bit < required.size(); ++bit) {
        expect((required[bit] == actual[bit]))
          << "Input:" << it << " at" << bit << ". Required:" << required[bit]
          << ", Actual:" << actual[bit] << "\n";
      };
    }
  };

  "dmux8way"_test = [&] {
    using Input               = std::pair<Bit, bit_array<3>>;
    using Output              = bit_array<HalfWordSize>;
    std::vector<Input> inputs = {
      {0, {0, 0, 0}},
      {0, {0, 0, 1}},
      {0, {0, 1, 0}},
      {0, {0, 1, 1}},
      {0, {1, 0, 0}},
      {0, {1, 0, 1}},
      {0, {1, 1, 0}},
      {0, {1, 1, 1}},
      {1, {0, 0, 0}},
      {1, {0, 0, 1}},
      {1, {0, 1, 0}},
      {1, {0, 1, 1}},
      {1, {1, 0, 0}},
      {1, {1, 0, 1}},
      {1, {1, 1, 0}},
      {1, {1, 1, 1}},
    };

    std::vector<Output> results = {
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0, 0, 0, 0},
      {0, 1, 0, 0, 0, 0, 0, 0},
      {0, 0, 1, 0, 0, 0, 0, 0},
      {0, 0, 0, 1, 0, 0, 0, 0},
      {0, 0, 0, 0, 1, 0, 0, 0},
      {0, 0, 0, 0, 0, 1, 0, 0},
      {0, 0, 0, 0, 0, 0, 1, 0},
      {0, 0, 0, 0, 0, 0, 0, 1},
    };

    expect((inputs.size() == results.size()) >> fatal)
      << "Input and Result size must be equal";

    size_t sz = inputs.size();
    for (size_t it = 0; it < sz; ++it) {
      auto& [in, sel] = inputs[it];
      auto& required  = results[it];

      std::reverse(sel.begin(), sel.end());

      auto actual = dmux8way(in, sel);

      for (size_t bit = 0; bit < required.size(); ++bit) {
        expect((required[bit] == actual[bit]))
          << "Input:" << it << " at" << bit << ". Required:" << required[bit]
          << ", Actual:" << actual[bit] << ". r" << required << "!= a" << actual
          << "\n";
      };
    }
  };
}

void test_multi_way_gates()
{
  "or8way"_test = [&]() {
    using Bit8 = bit_array<8>;   //NOLINT(readability-magic-numbers)

    //NOLINTNEXTLINE(readability-magic-numbers)
    std::array<std::pair<Bit8, Bit>, 8> data = {
  //  in sel a b
      std::make_pair(Bit8{0, 0, 0, 0, 0, 0, 0, 0},
      0),
      {{0, 0, 0, 0, 0, 0, 0, 1}, 1},
      {{1, 0, 0, 0, 0, 0, 0, 0}, 1},
      {{1, 0, 0, 0, 0, 0, 0, 1}, 1},
      {{1, 0, 0, 0, 1, 0, 0, 1}, 1}
    };

    for (const auto& item : data) {
      expect((or8way(item.first) == item.second) >> fatal) << "";
    }
  };

  "half_adder"_test = [] {
    using InputType                          = std::tuple<Bit, Bit, Bit, Bit>;
    static const std::vector<InputType> Data = {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {1, 0, 1, 0},
      {1, 1, 0, 1},
    };

    const size_t iteration = Data.size();
    for (size_t it = 0; it < iteration; ++it) {
      const auto& [pa, pb, sum_required, carry_required] = Data[it];

      const auto [sum_actual, carry_actual] = half_adder(pa, pb);

      expect((std::tie(sum_required, carry_required)
              == std::tie(sum_actual, carry_actual)))
        << "it:" << it << "\n\t   a    :" << pa << "\n\t   b    :" << pb
        << "\n\trequired:" << sum_required << ", " << carry_required
        << "\n\tactual  :" << sum_actual << ", " << carry_actual;
    }
  };

  "full_adder"_test = [] {
    using InputType = std::array<Bit, 5>;   //NOLINT

    static const std::vector<InputType> Data = {
      {0, 0, 0, 0, 0},
      {0, 0, 1, 1, 0},
      {0, 1, 0, 1, 0},
      {0, 1, 1, 0, 1},
      {1, 0, 0, 1, 0},
      {1, 0, 1, 0, 1},
      {1, 1, 0, 0, 1},
      {1, 1, 1, 1, 1},
    };

    const size_t iteration = Data.size();
    for (size_t it = 0; it < iteration; ++it) {
      const auto& [pa, pb, pc, sum_required, carry_required] = Data[it];

      const auto [sum_actual, carry_actual] = full_adder(pa, pb, pc);

      expect((std::tie(sum_required, carry_required)
              == std::tie(sum_actual, carry_actual)))
        << "it:" << it << "\n\t   a    :" << pa << "\n\t   b    :" << pb
        << "\n\t   c    :" << pc << "\n\trequired:" << sum_required << ", "
        << carry_required << "\n\tactual  :" << sum_actual << ", "
        << carry_actual;
    }
  };

  "add16"_test = [] {
    using InputType = std::tuple<const Word, const Word, const Word>;
    static const std::vector<InputType> Data = {
      {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
      {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}},
      {{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
       {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
      {{0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1},
       {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
       {0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1}},
      {{0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0},
       {1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0},
       {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}},
    };

    const size_t iteration = Data.size();
    for (size_t it = 0; it < iteration; ++it) {
      const auto& [r_pa, r_pb, r_expected] = Data[it];

      //Anything binary read is always in reverse order of the required order.
      //word[15] is the MSB
      auto pa = r_pa;
      auto pb = r_pb;
      std::ranges::reverse(pa);
      std::ranges::reverse(pb);

      auto expected = r_expected;
      std::ranges::reverse(expected);

      auto actual = add<WordSize>(pa, pb);
      expect((actual == expected))
        << "it:" << it << "\n\t   a    :" << pa << "\n\t   b    :" << pb
        << "\n\trequired:" << expected << "\n\tactual  :" << actual;
    }
  };

  "inc16"_test = [] {
    using InputType                          = std::tuple<Word, Word>;
    static const std::vector<InputType> Data = {
      {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}},
      {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
      {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0}},
      {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}},
    };

    const size_t iteration = Data.size();
    for (size_t it = 0; it < iteration; ++it) {
      const auto& [r_pa, r_expected] = Data[it];

      //Anything binary read is always in reverse order of the required order.
      //word[15] is the MSB
      auto pa = r_pa;
      std::ranges::reverse(pa);

      auto expected = r_expected;
      std::ranges::reverse(expected);

      auto actual = inc<WordSize>(pa);
      expect((actual == expected))
        << "it:" << it << "\n\t   a    :" << pa << "\n\trequired:" << expected
        << "\n\tactual  :" << actual;
    }
  };
}

struct ALU {
    Word pin_a;
    Word pin_b;

    Bit zx;
    Bit nx;
    Bit zy;
    Bit ny;
    Bit ff;
    Bit no;

    Word out;

    Bit zr;
    Bit ng;

    ALU(Word&& pa, Word&& pb, Bit zx, Bit nx, Bit zy, Bit ny, Bit ff, Bit no,
        Word out, Bit zr, Bit ng)
        : pin_a(pa)
        , pin_b(pb)
        , zx(zx)
        , nx(nx)
        , zy(zy)
        , ny(ny)
        , ff(ff)
        , no(no)
        , out(out)
        , zr(zr)
        , ng(ng)
    {
    }
};

void test_alu()
{
  static const std::vector<ALU> Inputs{

    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 1, 0, 1, 0, 1, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 1, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 1, 1, 1, 1, 1, 1,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 1, 1, 1, 0, 1, 0,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 0, 1, 1, 0, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 1, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 1, 1, 0, 0, 0, 0,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 0, 1, 1, 0, 1,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 1, 1, 0, 0, 0, 1,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 1, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 0, 1, 1, 1, 1,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 1, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 1, 1, 0, 0, 1, 1,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 1, 1, 1, 1, 1,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 1, 1, 0, 1, 1, 1,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 1, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 0, 1, 1, 1, 0,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 1, 1, 0, 0, 1, 0,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 0, 0, 0, 1, 0,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 1, 0, 0, 1, 1,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 0, 0, 1, 1, 1,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 0, 0, 0, 0, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 1, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 1, 0, 1, 0, 1,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 1, 0, 1, 0, 1, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 1, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 1, 1, 1, 1, 1, 1,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 1, 1, 1, 0, 1, 0,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 0, 0, 1, 1, 0, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 1, 1, 0, 0, 0, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 0, 0, 1, 1, 0, 1,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 1, 1, 0, 0, 0, 1,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 0, 0, 1, 1, 1, 1,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 1, 1, 0, 0, 1, 1,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 0, 1, 1, 1, 1, 1,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 1, 1, 0, 1, 1, 1,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 0, 0, 1, 1, 1, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 1, 1, 0, 0, 1, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 0, 0, 0, 0, 1, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 0, 1, 0, 0, 1, 1,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 0, 0, 0, 1, 1, 1,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0}, 0, 1),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 0, 0, 0, 0, 0, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 0, 0),
    ALU({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, 0, 1, 0, 1, 0, 1,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1}, 0, 0),

  };

  const size_t iteration = Inputs.size();
  for (size_t it = 0; it < iteration; ++it) {
    const auto& in = Inputs[it];

    auto pa       = in.pin_a;
    auto pb       = in.pin_b;
    auto expected = in.out;

    std::ranges::reverse(pa);
    std::ranges::reverse(pb);
    std::ranges::reverse(expected);

    auto res = alu(pa, pb, in.zx, in.nx, in.zy, in.ny, in.ff, in.no);
    expect((res == std::tie(expected, in.zr, in.ng)) >> fatal) << "";
  }
}

//NOLINTNEXTLINE
int main()
{
  test_basic_gates<Bit>();
  test_basic_gates<Word>();
  test_mux_variants();
  test_dmux_variants();
  test_multi_way_gates();
  test_alu();
}