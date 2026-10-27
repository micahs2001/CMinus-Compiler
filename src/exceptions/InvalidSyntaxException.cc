/******************************************************************************
 File Name:    InvalidSyntaxException.cc
 Description:  Invalid Syntax Exception class definition and implementation for
               CMinus language
 Author:       Micah Schafer
 Date:         October 16, 2026

 *****************************************************************************/

#include "tokenizing/Token.hh"

#include <format>
#include <initializer_list>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

class InvalidSyntaxException : public std::runtime_error
{
public:
  InvalidSyntaxException (int line,
                          int column,
                          std::initializer_list<TokenType> expected,
                          std::string got)
      : std::runtime_error (format_message (line, column, expected, got)),
        line_ (line),
        column_ (column),
        expected_ (expected),
        got_ (std::move (got))
  {
  }

  int
  line () const noexcept
  {
    return line_;
  }
  int
  column () const noexcept
  {
    return column_;
  }
  const std::vector<TokenType>&
  expected () const noexcept
  {
    return expected_;
  }
  const std::string&
  got () const noexcept
  {
    return got_;
  }

private:
  static std::string
  format_message (int line,
                  int col,
                  std::initializer_list<TokenType> expected,
                  const std::string& got)
  {
    std::string expStr =
      std::format ("{{ '{:s}' }}",
                   expected | std::views::transform (tokenToString) |
                     std::views::join_with (std::string_view ("', '")) |
                     std::ranges::to<std::string> ());

    return std::format ("Compilation Error\n  Parse Error [Line {}, Col {}]\n  "
                        "  Expected {} but got \'{}\'",
                        line,
                        col,
                        expStr,
                        got);
  }

  int line_;
  int column_;
  std::vector<TokenType> expected_;
  std::string got_;
};