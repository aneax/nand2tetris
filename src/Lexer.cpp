#include "Lexer.hpp"
#include "Errors.hpp"
#include "Reader.hpp"

#include <map>
namespace hack
{
static constexpr char Slash = '/';
static constexpr char Start = '*';
static constexpr char White = ' ';
static constexpr char Tab   = '\t';
static constexpr char Eol   = '\0';

static auto is_white_space(char ch) -> bool
{
  return (' ' == ch) || ('\n' == ch) || ('\r' == ch) || (Tab == ch) || ('\b' == ch) || ('\v' == ch)
      || ('\f' == ch);
}

static auto is_alpha(char ch) -> bool
{
  return (ch == '_') || (ch == '.') || (ch == '$') || ((ch >= 'a') && (ch <= 'z'))
      || ((ch >= 'A') && (ch <= 'Z'));
}

static auto is_digit(char ch) -> bool
{
  return (ch >= '0') && (ch <= '9');
}

static auto is_operator(char ch) -> bool
{
  return ('+' == ch) || ('-' == ch) || ('=' == ch) || ('!' == ch) || ('(' == ch) || (')' == ch)
      || ('@' == ch) || ('&' == ch) || ('|' == ch);
}

static auto is_alpha_numeric(char ch) -> bool
{
  return is_digit(ch) || is_alpha(ch);
}

class Lexer
{
  public:
    using iterator = FileReader::iterator;
    using T        = TokenType;
    Lexer(FileReader& reader) : reader_(reader), start_(reader.current())
    {
      keywords_.insert({"JGT", T::JGT});
      keywords_.insert({"JGE", T::JGE});
      keywords_.insert({"JGT", T::JLT});
      keywords_.insert({"JLE", T::JLE});
      keywords_.insert({"JEQ", T::JEQ});
      keywords_.insert({"JNE", T::JNE});
      keywords_.insert({"JMP", T::JMP});
      keywords_.insert({"M", T::M});
      keywords_.insert({"D", T::D});
      keywords_.insert({"DM", T::DM});
      keywords_.insert({"A", T::A});
      keywords_.insert({"AM", T::AM});
      keywords_.insert({"AD", T::AD});
      keywords_.insert({"ADM", T::ADM});
    }

    auto advance(size_t num = 1) -> void;
    auto peek(size_t n = 0) -> char;
    void scan();
    void push_token(TokenType token);
    void push_token(TokenType token, iterator begin, iterator end);
    auto is_end() -> bool;
    void start();
    auto substr(size_t n) -> std::string_view;
    auto match(std::string_view expected) -> bool;
    void check_errors() const { reporter_.check_errors(); }

    [[nodiscard]] auto tokens() const noexcept -> const std::vector<Token>&;
    [[nodiscard]] auto until(char ch) -> std::string_view;
    [[nodiscard]] auto current() -> iterator { return reader_.current(); }
    [[nodiscard]] auto current() const noexcept -> iterator { return reader_.current(); }
    [[nodiscard]] auto remaining() const noexcept -> size_t;

  private:
    auto match_string_() -> void;
    auto match_number_() -> void;
    auto match_identifier_() -> void;
    auto error_(std::string_view message) -> void;

    FileReader::iterator                  start_;
    FileReader&                           reader_;
    std::vector<Token>                    tokens_;
    std::map<std::string_view, TokenType> keywords_;
    ErrorReporter                         reporter_;
};

auto operator<<(std::ostream& out, const TokenType& token) -> std::ostream&
{
#define TOK(ID)       \
  case TokenType::ID: \
    {                 \
      out << #ID;     \
      break;          \
    }
  switch (token) {
    TOK(Label);
    TOK(Symbol);
    TOK(Equal);
    TOK(Add);
    TOK(Sub);
    TOK(And);
    TOK(Or);
    TOK(Not);
    TOK(At);
    TOK(Identifier);
    TOK(Number);
    TOK(JGT);
    TOK(JLT);
    TOK(JGE);
    TOK(JLE);
    TOK(JEQ);
    TOK(JNE);
    TOK(JMP);
    TOK(M);
    TOK(A);
    TOK(AD);
    TOK(AM);
    TOK(ADM);
    TOK(D);
    TOK(DM);
    TOK(DNULL);
    TOK(LParantheses);
    TOK(RParantheses);
    case TokenType::FEOF:
      out << "EOF";
      break;
  }
  return out;
}

auto operator<<(std::ostream& out, const Token& token) -> std::ostream&
{
  auto begin = token.begin();
  auto end   = token.end();

  out << token.type() << "<" << begin.line << ":" << begin.column << ", " << end.line << ":"
      << end.column << "> " << token.string_view();
  return out;
}

auto Lexer::advance(size_t num) -> void
{
  reader_.bump(num);
}

auto Lexer::peek(size_t n) -> char
{
  auto remaining = std::distance(reader_.current().data, reader_.end());
  if (n > remaining) {
    return '\0';
  }
  return current().data[n];
}

void Lexer::start()
{
  while (!is_end()) {
    start_ = current();
    scan();
  }
  tokens_.emplace_back(TokenType::FEOF, current(), current());
}

auto Lexer::is_end() -> bool
{
  return current().data == reader_.end();
}

void Lexer::push_token(TokenType token)
{
  push_token(token, start_, current());
}

void Lexer::push_token(TokenType token, iterator begin, iterator end)
{
  tokens_.emplace_back(token, begin, end);
}

auto Lexer::match_number_() -> void
{
  //continue bumping
  while (is_digit(peek())) {
    advance();
  }

  push_token(TokenType::Number);
}

auto Lexer::match_identifier_() -> void
{
  while (is_alpha_numeric(peek())) {
    advance();
  }
  auto lexmee = std::string_view{start_.data, current().data};
  if (auto it = keywords_.find(lexmee); it != keywords_.end()) {
    push_token(it->second);
  } else {
    push_token(TokenType::Identifier);
  }
}

auto Lexer::remaining() const noexcept -> size_t
{
  return (reader_.end() - current().data);
}

auto Lexer::substr(size_t n) -> std::string_view
{
  if (n < remaining()) {
    return {current().data, current().data + n};
  }

  return {current().data, reader_.end()};
}

auto Lexer::match(std::string_view expected) -> bool
{
  if (is_end()) {
    return false;
  }

  auto actual = substr(expected.size());
  return std::equal(expected.begin(), expected.end(), actual.begin());
}

auto Lexer::until(char ch) -> std::string_view
{
  auto begin = current();
  auto str   = std::string_view(&ch, 1);
  while (!match(str) && !is_end()) {
    advance();
  }
  auto end = current();
  return {begin.data, end.data};
}

auto Lexer::error_(std::string_view message) -> void
{
  auto str   = until('\n');
  auto error = std::string(message);
  error.append(" at ");
  error.append(reader_.source());
  error.append(":");
  error.append(to_string(start_));
  error.append(str);
  error.append("\n");
  reporter_.log(std::move(error));
}

void Lexer::scan()
{
  while (is_white_space(peek())) {
    advance();
    start_ = current();
  }

#define PUSH(id, token)  \
  case id:               \
    {                    \
      advance();         \
      push_token(token); \
      break;             \
    }
#define PUSH_CUSTOM(id, body) \
  case id:                    \
    {                         \
      body;                   \
      break;                  \
    }
  char ch = peek();
  switch (ch) {
    PUSH('(', T::LParantheses);
    PUSH(')', T::RParantheses);
    PUSH('+', T::Add);
    PUSH('-', T::Sub);
    PUSH('@', T::At);
    PUSH('!', T::Not);
    PUSH('&', T::And);
    PUSH('|', T::Or);
    PUSH('=', T::Equal);
    PUSH(';', T::SemiColon);
    PUSH_CUSTOM('\000', {})
    PUSH_CUSTOM(' ', {})
    PUSH_CUSTOM('\r', {})
    PUSH_CUSTOM('\t', {})
    PUSH_CUSTOM('\n', {});
    PUSH_CUSTOM('/', { (void)until('\n'); });
    default:
      {
        if (is_digit(ch)) {
          match_number_();
        } else if (is_alpha(ch)) {
          match_identifier_();
        } else {
          error_("Unexpected charater");
        }
        break;
      }
  }

#undef PUSH_CUSTOM
#undef PUSH
}

[[nodiscard]] auto Lexer::tokens() const noexcept -> const std::vector<Token>&
{
  return tokens_;
}

auto get_tokens(FileReader& reader) -> std::vector<Token>
{
  Lexer lexer(reader);
  lexer.start();
  lexer.check_errors();
  return lexer.tokens();
}

}   //namespace hack