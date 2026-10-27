/******************************************************************************
 File Name:    Scanner.hh
 Description:  Scanner class for CMinus language
 Author:       Micah Schafer
 Date:         October 16, 2026

 *****************************************************************************/

/*****************************************************************************/
// Includes

#pragma once

#include "tokenizing/Token.hh"
#include <string>

/*****************************************************************************/
// Class definition

class Scanner
{
public:
  explicit Scanner (std::string source);

  // Return the next token
  Token
  scanNext ();

private:
  // Return char at `cursor_` as an `int`, or EOF if at end.
  int
  peek () const;

  // Like `peek`, but advance `cursor_`.
  //   Update `line_` and `column_` as necessary.
  int
  advance ();

  // Look ahead one character without advancing cursor_
  int
  lookAhead ();

  // Returns true if the character is a vertical whitespace character
  bool
  isVerticalSpace (int c);

  // Returns true if the character is a uppercase or lowercase letter
  bool
  isLetter (int c);

  // Returns true if the character is a digit 0-9
  bool
  isNumber (int c);

  // Advances cursor to next non-whitespace char
  // Updates line and column as necessary
  void
  skipWhitespaceAndComments ();

  // Skips comment block until */ is found
  // Updates line and column as necessary
  void
  skipCommentBlock ();

  // Returns the token containing the current char as well as any sequential
  // letters
  // Updates the line and column as necessary
  Token
  scanIdentifier ();

  // Returns the token containing the current char as well as any sequential
  // digits
  // Updates the line and column as necessary
  Token
  scanNumber ();

  // Decrements the cursor
  void
  putBack ();

  /*****************************************************************************/
  // Class fields

private:
  std::string source_;
  size_t cursor_ { 0 };
  size_t line_ { 1 };
  size_t column_ { 1 };
};
