/******************************************************************************
 File Name:    Scanner.cc
 Description:  Scanner class for CMinus language
 Author:       Micah Schafer
 Date:         September 24, 2026

 *Function descriptors in header file
 *****************************************************************************/

#include <cctype>
#include <string>

#include "lexing/Scanner.hh"
#include "tokenizing/Token.hh"

Scanner::Scanner (std::string source)
    : source_ (source)
{
}

Token
Scanner::scanNext ()
{
  skipWhitespaceAndComments ();
  if (peek () == EOF)
  {
    return Token { TokenType::EndOfFile, "", line_, column_ };
  }

  int c = peek ();
  if (isLetter (c))
  {
    return scanIdentifier ();
  }
  if (isNumber (c))
  {
    return scanNumber ();
  }

  c = advance ();
  switch (c)
  {
    case '+':
      return Token { TokenType::Plus, "+", line_, column_ };
    case '-':
      return Token { TokenType::Minus, "-", line_, column_ };
    case '*':
      return Token { TokenType::Multiply, "*", line_, column_ };
    case '/':
      return Token { TokenType::Divide, "/", line_, column_ };
    case '<':
    {
      if (peek () == '=')
      {
        advance ();
        return Token { TokenType::LessEqual, "<=", line_, column_ };
      }
      return Token { TokenType::Less, "<", line_, column_ };
    }
    case '>':
    {
      if (peek () == '=')
      {
        advance ();
        return Token { TokenType::GreaterEqual, ">=", line_, column_ };
      }
      return Token { TokenType::Greater, ">", line_, column_ };
    }
    case '=':
    {
      if (peek () == '=')
      {
        advance ();
        return Token { TokenType::Equal, "==", line_, column_ };
      }
      return Token { TokenType::Assign, "=", line_, column_ };
    }
    case '!':
    {
      if (peek () == '=')
      {
        advance ();
        return Token { TokenType::NotEqual, "!=", line_, column_ };
      }
      break;
    }
    case ';':
      return Token { TokenType::Semi, ";", line_, column_ };
    case ',':
      return Token { TokenType::Comma, ",", line_, column_ };
    case '(':
      return Token { TokenType::LParen, "(", line_, column_ };
    case ')':
      return Token { TokenType::RParen, ")", line_, column_ };
    case '[':
      return Token { TokenType::LBrack, "[", line_, column_ };
    case ']':
      return Token { TokenType::RBrack, "]", line_, column_ };
    case '{':
      return Token { TokenType::LBrace, "{", line_, column_ };
    case '}':
      return Token { TokenType::RBrace, "}", line_, column_ };
  }
  return Token {
    TokenType::Error, std::string (1, static_cast<char> (c)), line_, column_
  };
}

int
Scanner::peek () const
{
  if (cursor_ < source_.size ())
  {
    return source_.at (cursor_);
  }
  else
  {
    return EOF;
  }
}

int
Scanner::advance ()
{
  int c = peek ();
  ++cursor_;
  ++column_;
  return c;
}

int
Scanner::lookAhead ()
{
  if (cursor_ + 1 < source_.size ())
  {
    return source_.at (cursor_ + 1);
  }
  else
  {
    return EOF;
  }
}

bool
Scanner::isVerticalSpace (int c)
{
  return (c > 9 && c < 14);
}

bool
Scanner::isLetter (int c)
{
  return (c > 64 && c < 91) || (c > 96 && c < 123);
}

bool
Scanner::isNumber (int c)
{
  return (c > 47 && c < 58);
}

void
Scanner::skipWhitespaceAndComments ()
{
  while (true)
  {
    // Skip whitespace
    int c = peek ();
    if (c != EOF && std::isspace (static_cast<char> (c)))
    {
      if (isVerticalSpace (c))
      {
        ++line_;
        column_ = 0;
      }
      advance ();
    }
    // Skip comments starting with /* and ending with */
    else if (c == '/' && lookAhead () == '*')
    {
      skipCommentBlock ();
    }
    else
    {
      return;
    }
  }
}

void
Scanner::skipCommentBlock ()
{
  advance ();
  advance ();
  while (peek () != EOF && !(peek () == '*' && lookAhead () == '/'))
  {
    advance ();
  }
  if (peek () != EOF)
  {
    advance ();
    advance ();
  }
}

Token
Scanner::scanIdentifier ()
{
  std::string s;
  size_t curCol = column_;
  while (peek () != EOF && isLetter (peek ()))
  {
    s += static_cast<char> (advance ());
  }

  std::string keywords[6] = { "if", "else", "int", "void", "return", "while" };
  for (size_t i = 0; i < 6; ++i)
  {
    if (s == keywords[i])
    {
      return Token { TokenType (i), s, line_, curCol };
    }
  }
  return Token { TokenType::Id, s, line_, curCol };
}

Token
Scanner::scanNumber ()
{
  std::string s;
  size_t curCol = column_;
  while (peek () != EOF && isNumber (peek ()))
  {
    s += static_cast<char> (advance ());
  }
  return Token { TokenType::Num, s, line_, curCol };
}

void
Scanner::putBack ()
{
  --cursor_;
  --column_;
}