#ifndef NAND2TETRIS_INCLUDE_READER_HPP
#define NAND2TETRIS_INCLUDE_READER_HPP

#include "Filesystem.hpp"
#include "Iterator.hpp"
#include <string>
#include <cassert>
#include <ostream>
#include <string_view>

namespace hack
{

namespace internal
{
struct StringHolder {
    const std::string buffer;

    template <typename T>
    explicit StringHolder(T&& in) : buffer(std::forward<T>(in))
    {
    }
    StringHolder()                    = delete;
    StringHolder(const StringHolder&) = delete;
    StringHolder(StringHolder&&)      = delete;

    StringHolder& operator=(const StringHolder&) = delete;   //NOLINT
    StringHolder& operator=(StringHolder&&)      = delete;   //NOLINT
    ~StringHolder()                              = default;
};

class MemoryInput
{
  public:
    using iterator  = Iterator;
    using siterator = const char*;
    MemoryInput(const iterator& begin, const char* end, const std::string& source)
        : begin_(begin.data)
        , current_(begin)
        , end_(end)
        , source_(source.begin(), source.end())
    {
    }
    MemoryInput(const char* begin, const char* end, const std::string& source)
        : MemoryInput(iterator(begin, 1, 1), end, source)
    {
    }

    MemoryInput(std::string_view str, const std::string& src = "")
        : MemoryInput(str.data(), str.data() + str.size(), src)
    {
    }

    MemoryInput(const MemoryInput&) = default;
    MemoryInput(MemoryInput&&)      = default;

    MemoryInput& operator=(const MemoryInput&) = delete;   //NOLINT
    MemoryInput& operator=(MemoryInput&&)      = delete;   //NOLINT

    ~MemoryInput() = default;

    [[nodiscard]] auto current() const noexcept -> iterator { return current_; }
    [[nodiscard]] auto begin() const noexcept -> siterator { return begin_; }
    [[nodiscard]] auto end() const noexcept -> siterator { return end_; }
    [[nodiscard]] auto line() const noexcept -> size_t { return current_.line; }
    [[nodiscard]] auto column() const noexcept -> size_t { return current_.column; }
    [[nodiscard]] auto source() const noexcept -> std::string_view { return source_; }
    [[nodiscard]] auto data() const noexcept -> std::string_view { return {begin_, end_}; }
    [[nodiscard]] auto is_end() const noexcept -> bool { return current_.data == end_; }
    [[nodiscard]] auto remaining() const noexcept -> size_t
    {
      return std::distance(current_.data, end_);
    }

    auto bump(size_t count = 1) noexcept -> bool;
    void restart(size_t line = 1, size_t column = 1);

  protected:
    const char* const begin_;
    iterator          current_;
    const char*       end_;
    std::string       source_;
};

struct StringInput : private StringHolder,
                     public MemoryInput {
    template <typename T>
    explicit StringInput(T&& in, const std::string& source)
        : StringHolder(std::forward<T>(in))
        , MemoryInput(buffer.data(), buffer.data() + buffer.size(), source)
    {
    }

    using iterator = MemoryInput::iterator;
};

auto read_file(std::string_view filename) -> std::string;
}   //namespace internal

struct ReadInput : public internal::StringInput {
    explicit ReadInput(const fs::path& path);
    using StringInput::iterator;
};

struct FileReader : public ReadInput {
    using ReadInput::ReadInput;
    using iterator = ReadInput::iterator;
};

using MemoryInput = internal::MemoryInput;
}   //namespace hack

#endif