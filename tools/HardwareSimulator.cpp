#include "Filesystem.hpp"
#include "Errors.hpp"
#include "HardwareSimulator/Bit.hpp"
#include "HardwareSimulator/Computer.hpp"
#include "HardwareSimulator/Memory.hpp"
#include "HardwareSimulator/Parser.hpp"
#include "HardwareSimulator/Helper.hpp"

#include <iostream>

using namespace hack::defs;

auto main(int argc, const char** argv) -> int
{
  fs::path src;
  if (argc != 2) {
    src = "/home/acharyanish7/MEGAsync/nand2tetris/tests/Assembler/inputs/"
          "expected/add/Add.hack";
  } else {
    src = (argv[1]);
  }
  {
    src = fs::canonical(src);
    std::cout << "Input: " << src << "\n";
    auto                 inputs = hack::defs::parse(src.string());
    RAM8                 ram_8;
    RAM64                ram_64;
    RAM512               ram_512;
    RAM4K                ram_4k;
    hack::defs::Computer comp;
    for (size_t it = 0; it < inputs.size(); ++it) {
      auto addr = hack::defs::int_to_word(static_cast<int>(it));
      auto word = rom32k(comp.rom, inputs[it],
                         std::span{addr}.first<MemoryAddressSize>(), BitTrue);
    }

    using namespace hack::defs;
    comp.cpu.pc = FalseWord;
    for (size_t it = 0; it < inputs.size(); ++it) {
      computer(comp, BitFalse);
    }

    for (size_t it = 0; it < inputs.size(); ++it) {
      auto addr = hack::defs::int_to_word(static_cast<int>(it));
      auto word = rom32k(comp.rom, FalseWord,
                         std::span{addr}.first<MemoryAddressSize>(), BitFalse);
      std::cout << "Word:" << word << ", int:" << word_to_int(word) << "\n";
    }

    std::cout << "Result:\n";
    auto result = ram16k(comp.memory.ram, FalseWord, BitFalse,
                         std::span{FalseWord}.first<RAM16KAddressSize>());
    std::cout << "Word:" << result << ", int:" << word_to_int(result) << "\n";
  }
  {
    src = "/home/acharyanish7/MEGAsync/nand2tetris/tests/Assembler/inputs/"
          "expected/max/Max.hack";
    auto w_inputs = hack::defs::parse(src.string());

    //NOLINTBEGIN
    std::vector<std::tuple<int, int, int>> inputs = {
      {12345, 23456, 23456},
      {23456, 12345, 23456},
      {    2,     3,     3},
      {    3,     2,     3}
    };
    //NOLINTEND

    Computer comp;
    load_rom(comp.rom, w_inputs);

    auto set_ram = [&comp](int value, int address) {
      auto                  word = int_to_word(value);
      auto                  addr = int_to_word(address);
      [[maybe_unused]] auto result =
        ram16k(comp.memory.ram, word, BitTrue,
               std::span{addr}.first<RAM16KAddressSize>());
    };

    auto res_addr = int_to_word(2);
    for (const auto& [in_a, in_b, expected] : inputs) {
      set_ram(in_a, 0);
      set_ram(in_b, 1);
      for ([[maybe_unused]] const auto& it : w_inputs) {
        computer(comp, BitFalse);
      }
      auto result  = ram16k(comp.memory.ram, FalseWord, BitFalse,
                            std::span{res_addr}.first<RAM16KAddressSize>());
      auto iresult = word_to_int(result);
      std::cout << "max(" << in_a << ", " << in_b << ") = actual:" << iresult
                << ", expected:" << expected << std::endl;   //NOLINT
    }
  }
  return 0;
}
