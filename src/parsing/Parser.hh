/******************************************************************************
 File Name:    Parser.hh
 Description:  Parser class definition for CMinus language
 Author:       Micah Schafer
 Date:         October 16, 2026

 *****************************************************************************/

/*****************************************************************************/
// Includes
#pragma once

#include "lexing/Scanner.hh"
#include "semantic_analysis/Ast.hh"

/*****************************************************************************/
// Parser Class Definition

class Parser
{
public:
  explicit Parser (Scanner scanner);

  AstNodePtr
  parse ();

private:
  // Save the current token, and replace it with the next one.
  // If there's an error, throw a ScanError.
  // Return the saved token.
  Token
  advance ();

  // Check if current token matches `expected`, and advance if so.
  // Otherwise, throw a ParseError.
  Token
  match (TokenType expected);

  // Check if the current token matches `type`.
  bool
  check (TokenType type) const;

  // Check if current token is in 'types'
  bool
  findToken (std::initializer_list<TokenType> types) const;

  // Repeat function 'rule()' until the current token is found in 'terminators'
  template<std::invocable Fun>
  void
  repeatUntil (Fun rule, std::initializer_list<TokenType> terminators);

  // Repeat function 'rule()' while the current token is found in 'first'
  template<std::invocable Fun>
  void
  repeatWhile (Fun rule, std::initializer_list<TokenType> first);

  /****************************************************************************
   * Parse program of form:
   * <program> ::= <declaration> { <declaration> } <EOF>
   ***************************************************************************/
  AstNodePtr
  parseProgram ();

  /****************************************************************************
   * Parse declaration of form:
   * <declaration> ::= <type-spec> <id> ( ";"
   *                                    | "[" <num> "]" ";"
   *                                    | "(" <full-params> ")" <compound-stmt>
   *                                    )
   ***************************************************************************/
  AstNodePtr
  parseDecl ();

  /****************************************************************************
   * Parse variable declaration of form:
   * <var-declaration> ::= <type-spec> <id> [ "[" NUM "]" ] ";"
   ***************************************************************************/
  AstNodePtr
  parseVarDeclaration ();

  /****************************************************************************
   * Parse type specifier of form:
   * <type-spec> ::= "int" | "void"
   ***************************************************************************/
  TypeSpecifier
  parseTypeSpec ();

  /****************************************************************************
   * Parse parameters of form:
   * <full-params> ::= "void"
                     | <type-spec> <param-post-type-spec>
                     | <param>
                     | <full-params> { "," <param> }
   ***************************************************************************/
  std::vector<AstNodePtr>
  parseFullParams ();

  /****************************************************************************
   * Parse parameter of form:
   * <param> ::= <type-spec> <param-post-type-spec>
   ***************************************************************************/
  AstNodePtr

  parseParam ();

  /****************************************************************************
   * Parse compound statement of form:
   * <compound-stmt> ::= "{" { <var-declaration> } { <statement> } "}"
   ***************************************************************************/

  AstNodePtr
  parseCompoundStmt ();

  /****************************************************************************
   * Parse statement of form:
   * <statement> ::= <expression-stmt>
   *               | <compound-stmt>
   *               | <selection-stmt>
   *               | <iteration-stmt>
   *               | <return-stmt>
   ***************************************************************************/
  AstNodePtr
  parseStatement ();

  /****************************************************************************
   * Parse expression statement of form:
   * <expression-stmt> ::= [ <expression> ] ";"
   ***************************************************************************/
  AstNodePtr
  parseExpressionStmt ();

  /****************************************************************************
   * Parse selection statement of form:
   * <selection-stmt> ::= "if" "(" <expression> ")" <statement>
   *                      [ "else" <statement> ]
   ***************************************************************************/
  AstNodePtr
  parseSelectionStmt ();

  /****************************************************************************
   * Parse iteration statement of form:
   * <iteration-stmt> ::= "while" "(" <expression> ")" <statement>
   ***************************************************************************/
  AstNodePtr
  parseIterationStmt ();

  /****************************************************************************
   * Parse return statement of form:
   * <return-stmt> ::= "return" [ <expression> ] ";"
   ***************************************************************************/
  AstNodePtr
  parseReturnStmt ();

  /****************************************************************************
   * Parse expression of form:
   * <expression> ::= <var> "=" <expression>
   *                | <simple-expression>
   *                | <additive-expression> <simple-expr-post-add-expr>
   ***************************************************************************/
  AstNodePtr
  parseExpression ();

  /****************************************************************************
   * Graduates a factor that was just parsed to an additive expression
   ***************************************************************************/
  AstNodePtr
  graduateFactor (AstNodePtr leftTree);

  /****************************************************************************
   * Parse a variable of form:
   * <var> ::= ID | ID "[" <expression> "]"
   ***************************************************************************/
  AstNodePtr
  parseVar ();

  /****************************************************************************
   * Parse simple expression of form:
   * <simple-expression> ::= <additive-expression> <simple-expr-post-add-expr>
   ***************************************************************************/
  AstNodePtr
  parseSimpleExpr ();

  bool
  isRelop ();

  /****************************************************************************
   * Parse a simple expression after an additive expression of form:
   * <simple-expr-post-add-expr> ::= [ <relop> <additive-exprression> ]
   * Where <relop> ::= "<=" | "<" | ">" | ">=" | "==" | "!="
   ***************************************************************************/
  AstNodePtr
  parseSimpleExprPostAddExpr ();

  /****************************************************************************
   * Parse additive expression of form:
   * <additive-expression> ::= <term> { <addop> <term> }
   * Where <addop> ::= "+" | "-"
   ***************************************************************************/
  AstNodePtr
  parseAdditiveExpr ();

  /****************************************************************************
   * Parse term of form:
   * <term> ::= <factor> { <mulop> <factor> }
   * Where <mulop> ::= "*" | "/"
   ***************************************************************************/
  AstNodePtr
  parseTerm ();

  /****************************************************************************
   * Parse factor of form:
   * <factor> ::= "(" <expression> ")" | <var> | <call> | NUM
   ***************************************************************************/
  AstNodePtr
  parseFactor ();

  /****************************************************************************
   * Parse args of form:
   * <expression> { "," <expression> }
   ***************************************************************************/
  std::vector<AstNodePtr>
  parseArgs ();

  /*****************************************************************************/
  // Class Fields

private:
  Scanner scanner_;
  Token current_;
};