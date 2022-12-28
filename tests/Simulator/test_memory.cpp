#include "HardwareSimulator/Bit.hpp"
#include "HardwareSimulator/Helper.hpp"
#include "HardwareSimulator/Memory.hpp"
#include "data.hpp"
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <boost/ut.hpp>
#include <bitset>
#include <numeric>
#include <ranges>

using namespace hack::defs;

auto test_conversion() -> void
{
  //NOLINTBEGIN
  std::vector<int> inputs{
    1,    0,     3,     11111, 12345,  2,      4,      8,    16,    32,
    64,   128,   256,   512,   1024,   2048,   4096,   8192, 16384, 32767,
    -1,   -2,    -3,    -5,    -9,     -17,    -33,    -65,  -129,  -257,
    -513, -1025, -4097, -8193, -16385, -32123, -32768,
  };
  //NOLINTEND

  for (size_t it = 0; it < inputs.size(); ++it) {
    //std::cout << "-------------------------\n";
    //std::cout << inputs[it] << "\n";
    Word word = int_to_word(inputs[it]);
    //std::cout << word << "\n";
    int value = word_to_int(word);
    //std::reverse(word.begin(), word.end());
    //std::cout << word << "\n";
    //std::cout << std::bitset<WordSize>(inputs[it]) << "\n";
    //std::cout << value << std::endl;
    boost::ut::expect((value == inputs[it]))
      << "at index:" << it << "." << value << "!=" << inputs[it] << "\n";
  }
}

template <bool PrintOnly = false>
auto test_memory_bit() -> void
{
  using Input                   = std::pair<Bit, Bit>;
  using Output                  = Input;
  BitRegister        b_register = BitFalse;
  std::vector<Input> inputs     = {
    {BitFalse, BitFalse},
    {BitFalse,  BitTrue},
    { BitTrue, BitFalse},
    { BitTrue,  BitTrue},
    {BitFalse, BitFalse},
    { BitTrue, BitFalse},
    {BitFalse,  BitTrue},
    { BitTrue,  BitTrue},
    {BitFalse, BitFalse},
    {BitFalse, BitFalse},
    { BitTrue, BitFalse},
    {BitFalse,  BitTrue},
    {BitFalse, BitFalse},
    { BitTrue,  BitTrue},
    {BitFalse, BitFalse},
    {BitFalse,  BitTrue},
  };

  std::vector<Output> results{
    {BitFalse, BitFalse},
    {BitFalse, BitFalse},
    {BitFalse, BitFalse},
    {BitFalse,  BitTrue},
    { BitTrue,  BitTrue},
    { BitTrue,  BitTrue},
    { BitTrue, BitFalse},
    {BitFalse,  BitTrue},
    { BitTrue,  BitTrue},
    { BitTrue,  BitTrue},
    { BitTrue,  BitTrue},
    { BitTrue, BitFalse},
    {BitFalse, BitFalse},
    {BitFalse,  BitTrue},
    { BitTrue,  BitTrue},
    { BitTrue, BitFalse},
  };

  {
    using namespace boost::ut;
    expect((inputs.size() == results.size() >> fatal))
      << "Input and Output size must be equal.";
    Bit before = b_register;
    Bit after  = b_register;

    if constexpr (PrintOnly) {
      std::cout << "|  in  |  load   |  before  |  after  |  register  |\n";
    }
    const size_t iteration = inputs.size();
    for (size_t iter = 0; iter < iteration; ++iter) {
      const auto& [in, load]                        = inputs[iter];
      const auto& [expected_before, expected_after] = results[iter];

      before = bit_register(b_register, in, load);
      after  = b_register;

      if constexpr (PrintOnly) {
        std::cout << "|  " << in << "   |    " << load << "    |     " << before
                  << "    |    " << after << "    |     " << b_register
                  << "      |\n";
      } else {
        expect((before == expected_before))
          << "Before Failed at:" << iter << "." << before
          << "!=" << expected_before << "\n";
        expect((after == expected_after))
          << "After Failed at:" << iter << "." << after
          << "!=" << expected_after << "\n";
        expect((after == b_register)) << "Register Failed at:" << iter << "."
                                      << after << "!=" << b_register << "\n";
      }
    }
  }
}

template <bool PrintOnly = false>
auto test_register() -> void
{
  using Input  = std::pair<Word, Bit>;
  using Output = std::pair<Word, Word>;
  //NOLINTBEGIN
  std::vector<Input> inputs = {
    {     int_to_word(0), BitFalse},
    {     int_to_word(0),  BitTrue},
    {int_to_word(-32123), BitFalse},
    { int_to_word(11111), BitFalse},
    {int_to_word(-32123),  BitTrue},
    {   int_to_word(-11),  BitTrue},
    {int_to_word(-32123),  BitTrue},
    {    int_to_word(-5), BitFalse},
    { int_to_word(12345), BitFalse},
    { int_to_word(12345),  BitTrue},
    {     int_to_word(1), BitFalse},
    {     int_to_word(1),  BitTrue},
    {     int_to_word(2), BitFalse},
    {     int_to_word(2),  BitTrue},
    {int_to_word(-32768), BitFalse},
    {int_to_word(-32768),  BitTrue},
  };

  std::vector<Output> results{
    {     int_to_word(0),      int_to_word(0)},
    {     int_to_word(0),      int_to_word(0)},
    {     int_to_word(0),      int_to_word(0)},
    {     int_to_word(0),      int_to_word(0)},
    {     int_to_word(0), int_to_word(-32123)},
    {int_to_word(-32123),    int_to_word(-11)},
    {   int_to_word(-11), int_to_word(-32123)},
    {int_to_word(-32123), int_to_word(-32123)},
    {int_to_word(-32123), int_to_word(-32123)},
    {int_to_word(-32123),  int_to_word(12345)},
    { int_to_word(12345),  int_to_word(12345)},
    { int_to_word(12345),      int_to_word(1)},
    {     int_to_word(1),      int_to_word(1)},
    {     int_to_word(1),      int_to_word(2)},
    {     int_to_word(2),      int_to_word(2)},
    {     int_to_word(2), int_to_word(-32768)},
  };
  //NOLINTEND

  {
    using namespace boost::ut;
    expect((inputs.size() == results.size() >> fatal))
      << "Input and Output size must be equal.";

    Register b_register = FalseWord;
    Word     before     = b_register;
    Word     after      = b_register;

    if constexpr (PrintOnly) {
      std::cout << "|   in   |  load   |  before  |  after  |  register  |\n";
    }
    const size_t iteration = inputs.size();
    for (size_t iter = 0; iter < iteration; ++iter) {
      const auto& [in, load]                        = inputs[iter];
      const auto& [expected_before, expected_after] = results[iter];

      before = register_n(b_register, std::span{in}, load);
      after  = b_register;

      if constexpr (PrintOnly) {
        std::cout << "|  " << in << "   |    " << load << "    |     " << before
                  << "    |    " << after << "    |     " << b_register
                  << "      |\n";
      } else {
        expect((before == expected_before))
          << "Before Failed at:" << iter << "." << before
          << "!=" << expected_before << "\n";
        expect((after == expected_after))
          << "After Failed at:" << iter << "." << after
          << "!=" << expected_after << "\n";
        expect((after == b_register)) << "Register Failed at:" << iter << "."
                                      << after << "!=" << b_register << "\n";
      }
    }
  }
}

template <bool PrintOnly = false>
auto test_ram_8() -> void
{
  using Input = std::tuple<Word, Bit, Word>;

  //NOLINTBEGIN
  std::vector<Input> inputs{
    {    int_to_word(0), 0, int_to_word(0)},
    {    int_to_word(0), 0, int_to_word(0)},
    {    int_to_word(0), 1, int_to_word(0)},
    {int_to_word(11111), 0, int_to_word(0)},
    {int_to_word(11111), 1, int_to_word(1)},
    {int_to_word(11111), 0, int_to_word(0)},
    { int_to_word(3333), 0, int_to_word(3)},
    { int_to_word(3333), 1, int_to_word(3)},
    { int_to_word(3333), 0, int_to_word(3)},
    { int_to_word(3333), 0, int_to_word(1)},
    { int_to_word(7777), 0, int_to_word(1)},
    { int_to_word(7777), 1, int_to_word(7)},
    { int_to_word(7777), 0, int_to_word(7)},
    { int_to_word(7777), 0, int_to_word(3)},
    { int_to_word(7777), 0, int_to_word(7)},
    { int_to_word(7777), 0, int_to_word(0)},
    { int_to_word(7777), 0, int_to_word(1)},
    { int_to_word(7777), 0, int_to_word(2)},
    { int_to_word(7777), 0, int_to_word(3)},
    { int_to_word(7777), 0, int_to_word(4)},
    { int_to_word(7777), 0, int_to_word(5)},
    { int_to_word(7777), 0, int_to_word(6)},
    { int_to_word(7777), 0, int_to_word(7)},
    {int_to_word(21845), 1, int_to_word(0)},
    {int_to_word(21845), 1, int_to_word(1)},
    {int_to_word(21845), 1, int_to_word(2)},
    {int_to_word(21845), 1, int_to_word(3)},
    {int_to_word(21845), 1, int_to_word(4)},
    {int_to_word(21845), 1, int_to_word(5)},
    {int_to_word(21845), 1, int_to_word(6)},
    {int_to_word(21845), 1, int_to_word(7)},
    {int_to_word(21845), 0, int_to_word(0)},
    {int_to_word(21845), 0, int_to_word(1)},
    {int_to_word(21845), 0, int_to_word(2)},
    {int_to_word(21845), 0, int_to_word(3)},
    {int_to_word(21845), 0, int_to_word(4)},
    {int_to_word(21845), 0, int_to_word(5)},
    {int_to_word(21845), 0, int_to_word(6)},
    {int_to_word(21845), 0, int_to_word(7)},
    {int_to_word(21846), 1, int_to_word(0)},
    {int_to_word(21846), 0, int_to_word(0)},
    {int_to_word(21846), 0, int_to_word(1)},
    {int_to_word(21846), 0, int_to_word(2)},
    {int_to_word(21846), 0, int_to_word(3)},
    {int_to_word(21846), 0, int_to_word(4)},
    {int_to_word(21846), 0, int_to_word(5)},
    {int_to_word(21846), 0, int_to_word(6)},
    {int_to_word(21846), 0, int_to_word(7)},
    {int_to_word(21845), 1, int_to_word(0)},
    {int_to_word(21846), 1, int_to_word(1)},
    {int_to_word(21846), 0, int_to_word(0)},
    {int_to_word(21846), 0, int_to_word(1)},
    {int_to_word(21846), 0, int_to_word(2)},
    {int_to_word(21846), 0, int_to_word(3)},
    {int_to_word(21846), 0, int_to_word(4)},
    {int_to_word(21846), 0, int_to_word(5)},
    {int_to_word(21846), 0, int_to_word(6)},
    {int_to_word(21846), 0, int_to_word(7)},
    {int_to_word(21845), 1, int_to_word(1)},
    {int_to_word(21846), 1, int_to_word(2)},
    {int_to_word(21846), 0, int_to_word(0)},
    {int_to_word(21846), 0, int_to_word(1)},
    {int_to_word(21846), 0, int_to_word(2)},
    {int_to_word(21846), 0, int_to_word(3)},
    {int_to_word(21846), 0, int_to_word(4)},
    {int_to_word(21846), 0, int_to_word(5)},
    {int_to_word(21846), 0, int_to_word(6)},
    {int_to_word(21846), 0, int_to_word(7)},
    {int_to_word(21845), 1, int_to_word(2)},
    {int_to_word(21846), 1, int_to_word(3)},
    {int_to_word(21846), 0, int_to_word(0)},
    {int_to_word(21846), 0, int_to_word(1)},
    {int_to_word(21846), 0, int_to_word(2)},
    {int_to_word(21846), 0, int_to_word(3)},
    {int_to_word(21846), 0, int_to_word(4)},
    {int_to_word(21846), 0, int_to_word(5)},
    {int_to_word(21846), 0, int_to_word(6)},
    {int_to_word(21846), 0, int_to_word(7)},
    {int_to_word(21845), 1, int_to_word(3)},
    {int_to_word(21846), 1, int_to_word(4)},
    {int_to_word(21846), 0, int_to_word(0)},
    {int_to_word(21846), 0, int_to_word(1)},
    {int_to_word(21846), 0, int_to_word(2)},
    {int_to_word(21846), 0, int_to_word(3)},
    {int_to_word(21846), 0, int_to_word(4)},
    {int_to_word(21846), 0, int_to_word(5)},
    {int_to_word(21846), 0, int_to_word(6)},
    {int_to_word(21846), 0, int_to_word(7)},
    {int_to_word(21845), 1, int_to_word(4)},
    {int_to_word(21846), 1, int_to_word(5)},
    {int_to_word(21846), 0, int_to_word(0)},
    {int_to_word(21846), 0, int_to_word(1)},
    {int_to_word(21846), 0, int_to_word(2)},
    {int_to_word(21846), 0, int_to_word(3)},
    {int_to_word(21846), 0, int_to_word(4)},
    {int_to_word(21846), 0, int_to_word(5)},
    {int_to_word(21846), 0, int_to_word(6)},
    {int_to_word(21846), 0, int_to_word(7)},
    {int_to_word(21845), 1, int_to_word(5)},
    {int_to_word(21846), 1, int_to_word(6)},
    {int_to_word(21846), 0, int_to_word(0)},
    {int_to_word(21846), 0, int_to_word(1)},
    {int_to_word(21846), 0, int_to_word(2)},
    {int_to_word(21846), 0, int_to_word(3)},
    {int_to_word(21846), 0, int_to_word(4)},
    {int_to_word(21846), 0, int_to_word(5)},
    {int_to_word(21846), 0, int_to_word(6)},
    {int_to_word(21846), 0, int_to_word(7)},
    {int_to_word(21845), 1, int_to_word(6)},
    {int_to_word(21846), 1, int_to_word(7)},
    {int_to_word(21846), 0, int_to_word(0)},
    {int_to_word(21846), 0, int_to_word(1)},
    {int_to_word(21846), 0, int_to_word(2)},
    {int_to_word(21846), 0, int_to_word(3)},
    {int_to_word(21846), 0, int_to_word(4)},
    {int_to_word(21846), 0, int_to_word(5)},
    {int_to_word(21846), 0, int_to_word(6)},
    {int_to_word(21846), 0, int_to_word(7)},
    {int_to_word(21845), 1, int_to_word(7)},
    {int_to_word(21845), 0, int_to_word(0)},
    {int_to_word(21845), 0, int_to_word(1)},
    {int_to_word(21845), 0, int_to_word(2)},
    {int_to_word(21845), 0, int_to_word(3)},
    {int_to_word(21845), 0, int_to_word(4)},
    {int_to_word(21845), 0, int_to_word(5)},
    {int_to_word(21845), 0, int_to_word(6)},
    {int_to_word(21845), 0, int_to_word(7)},
  };

  std::vector<Word> results{
    {int_to_word(0)},     {int_to_word(0)},     {int_to_word(0)},
    {int_to_word(0)},     {int_to_word(11111)}, {int_to_word(0)},
    {int_to_word(0)},     {int_to_word(3333)},  {int_to_word(3333)},
    {int_to_word(11111)}, {int_to_word(11111)}, {int_to_word(7777)},
    {int_to_word(7777)},  {int_to_word(3333)},  {int_to_word(7777)},
    {int_to_word(0)},     {int_to_word(11111)}, {int_to_word(0)},
    {int_to_word(3333)},  {int_to_word(0)},     {int_to_word(0)},
    {int_to_word(0)},     {int_to_word(7777)},  {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21846)}, {int_to_word(21846)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21846)}, {int_to_word(21845)},
    {int_to_word(21846)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21846)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21846)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21846)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21846)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21846)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21846)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21846)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21846)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21846)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21846)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21846)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21846)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)}, {int_to_word(21845)}, {int_to_word(21845)},
    {int_to_word(21845)},
  };
  //NOLINTEND

  {
    using namespace boost::ut;
    expect((inputs.size() == results.size() >> fatal))
      << "Input and Output size must be equal.";

    RAM8 ram;
    Word before = FalseWord;

    if constexpr (PrintOnly) {
      std::cout
        << "iter |   in   |  load   |  address  |  before  |  after  |\n";
    }
    const size_t iteration = inputs.size();
    for (size_t iter = 0; iter < iteration; ++iter) {
      auto [in, load, address] = inputs[iter];
      const auto& expected     = results[iter];
      before                   = ram8(ram, std::span{in}, load,
                                      std::span{address}.first<RAM8AddressSize>());

      if constexpr (PrintOnly) {
        std::printf(" %3lu | %5d | %5d | %5d | %5d | %5d |", iter,
                    word_to_int(in), static_cast<int>(load),
                    word_to_int(address), word_to_int(before),
                    word_to_int(ram.data[word_to_int(address)]));
        for (size_t it = 0; it < hack::defs::RAM8::Size; ++it) {
          std::printf("%2lu:%5d, ", it, word_to_int(ram.data[it]));
        }
        std::printf("\n");
      } else {
        auto word = ram.data[word_to_int(address)];
        expect((word == expected))
          << "Before Failed at:" << iter << "." << word_to_int(word)
          << "!=" << word_to_int(expected) << "\n";
      }
    }
  }
}

template <typename RAMImpl, typename Apply>
auto test_ram_x(Apply apply, const std::vector<test::memory::InputType>& inputs,
                const std::vector<test::memory::OutputType>& results) -> void
{
  {
    using namespace boost::ut;
    expect((inputs.size() == results.size() >> fatal))
      << "Input and Output size must be equal.";

    RAMImpl ram;

    const size_t iteration = inputs.size();
    for (size_t iter = 0; iter < iteration; ++iter) {
      const auto& [r_in, r_load, r_address] = inputs[iter];

      //Inputs
      auto in      = int_to_word(r_in);
      auto load    = Bit(r_load);
      auto address = int_to_word(r_address);

      //Expected Results
      const auto& e_res   = results[iter];
      auto        e_after = int_to_word(e_res);

      //Function to be tested.
      [[maybe_unused]] auto before =
        apply(ram, std::span{in}, load,
              std::span{address}.template first<RAMImpl::AddressSize>());

      //Result comparision.
      auto word =
        apply(ram, FalseWord, BitFalse,
              std::span{address}.template first<RAMImpl::AddressSize>());
      bool passed = (word == e_after);
      expect((passed)) << "at:" << iter << ", address:" << word_to_int(address)
                       << "." << word_to_int(word)
                       << "!=" << word_to_int(e_after);

      //if (!passed) {
      //  for (size_t idx = 0; idx < RAMImpl::total(); ++idx) {
      //    auto addr   = int_to_word(idx);
      //    auto r_addr = addr;
      //    std::reverse(r_addr.begin(), r_addr.end());
      //    auto content =
      //      apply(ram, FalseWord, BitFalse,
      //            std::span{addr}.template first<RAMImpl::AddressSize>());
      //    std::cout << "Index: " << idx << ", Address:" << r_addr
      //              << ", Value: " << content << ", " << word_to_int(content)
      //              << "\n";
      //    return;
      //  }
      //}
    }
  }
}

auto test_address() -> void
{
  std::vector<int> address(QuarterWordSize);
  std::iota(address.begin(), address.end(), 0);

  for (int add : address) {
    auto word = int_to_word(add);
    std::cout << "f:" << word << " , r:";
    std::ranges::reverse(word);
    std::cout << word << "\n";
  }
}

//NOLINTEND
auto main() -> int
{
  namespace tm = test::memory;
  using namespace boost::ut;
  "memory"_test = [] {
    "memory_bit"_test = [] { test_memory_bit(); };
    //test_memory_bit<true>();
    "conversion"_test = [] { test_conversion(); };
    "register"_test   = [] { test_register(); };
    //test_register();
    //test_ram_8<true>();
    "ram8"_test  = []() { test_ram_8(); };
    "ram64"_test = []() {
      auto apply = []<typename... Args>(Args&&... args) {
        return ram64(args...);
      };
      test_ram_x<RAM64>(apply, tm::ram64::Inputs, tm::ram64::Results);
    };

    "ram512"_test = []() {
      auto apply = []<typename... Args>(Args&&... args) {
        return ram512(args...);
      };
      test_ram_x<RAM512>(apply, tm::ram512::Inputs, tm::ram512::Results);
    };

    "ram4k"_test = []() {
      auto apply = []<typename... Args>(Args&&... args) {
        return ram4k(args...);
      };
      test_ram_x<RAM4K>(apply, tm::ram4k::Inputs, tm::ram4k::Results);
    };

    "ram16k"_test = []() {
      auto apply = []<typename... Args>(Args&&... args) {
        return ram16k(args...);
      };
      test_ram_x<RAM16K>(apply, tm::ram16k::Inputs, tm::ram16k::Results);
    };
  };
}
