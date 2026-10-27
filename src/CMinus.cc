/******************************************************************************
 File Name:    CMinus.cc
 Description:  Compiler driver for CMinus language
 Author:       Micah Schafer
 Date:         October 16, 2026

 *****************************************************************************/

/*****************************************************************************/
// Includes
#include "exceptions/Exceptions.hh"
#include "lexing/Scanner.hh"
#include "parsing/Parser.hh"

#include <fstream>
#include <iostream>
#include <print>
#include <string>

/*****************************************************************************/
// Forward Declarations

std::string
fileToStr (const std::string& filename);

std::string
readFromIn ();

std::string
tokToStr (TokenType type);

/*****************************************************************************/
// Main

int
main (int argc, char* argv[])
{
  // Select input method based on if there was a file inputted
  std::string input;
  if (argc > 1)
  {
    input = fileToStr (argv[1]);
    std::println ("File inputted");
  }
  else
  {
    input = readFromIn ();
  }

  // Input is string of file or input stream from terminal
  Scanner scnr (input);
  Parser parser (std::move (scnr));

  try
  {
    parser.parse ();
  }
  catch (InvalidSyntaxException& e)
  {
    std::println ("{}", e.what ());
  }
}

/*****************************************************************************/
// Function Implementations

// Convert a file to a string for input
std::string
fileToStr (const std::string& filename)
{
  std::ifstream file (filename);

  return std::string ((std::istreambuf_iterator<char> (file)),
                      std::istreambuf_iterator<char> ());
}

// Read input from terminal
// Ctrl+D to end input stream
std::string
readFromIn ()
{
  std::string input;
  std::string line;
  while (std::getline (std::cin, line))
  {
    input += line + "\n";
  }
  return input;
}

// Convert enumerated token type to its string version
std::string
tokToStr (TokenType type)
{
  std::string toks[29] = { // Keywords
                           "If",
                           "Else",
                           "Int",
                           "Void",
                           "Return",
                           "While",
                           // Operators
                           "Plus",
                           "Minus",
                           "Multiply",
                           "Divide",
                           "Less",
                           "LessEqual",
                           "Greater",
                           "GreaterEqual",
                           "Equal",
                           "NotEqual",
                           "Assign",
                           // Delimiters/Punctuators
                           "Semi",
                           "Comma",
                           "LParen",
                           "RParen",
                           // Bracks are [ and ]
                           "LBrack",
                           "RBrack",
                           // Braces are { and }
                           "LBrace",
                           "RBrace",
                           // Identifiers and Literals
                           "Id",
                           "Num",
                           // Special
                           "EndOfFile",
                           "Error"
  };
  return toks[static_cast<int> (type)];
}