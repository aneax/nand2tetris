
#include "HardwareSimulator/Memory.hpp"
#include "HardwareSimulator/Bit.hpp"

namespace hack::defs
{

//Data Flip-flop
auto dff(DFF& dff, const Bit& in) -> Bit
{
  auto temp = dff;
  dff       = in;
  return temp;
}

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
  -> Bit
{
  auto fout = mux(bit_register, in, load);
  auto feed = dff(static_cast<DFF&>(bit_register), fout);
  return feed;
}

auto ram8(RAM8& ram, SpanWord in, Bit load, bit_span<RAM8AddressSize> address)
  -> bit_array<WordSize>
{
  auto r_dmux = dmux8way(load, address);

  std::array<bit_array<WordSize>, RAM8::Size> imm_out;
  for (size_t reg_id = 0; reg_id < RAM8::Size; ++reg_id) {
    imm_out[reg_id] = register_n({ram.data[reg_id]}, in, r_dmux[reg_id]);
  }
  auto out = mux8way(imm_out, address);
  return out;
}

template <typename Apply, typename RAMImpl, size_t SourceSize>
inline auto ram_x(Apply apply, RAMImpl& ram, SpanWord in, Bit load,
                  bit_span<SourceSize> address) -> bit_array<WordSize>
{
  std::array<bit_array<WordSize>, RAM64::Size> i_out;

  auto dmux_address = std::span{address}.template last<RAM8AddressSize>();
  auto r_dmux       = dmux8way(load, dmux_address);
  for (size_t reg_id = 0; reg_id < RAMImpl::Size; ++reg_id) {
    i_out[reg_id] = apply(ram.data[reg_id], r_dmux[reg_id]);
  }
  auto out = mux8way(i_out, dmux_address);
  return out;
}

//auto ram64(RAM64& ram, WordBitSpan in, Bit load, bit_span<RAM64AddressSize>
//address)
//  -> bit_array<Word>
//{
//  std::array<bit_array<Word>, RAM64::Size> i_out;

//auto dmux_address = std::span{address}.last<RAM8AddressSize>();
//auto r_dmux       = dmux8way(load, dmux_address);
//for (size_t reg_id = 0; reg_id < RAM64::Size; ++reg_id) {
//  i_out[reg_id] =
//    ram8(ram.data[reg_id], in, r_dmux[reg_id],
//    std::span{address}.first<RAM8AddressSize>());
//}
//auto out = mux8way(i_out, dmux_address);
//return out;
//}

auto ram64(RAM64& ram, SpanWord in, Bit load,
           bit_span<RAM64AddressSize> address) -> bit_array<WordSize>
{
  auto apply = [&](RAM64::element_type& data, const Bit load) {
    return ram8(data, in, load, std::span{address}.first<RAM8AddressSize>());
  };
  return ram_x(apply, ram, in, load, address);
}

auto ram512(RAM512& ram, SpanWord in, Bit load,
            bit_span<RAM512AddressSize> address) -> bit_array<WordSize>
{
  auto apply = [&](RAM512::element_type& data, const Bit load) {
    return ram64(data, in, load, std::span{address}.first<RAM64AddressSize>());
  };
  return ram_x(apply, ram, in, load, address);
}

auto ram4k(RAM4K& ram, SpanWord in, Bit load,
           bit_span<RAM4KAddressSize> address) -> bit_array<WordSize>
{
  auto apply = [&](RAM4K::element_type& data, const Bit load) {
    return ram512(data, in, load,
                  std::span{address}.first<RAM512AddressSize>());
  };
  return ram_x(apply, ram, in, load, address);
}

auto ram16k(RAM16K& ram, SpanWord in, Bit load,
            bit_span<RAM16KAddressSize> address) -> bit_array<WordSize>
{
  std::array<bit_array<WordSize>, RAM16K::Size> i_out;
  auto dmux_address = std::span{address}.template last<2>();
  auto r_dmux       = dmux4way(load, dmux_address);
  for (size_t reg_id = 0; reg_id < RAM16K::Size; ++reg_id) {
    i_out[reg_id] = ram4k(ram.data[reg_id], in, r_dmux[reg_id],
                          std::span{address}.first<RAM4KAddressSize>());
  }
  auto out = mux4way(i_out, dmux_address);
  return out;
}

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
            Bit load) -> Word
{
  auto apply = [&](ROM32K::element_type& data, Bit load) {
    return ram4k(data, in, load, std::span{address}.first<RAM4KAddressSize>());
  };
  return ram_x(apply, rom, in, load, address);
}
}   //namespace hack::defs