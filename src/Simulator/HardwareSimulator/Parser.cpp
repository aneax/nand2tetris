#include "HardwareSimulator/Parser.hpp"
#include "HardwareSimulator/Bit.hpp"
#include "Characters.hpp"
#include "Reader.hpp"
#include "Errors.hpp"
#include <string_view>
#include <utility>

namespace hack::defs
{

class Parser
{
    hack::MemoryInput reader_;

    static auto parse_bit_(char ch) -> Bit;

    auto parse_line_() -> Word;
    auto peek_() -> char;
    auto advance_() -> void;
    auto consume_whitespace_() -> void;

  public:
    Parser(hack::MemoryInput input) : reader_(std::move(input)) {}
    auto parse() -> std::vector<Word>;
};

auto Parser::peek_() -> char
{
  return reader_.current().data[0];
}

auto Parser::advance_() -> void
{
  reader_.bump();
}

auto Parser::consume_whitespace_() -> void
{
  while (is_white_space(peek_())) {
    reader_.bump();
  }
}

auto Parser::parse_bit_(char ch) -> Bit
{
  constexpr char Zero = '0';
  constexpr char One  = '1';

  if ((ch != One) && (ch != Zero)) {
    std::string error("Unknown character: ");
    error.push_back(ch);
    throw hack::ParseError(error);
  }

  int value = static_cast<int>(ch) - Zero;
  Bit bit(value);
  return bit;
}

auto Parser::parse_line_() -> Word
{
  Word word;
  consume_whitespace_();
  //size_t iter = WordSize;
  //char   ch   = peek_();
  //do {
  //  word[iter] = parse_bit_(ch);
  //  --iter;
  //  advance_();
  //  ch = peek_();
  //} while (is_digit(ch));
  // for (size_t it = 0; it < WordSize; ++it) {
  //   word[it] = parse_bit_(peek_());
  //   advance_();
  // }
  for (int it = WordSize - 1; it >= 0; --it) {
    word[it] = parse_bit_(peek_());
    advance_();
  }
  consume_whitespace_();
  return word;
}

auto Parser::parse() -> std::vector<Word>
{
  std::vector<Word> instructions;
  while (!reader_.is_end()) {
    instructions.push_back(parse_line_());
  }
  return instructions;
}

auto parse_from_file(std::string_view file_path) -> std::vector<Word>
{
  hack::FileReader reader(file_path);
  Parser           parser(reader);
  return parser.parse();
}

auto parse(std::string_view file_path, ParseFrom type) -> std::vector<Word>
{
  switch (type) {
    case ParseFrom::File:
      return parse_from_file(file_path);
    case ParseFrom::Memory:
      return Parser(hack::MemoryInput(file_path)).parse();
      break;
  }

  throw hack::ParseError("Unkown source");
}

}   //namespace hack::defs
