#include "VMCommand.hpp"
#include "VMParser.hpp"
#include "VMData.hpp"
#include <Errors.hpp>
#include <Iterator.hpp>
#include <Reader.hpp>
#include <Characters.hpp>
#include <cstdlib>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>

namespace hack::vasm
{
using PredefinedVMTokenMap = std::unordered_map<std::string_view, VMCommand::Command>;
static auto populate_keywords() -> PredefinedVMTokenMap
{
  PredefinedVMTokenMap keywords;
  auto insert = [&](std::string_view src, VMCommand::Command cmd) { keywords.insert({src, cmd}); };
  using T     = VMCommand::Command;
  insert("push", T::Push);
  insert("pop", T::Pop);
  insert("label", T::Label);
  insert("goto", T::Goto);
  insert("if-goto", T::If);
  insert("function", T::Function);
  insert("call", T::Call);
  insert("add", T::Arithmetic);
  insert("sub", T::Arithmetic);
  insert("neg", T::Arithmetic);
  insert("eq", T::Logical);
  insert("gt", T::Logical);
  insert("lt", T::Logical);
  insert("and", T::Arithmetic);
  insert("or", T::Arithmetic);
  insert("not", T::Arithmetic);
  return keywords;
}

class Parser
{
  public:
    using CMD      = VMCommand::Command;
    using iterator = FileReader::iterator;
    Parser(const fs::path& src) : reader_(src), keywords_(populate_keywords()) {}
    auto parse() -> VMCommandVector;

    auto peek(size_t n = 0) -> char;
    auto is_end() -> bool { return current().data == reader_.end(); }
    auto match(char expected) -> bool;
    void advance(size_t num = 1);
    void run();
    auto scan() -> std::string_view;
    void consume_whitespace();

    [[nodiscard]] auto current() -> iterator { return reader_.current(); }

    template <typename... CHs>
    requires std::conjunction_v<std::is_same<CHs, char>...>
    [[nodiscard]] auto until(CHs... ch) -> std::string_view;

  private:
    auto find_type_(std::string_view token) -> CMD
    {
      auto it = keywords_.find(token);
      if (it == keywords_.end()) {
        throw ParseError("Unexpected token at " + std::string(reader_.source()) + ":"
                         + to_string(current()));
      }

      return it->second;
    }

    auto parse_line_(CMD type, std::string_view token) -> VMCommand;
    auto mem_access_(CMD type) -> VMCommand;
    auto branch_(CMD type) -> VMCommand;
    auto function_(CMD type) -> VMCommand;

    FileReader           reader_;
    PredefinedVMTokenMap keywords_;
};

auto Parser::advance(size_t num) -> void
{
  reader_.bump(num);
}

auto Parser::peek(size_t n) -> char
{
  auto remaining = std::distance(reader_.current().data, reader_.end());
  if (n > remaining) {
    return '\0';
  }
  return current().data[n];
}

auto Parser::match(char expected) -> bool
{
  if (is_end()) {
    return false;
  }
  return expected == peek();
}

template <typename... CHs>
requires std::conjunction_v<std::is_same<CHs, char>...>
[[nodiscard]] auto Parser::until(CHs... ch) -> std::string_view
{
  auto begin = current();
  while (!(match(ch) || ...) && !is_end()) {
    advance();
  }
  auto end = current();
  return {begin.data, end.data};
}

void Parser::consume_whitespace()
{
  while (is_white_space(peek()) || peek() == '/') {
    advance();
    if (peek() == '/') {
      (void)until('\n');
    }
  }
}

auto Parser::scan() -> std::string_view
{
  consume_whitespace();
  auto token = until('\n', ' ', '\r');
  consume_whitespace();
  return token;
}

auto Parser::mem_access_(CMD type) -> VMCommand
{
  auto segment = scan();
  auto sindex  = std::string(scan());
  auto index   = std::stol(sindex, nullptr, 10);
  return {type, segment, index};
}

auto Parser::branch_(CMD type) -> VMCommand
{
  auto label = scan();
  return {type, label};
}

auto Parser::function_(CMD type) -> VMCommand
{
  auto name  = scan();
  auto svars = std::string(scan());
  auto nvars = std::stol(svars, nullptr, 10);
  return {type, name, nvars};
}

auto Parser::parse_line_(CMD type, std::string_view token) -> VMCommand
{
  switch (type) {
    case CMD::Push:
    case CMD::Pop:
      return mem_access_(type);
    case CMD::Label:
    case CMD::Goto:
    case CMD::If:
      return branch_(type);
    case CMD::Function:
    case CMD::Call:
      return function_(type);
    case CMD::Return:
    case CMD::Arithmetic:
    case CMD::Logical:
      return {type, token};
  }
  throw RuntimeError("Unreachable");
}

auto Parser::parse() -> VMCommandVector
{
  VMCommandVector commands;
  while (!is_end()) {
    auto      token   = scan();
    CMD       type    = find_type_(token);
    VMCommand command = parse_line_(type, token);
    commands.push_back(std::move(command));
  }
  return commands;
}

auto parse_bytecode(const fs::path& src) -> VMData
{
  Parser parser(src);
  return {src, parser.parse()};
}
}   //namespace hack::vasm