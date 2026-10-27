
#pragma once

#include <string>
#include <string_view>

enum class TokenType
{
  // Keywords
  If,
  Else,
  Int,
  Void,
  Return,
  While,
  // Operators
  Plus,
  Minus,
  Multiply,
  Divide,
  Less,
  LessEqual,
  Greater,
  GreaterEqual,
  Equal,
  NotEqual,
  Assign,
  // Delimiters/Punctuators
  Semi,
  Comma,
  LParen,
  RParen,
  // Bracks are [ and ]
  LBrack,
  RBrack,
  // Braces are { and }
  LBrace,
  RBrace,
  // Identifiers and Literals
  Id,
  Num,
  // Special
  EndOfFile,
  Error
};

struct Token
{
  TokenType type { TokenType::Error };
  std::string lexeme;
  size_t line { 1 };
  size_t column { 1 };
};

std::string_view
tokenToString (TokenType type);
