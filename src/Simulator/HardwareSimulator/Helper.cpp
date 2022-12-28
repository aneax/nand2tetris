#include "HardwareSimulator/Helper.hpp"
#include "HardwareSimulator/Bit.hpp"
#include "HardwareSimulator/Gates.hpp"
#include "HardwareSimulator/Memory.hpp"
#include <algorithm>
#include <bits/ranges_algo.h>
#include <stdexcept>
#include <string_view>

namespace hack::defs
{
using std::reverse;

auto int_to_word(int in) -> Word
{
  Word         word;
  unsigned int value = in;
  if (in < 0) {
    value = -in;
  }

  for (size_t it = 0; it < WordSize - 1; ++it) {
    word[it] = value & 1U;
    value    = value >> 1U;
  }

  if (in < 0) {
    for (auto& bit : word) {
      bit = ~bit;
    }
    word               = inc<WordSize>({word});
    word[WordSize - 1] = BitTrue;
  }
  return word;
}


auto word_to_int(const Word& in) -> int
{
  std::uint16_t res  = 0;
  int           it   = WordSize - 2;
  Word          word = in;
  if (word[WordSize - 1]) {
    for (auto& bit : word) {
      bit = ~bit;
    }
  }
  for (; it >= 0; --it) {
    res = res | word[it].value();
    res = res << 1U;
  }
  res        = res >> 1U;
  int result = res;
  if (in[WordSize - 1]) {
    res    = (res + 1U);
    result = -res;
  }
  return result;
}

auto address_to_int(std::span<const Bit, MemoryAddressSize> in) -> int
{
  std::uint16_t res = 0;
  for (int it = in.size() - 1; it >= 0; --it) {
    res = res | in[it].value();
    res = res << 1U;
  }
  res = res >> 1U;
  return static_cast<int>(res);
}

auto load_rom(ROM32K& rom, std::span<const int> instructions) -> void
{
  for (size_t it = 0; it < instructions.size(); ++it) {
    auto addr  = hack::defs::int_to_word(static_cast<int>(it));
    auto input = int_to_word(instructions[it]);
    auto word =
      rom32k(rom, input, std::span{addr}.first<MemoryAddressSize>(), BitTrue);
  }
}


auto load_rom(ROM32K& rom, const std::vector<Word>& instructions) -> void
{
  for (size_t it = 0; it < instructions.size(); ++it) {
    auto addr = hack::defs::int_to_word(static_cast<int>(it));
    auto word = rom32k(rom, instructions[it],
                       std::span{addr}.first<MemoryAddressSize>(), BitTrue);
  }
}


auto string_to_word(std::span<std::string_view> str_insts) -> std::vector<Word>
{
  throw std::runtime_error("Unimplemented function");
  std::vector<Word> instructions;
  instructions.reserve(str_insts.size());
  for (const auto& str : str_insts) {
    Word word;
    for (int it = 1; it < WordSize; ++it) {
      char           ch   = str[it];
      constexpr char Zero = '0';
      constexpr char One  = '1';

      if ((ch != One) && (ch != Zero)) {
        std::string error("Parse error. Unknown character: ");
        error.push_back(ch);
        throw std::runtime_error(error);
      }

      int value = static_cast<int>(ch) - Zero;
      word[it]  = Bit(value);
    }
    //std::ranges::reverse(word); 
    instructions.push_back(word);
  }
  return instructions;
}

}   //namespace hack::defs
