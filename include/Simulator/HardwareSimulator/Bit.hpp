#ifndef NAND2TETRIS_INCLUDE_SIMULATOR_HARDWARE_BIT_HPP
#define NAND2TETRIS_INCLUDE_SIMULATOR_HARDWARE_BIT_HPP

#include <array>
#include <span>
#include <cstdint>
#include <ostream>

namespace hack::defs
{
static constexpr size_t WordSize        = 16;
static constexpr size_t HalfWordSize    = WordSize / 2;
static constexpr size_t QuarterWordSize = WordSize / 4;

class Bit
{
  private:
    using type = uint64_t;

    type set_ : 1 = false;

  public:
    constexpr Bit(type value = 0) : set_(value) {}
    [[nodiscard]] auto value() const -> type { return set_; }

    friend constexpr auto operator&(const Bit& lhs, const Bit& rhs) -> Bit
    {
      return (static_cast<type>(lhs.set_) & static_cast<type>(rhs.set_));
    }

    friend constexpr auto operator|(const Bit& lhs, const Bit& rhs) -> Bit
    {
      return (static_cast<type>(lhs.set_) | static_cast<type>(rhs.set_));
    }

    friend constexpr auto operator|=(Bit& lhs, const Bit& rhs) -> Bit&
    {
      lhs = lhs | rhs;
      return lhs;
    }

    friend constexpr auto operator~(const Bit& bit) -> Bit { return ~static_cast<type>(bit.set_); }

    friend constexpr auto operator==(const Bit& lhs, const Bit& rhs) -> bool
    {
      return lhs.set_ == rhs.set_;
    }

    operator bool() const { return static_cast<bool>(set_); }

    friend auto operator<<(std::ostream& out, const Bit& bit) -> std::ostream&
    {
      out << static_cast<type>(bit.set_);
      return out;
    }
};

//using Bit = std::bitset<1>;
using Sel                     = Bit;
static constexpr Bit BitTrue  = Bit{1};
static constexpr Bit BitFalse = Bit{0};

template <size_t Size>
using bit_array = std::array<Bit, Size>;

template <size_t Size>
using bit16_array = std::array<bit_array<WordSize>, Size>;

template <size_t Size>
using bit_span = std::span<const Bit, Size>;

template <size_t Size>
using mutable_bit_span = std::span<Bit, Size>;

template <size_t Size>
using bit16_span = std::span<bit_span<WordSize>, Size>;

using Sel2 = bit_array<2>;
using Sel3 = bit_array<3>;

using Word     = bit_array<WordSize>;
using SpanWord = bit_span<WordSize>;

template <typename T, typename... Ts>
using is_any_one_of = std::disjunction<std::is_same<T, Ts>...>;

template <typename T, typename... Ts>
constexpr bool is_any_one_of_v = is_any_one_of<T, Ts...>::value;   //NOLINT

template <typename T>
concept BitArrayLike = requires(T container) {
                         typename std::decay_t<T>::value_type;
                         std::is_same_v<typename std::decay_t<T>::value_type, Bit>;
                         // clang-format off
                         {
                           container[0UL]
                         } -> std::convertible_to<typename std::decay_t<T>::value_type>;
                         // clang-format on
                       };

template <typename T>
concept BitOrBitArray = std::is_same_v<T, Bit> || BitArrayLike<T>;

template <size_t Size, bool Value = false>
constexpr auto make_bit_array()
{
  std::array<Bit, Size> array;
  for (auto& elm : array) {
    elm = Bit(Value);
  }
  return array;
}

static constexpr Word FalseWord = make_bit_array<WordSize, false>();
static constexpr Word TrueWord  = make_bit_array<WordSize, true>();

template <size_t Size>
auto operator<<(std::ostream& out, bit_span<Size> bits) -> std::ostream&
{
  for (const auto& bit : bits) {
    out << bit;
  }
  return out;
}

template <size_t Size>
auto operator<<(std::ostream& out, const bit_array<Size>& bits) -> std::ostream&
{
  return out << std::span{bits};
}

}   //namespace hack::defs
#endif
