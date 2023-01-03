#ifndef NAND2TETRIS_SOURCE_COMPILER_ELEMENTS_HPP
#define NAND2TETRIS_SOURCE_COMPILER_ELEMENTS_HPP

#include <string_view>

namespace hack::elements
{
static constexpr char ParenOpen    = '(';
static constexpr char ParenClose   = ')';
static constexpr char BraceOpen    = '{';
static constexpr char BraceClose   = '}';
static constexpr char BracketOpen  = '[';
static constexpr char BracketClose = ']';
static constexpr char AngleOpen    = '<';
static constexpr char AngleClose   = '>';
static constexpr char Comma        = ',';
static constexpr char Semicolon    = ';';
static constexpr char Colon        = ':';
static constexpr char Dot          = '.';
static constexpr char Equals       = '=';
static constexpr char Plus         = '+';
static constexpr char Minus        = '-';
static constexpr char Slash        = '/';
static constexpr char Asterisk     = '*';
static constexpr char Ampersand    = '&';
static constexpr char Tilde        = '~';
static constexpr char Pipe         = '|';
static constexpr char DoubleQuote  = '"';
static constexpr char SingleQuote  = '\'';
static constexpr char Exclamation  = '!';

static constexpr std::string_view Class       = "class";
static constexpr std::string_view Constructor = "constructor";
static constexpr std::string_view Method      = "method";
static constexpr std::string_view Function    = "function";
static constexpr std::string_view Int         = "int";
static constexpr std::string_view Boolean     = "boolean";
static constexpr std::string_view Char        = "char";
static constexpr std::string_view Void        = "void";
static constexpr std::string_view Var         = "var";
static constexpr std::string_view Static      = "static";
static constexpr std::string_view Field       = "field";
static constexpr std::string_view Let         = "let";
static constexpr std::string_view Do          = "do";
static constexpr std::string_view If          = "if";
static constexpr std::string_view Else        = "else";
static constexpr std::string_view While       = "while";
static constexpr std::string_view Return      = "return";
static constexpr std::string_view True        = "true";
static constexpr std::string_view False       = "false";
static constexpr std::string_view Null        = "null";
static constexpr std::string_view This        = "this";

}   //namespace hack::elements
#endif
