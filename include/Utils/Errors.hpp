
#ifndef MINI_CUSTOM_INCLUDE_ERRORS_HPP
#define MINI_CUSTOM_INCLUDE_ERRORS_HPP

#include <stdexcept>
#include <vector>
#include <string>
namespace hack
{

class Token;

enum class ErrorSeverity {
  Fatal,
  NonFatal
};

struct Error : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct ParseError : public Error {
    using Error::Error;
};

struct RuntimeError : public Error {
    using Error::Error;
};

struct SemanticError : public Error {
    using Error::Error;
};

class ErrorReporter
{
  public:
    void log(std::string&& error) { errors_.push_back(std::move(error)); }
    void check_errors() const;

  private:
    [[nodiscard]] auto       has_errors_() const noexcept -> bool { return !errors_.empty(); }
    void                     dump_() const;
    std::vector<std::string> errors_;
};

}   //namespace hack

#endif