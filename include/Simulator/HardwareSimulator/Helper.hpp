#include "Bit.hpp"
#include "HardwareSimulator/Memory.hpp"
#include <cstdint>
#include <vector>

namespace hack::defs
{
auto int_to_word(int in) -> Word;
auto word_to_int(const Word& in) -> int;
auto address_to_int(std::span<const Bit, MemoryAddressSize> in) -> int;
auto load_rom(ROM32K& rom, std::span<const int> instructions) -> void;
auto load_rom(ROM32K& rom, const std::vector<Word>& instructions) -> void;
auto string_to_word(std::span<std::string_view> str_insts) -> std::vector<Word>;
}   //namespace hack::defs
