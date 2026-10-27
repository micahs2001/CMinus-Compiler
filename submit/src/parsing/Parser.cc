/******************************************************************************
 File Name:    Parser.cc
 Description:  Parser class implementation for CMinus language
 Author:       Micah Schafer
 Date:         October 16, 2026

 Detailed product rules in Parser.hh
 *****************************************************************************/

/*****************************************************************************/
// Includes
#include <algorithm>
#include <print>

#include "exceptions/Exceptions.hh"
#include "parsing/Parser.hh"
#include "tokenizing/Token.hh"

/*****************************************************************************/
// Parser Class Implementation

Parser::Parser (Scanner scanner)
    : scanner_ (scanner)
{
}

void
Parser::parse ()
{
  advance ();
  parseProgram ();

  std::println ("\n\nValid!");
  return;
}

Token
Parser::advance ()
{
  Token save = current_;
  current_ = scanner_.scanNext ();
  if (check (TokenType::Error))
    throw InvalidTokenException ("Invalid Token");
  return save;
}

Token
Parser::match (TokenType expected)
{
  if (!check (expected))
  {
    throw InvalidSyntaxException (
      current_.line, current_.column, { expected }, current_.lexeme);
    return Token ();
  }
  return advance ();
}

bool
Parser::check (TokenType type) const
{
  return (current_.type == type);
}

bool
Parser::findToken (std::initializer_list<TokenType> types) const
{
  return std::ranges::find (types, current_.type) != types.end ();
}

template<std::invocable Fun>
void
Parser::repeatUntil (Fun rule, std::initializer_list<TokenType> terminators)
{
  while (!findToken (terminators) && current_.type != TokenType::EndOfFile)
    rule ();
}

template<std::invocable Fun>
void
Parser::repeatWhile (Fun rule, std::initializer_list<TokenType> first)
{
  while (findToken (first))
    rule ();
}

void
Parser::parseProgram ()
{
  repeatUntil ([this] { return parseDecl (); }, { TokenType::EndOfFile });
}

void
Parser::parseDecl ()
{
  parseTypeSpec ();
  match (TokenType::Id);

  switch (current_.type)
  {
    case TokenType::Semi:
      match (TokenType::Semi);
      break;
    case TokenType::LBrack:
      match (TokenType::LBrack);
      match (TokenType::Num);
      match (TokenType::RBrack);
      match (TokenType::Semi);
      break;
    case TokenType::LParen:
      match (TokenType::LParen);
      parseFullParams ();
      match (TokenType::RParen);
      parseCompoundStmt ();
      break;
    default:
      throw InvalidSyntaxException (
        current_.line,
        current_.column,
        { TokenType::Semi, TokenType::LBrack, TokenType::LParen },
        current_.lexeme);
  }
}

void
Parser::parseVarDeclaration ()
{
  parseTypeSpec ();
  match (TokenType::Id);

  switch (current_.type)
  {
    case TokenType::Semi:
      match (TokenType::Semi);
      break;
    case TokenType::LBrack:
      match (TokenType::LBrack);
      match (TokenType::Num);
      match (TokenType::RBrack);
      match (TokenType::Semi);
      break;
    default:
      throw InvalidSyntaxException (current_.line,
                                    current_.column,
                                    { TokenType::Semi, TokenType::LBrack },
                                    current_.lexeme);
  }
}

void
Parser::parseTypeSpec ()
{
  switch (current_.type)
  {
    case TokenType::Int:
      match (TokenType::Int);
      break;
    case TokenType::Void:
      match (TokenType::Void);
      break;
    default:
      throw InvalidSyntaxException (current_.line,
                                    current_.column,
                                    { TokenType::Int, TokenType::Void },
                                    current_.lexeme);
  }
}

void
Parser::parseFullParams ()
{
  if (check (TokenType::Void))
  {
    match (TokenType::Void);
    // If "void" is at the end of params
    if (check (TokenType::RParen))
      return;
    ParseParamPostTypeSpec ();
  }
  else
    parseParam ();

  repeatWhile (
    [this]
    {
      match (TokenType::Comma);
      parseParam ();
    },
    { TokenType::Comma });
}

void
Parser::parseParam ()
{
  parseTypeSpec ();
  ParseParamPostTypeSpec ();
}

void
Parser::ParseParamPostTypeSpec ()
{
  match (TokenType::Id);
  if (check (TokenType::LBrack))
  {
    match (TokenType::LBrack);
    match (TokenType::RBrack);
  }
}

void
Parser::parseCompoundStmt ()
{
  match (TokenType::LBrace);
  repeatWhile ([this] { parseVarDeclaration (); },
               { TokenType::Int, TokenType::Void });
  repeatUntil ([this] { parseStatement (); }, { TokenType::RBrace });
  match (TokenType::RBrace);
}

void
Parser::parseStatement ()
{
  switch (current_.type)
  {
    case TokenType::Semi:
    case TokenType::LParen:
    case TokenType::Id:
    case TokenType::Num:
      parseExpressionStmt ();
      break;
    case TokenType::LBrace:
      parseCompoundStmt ();
      break;
    case TokenType::If:
      parseSelectionStmt ();
      break;
    case TokenType::While:
      parseIterationStmt ();
      break;
    case TokenType::Return:
      parseReturnStmt ();
      break;
    default:
      throw InvalidSyntaxException (current_.line,
                                    current_.column,
                                    { TokenType::Semi,
                                      TokenType::LParen,
                                      TokenType::Id,
                                      TokenType::Num,
                                      TokenType::LBrace,
                                      TokenType::If,
                                      TokenType::While,
                                      TokenType::Return },
                                    current_.lexeme);
  }
}

void
Parser::parseExpressionStmt ()
{
  switch (current_.type)
  {
    case TokenType::LParen:
    case TokenType::Id:
    case TokenType::Num:
      parseExpression ();
      break;
    case TokenType::Semi:
      break;
    default:
      throw InvalidSyntaxException (
        current_.line,
        current_.column,
        { TokenType::Semi, TokenType::LParen, TokenType::Id, TokenType::Num },
        current_.lexeme);
  }
  match (TokenType::Semi);
}

void
Parser::parseSelectionStmt ()
{
  match (TokenType::If);
  match (TokenType::LParen);
  parseExpression ();
  match (TokenType::RParen);
  parseStatement ();
  if (check (TokenType::Else))
  {
    match (TokenType::Else);
    parseStatement ();
  }
}

void
Parser::parseIterationStmt ()
{
  match (TokenType::While);
  match (TokenType::LParen);
  parseExpression ();
  match (TokenType::RParen);
  parseStatement ();
}

void
Parser::parseReturnStmt ()
{
  match (TokenType::Return);
  if (!check (TokenType::Semi))
    parseExpression ();
  match (TokenType::Semi);
}

void
Parser::parseExpression ()
{
  if (!check (TokenType::Id))
  {
    parseSimpleExpr ();
    return;
  }

  match (TokenType::Id);

  // If parsing <call>
  if (check (TokenType::LParen))
  {
    match (TokenType::LParen);
    if (!check (TokenType::RParen))
      parseArgs ();
    match (TokenType::RParen);
    graduateFactor ();
    parseSimpleExprPostAddExpr ();
    return;
  }

  // If parsing <var>
  else if (check (TokenType::LBrack))
  {
    match (TokenType::LBrack);
    parseExpression ();
    match (TokenType::RBrack);
  }

  // If still parsing here, must have parsed: < ID | <var> >

  // If < ID | <var> > was a factor
  if (!check (TokenType::Assign))
  {
    graduateFactor ();
    parseSimpleExprPostAddExpr ();
  }

  // If < ID | <var> > was part of "<var> = <expr>"
  else
  {
    match (TokenType::Assign);
    parseExpression ();
  }
}

void
Parser::graduateFactor ()
{
  // Graduate from <factor> to <term>
  repeatWhile (
    [this]
    {
      match (current_.type);
      parseFactor ();
    },
    { TokenType::Multiply, TokenType::Divide });

  // Graduate from <term> to <additive-expression>
  repeatWhile (
    [this]
    {
      match (current_.type);
      parseTerm ();
    },
    { TokenType::Plus, TokenType::Minus });
}

void
Parser::parseVar ()
{
  match (TokenType::Id);
  if (check (TokenType::LBrack))
  {
    match (TokenType::LBrack);
    parseExpression ();
    match (TokenType::RBrack);
  }
}

void
Parser::parseSimpleExpr ()
{
  parseAdditiveExpr ();
  parseSimpleExprPostAddExpr ();
}

void
Parser::parseSimpleExprPostAddExpr ()
{
  switch (current_.type)
  {
    case TokenType::LessEqual:
    case TokenType::Less:
    case TokenType::Greater:
    case TokenType::GreaterEqual:
    case TokenType::Equal:
    case TokenType::NotEqual:
    {
      match (current_.type);
      parseAdditiveExpr ();
      break;
    }
    default:
      return;
  }
}

void
Parser::parseAdditiveExpr ()
{
  parseTerm ();
  repeatWhile (
    [this]
    {
      match (current_.type);
      parseTerm ();
    },
    { TokenType::Plus, TokenType::Minus });
}

void
Parser::parseTerm ()
{
  parseFactor ();
  repeatWhile (
    [this]
    {
      match (current_.type);
      parseFactor ();
    },
    { TokenType::Multiply, TokenType::Divide });
}

void
Parser::parseFactor ()
{
  switch (current_.type)
  {
    case TokenType::LParen:
      match (TokenType::LParen);
      parseExpression ();
      match (TokenType::RParen);
      break;
    case TokenType::Num:
      match (TokenType::Num);
      break;
    case TokenType::Id:
      match (TokenType::Id);
      switch (current_.type)
      {
        case TokenType::LBrack:
          match (TokenType::LBrack);
          parseExpression ();
          match (TokenType::RBrack);
          break;
        case TokenType::LParen:
          match (TokenType::LParen);
          parseExpression ();
          match (TokenType::RParen);
          break;
        default:
          return;
      }
      break;
    default:
      throw InvalidSyntaxException (current_.line,
                                    current_.column,
                                    {
                                      TokenType::LParen,
                                      TokenType::Id,
                                      TokenType::Num,
                                    },
                                    current_.lexeme);
  }
}

void
Parser::parseArgs ()
{
  parseExpression ();
  repeatWhile (
    [this]
    {
      match (TokenType::Comma);
      parseExpression ();
    },
    { TokenType::Comma });
}