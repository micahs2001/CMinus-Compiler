// #pragma once

// #include "exceptions/Exceptions.hh"
// #include "parsing/Parser.hh"
// #include "tokenizing/Token.hh"

// Parser::Parser (Scanner scanner)
//     : scanner_ (scanner)
// {
// }

// void
// Parser::parse ()
// {
//   try
//   {
//     advance ();
//   }
//   catch (const InvalidTokenException& e)
//   {
//     std::println (stderr, "{}", e.what ());
//     exit (1);
//   }
//   try
//   {
//     parseProgram ();
//   }
//   catch (const InvalidSyntaxException& e)
//   {
//     std::println (stderr, "{}", e.what ());
//     exit (1);
//   }
//   std::println ("\n\nValid!");
//   return;
// }

// Token
// Parser::advance ()
// {
//   Token save = current_;
//   current_ = scanner_.scanNext ();
//   if (check (TokenType::Error))
//     throw InvalidTokenException ("Invalid Token");
//   return save;
// }

// Token
// Parser::match (TokenType expected)
// {
//   if (!check (expected))
//   {
//     throw InvalidSyntaxException ("Invalid Syntax");
//     return current_;
//   }
//   return advance ();
// }

// bool
// Parser::check (TokenType type) const
// {
//   return (current_.type == type);
// }

// bool
// Parser::findToken (std::initializer_list<TokenType> types) const
// {
//   return std::ranges::find (types, current_.type) != types.end ();
// }

// template<std::invocable Fun>
// void
// Parser::repeatUntil (Fun rule, std::initializer_list<TokenType> terminators)
// {
//   while (!findToken (terminators) && current_.type != TokenType::EndOfFile)
//     rule ();
// }

// template<std::invocable Fun>
// void
// Parser::repeatWhile (Fun rule, std::initializer_list<TokenType> first)
// {
//   while (findToken (first))
//     rule ();
// }

// /*
// <program> ::= <declaration-list>
// <declaration-list> ::= <declaration-list> <declaration> |
//                        <declaration>
// */
// void
// Parser::parseProgram ()
// {
//   repeatUntil ([this] { return parseDecl (); }, { TokenType::EndOfFile });
// }

// /*
// <declaration> ::= <var-declaration> |
//                   <fun-declaration>
// */
// void
// Parser::parseDecl ()
// {
//   parseTypeSpec ();
//   match (TokenType::Id);

//   switch (current_.type)
//   {
//     case TokenType::Semi:
//       match (TokenType::Semi);
//       break;
//     case TokenType::LBrack:
//       match (TokenType::LBrack);
//       match (TokenType::Num);
//       match (TokenType::RBrack);
//       match (TokenType::Semi);
//       break;
//     case TokenType::LParen:
//       match (TokenType::LParen);
//       parseFullParams ();
//       match (TokenType::RParen);
//       parseCompoundStmt ();
//       break;
//     default:
//       throw InvalidSyntaxException ("Invalid Declaration");
//   }
// }

// void
// Parser::parseVarDeclaration ()
// {
//   parseTypeSpec ();
//   match (TokenType::Id);

//   switch (current_.type)
//   {
//     case TokenType::Semi:
//       match (TokenType::Semi);
//       break;
//     case TokenType::LBrack:
//       match (TokenType::LBrack);
//       match (TokenType::Num);
//       match (TokenType::RBrack);
//       match (TokenType::Semi);
//       break;
//     default:
//       throw InvalidSyntaxException ("Invalid Variable Declaration");
//   }
// }

// /*
// <type-specifier> ::= "int"  |
//                      "void"
// */
// void
// Parser::parseTypeSpec ()
// {
//   switch (current_.type)
//   {
//     case TokenType::Int:
//       match (TokenType::Int);
//       break;
//     case TokenType::Void:
//       match (TokenType::Void);
//       break;
//     default:
//       throw InvalidSyntaxException ("Type must be \"int\" or \"void\"");
//   }
// }

// // void
// // Parser::parseFunDeclaration ()
// // {
// //   parseTypeSpec ();
// //   parseID ();
// //   match (TokenType::LParen);
// //   parseParams ();
// //   match (TokenType::RParen);
// //   parseCompoundStmnt ();
// // }

// /*
// <fullParams>     ::= <param-list>         |
//                      "void"

// <param-list> ::= <param-list> "," <param> |
//                  <param>
// */
// void
// Parser::parseFullParams ()
// {
//   if (check (TokenType::Void))
//   {
//     match (TokenType::Void);
//     // If "void" is at the end of params
//     if (check (TokenType::RParen))
//       return;
//     ParseParamPostTypeSpec ();
//   }
//   else
//     parseParam ();

//   repeatWhile (
//     [this]
//     {
//       match (TokenType::Comma);
//       parseParam ();
//     },
//     { TokenType::Comma });
// }

// /*
// <param> ::= <type-specifier> <paramPostTypeSpec>
// */
// void
// Parser::parseParam ()
// {
//   parseTypeSpec ();
//   ParseParamPostTypeSpec ();
// }

// /*
// <paramPostTypeSpec> ::= ID [ "[" "]" ]
// */
// void
// Parser::ParseParamPostTypeSpec ()
// {
//   match (TokenType::Id);
//   if (check (TokenType::LBrack))
//   {
//     match (TokenType::LBrack);
//     match (TokenType::RBrack);
//   }
// }

// /*
// <compound-stmt> ::= "{" <local-declarations> <statement-list> "}"
// */
// void
// Parser::parseCompoundStmt ()
// {
//   match (TokenType::LBrace);
//   repeatWhile ([this] { parseVarDeclaration (); },
//                { TokenType::Int, TokenType::Void });
//   repeatUntil ([this] { parseStatement (); }, { TokenType::RBrace });
//   match (TokenType::RBrace);
// }

// /*
// <statement> ::= <expression-stmt> |
//                 <compound-stmt>   |
//                 <selection-stmt>  |
//                 <iteration-stmt>  |
//                 <return-stmt>
// */
// void
// Parser::parseStatement ()
// {
//   switch (current_.type)
//   {
//     case TokenType::Semi:
//     case TokenType::LParen:
//     case TokenType::Id:
//     case TokenType::Num:
//       parseExpressionStmt ();
//       break;
//     case TokenType::LBrace:
//       parseCompoundStmt ();
//       break;
//     case TokenType::If:
//       parseSelectionStmt ();
//       break;
//     case TokenType::While:
//       parseIterationStmt ();
//       break;
//     case TokenType::Return:
//       parseReturnStmt ();
//       break;
//     default:
//       throw InvalidSyntaxException ("Must be valid statement syntax");
//   }
// }

// /*
// <expression-stmt> ::= <expression> ";" | ";"
// */
// void
// Parser::parseExpressionStmt ()
// {
//   switch (current_.type)
//   {
//     case TokenType::LParen:
//     case TokenType::Id:
//     case TokenType::Num:
//       parseExpression ();
//       break;
//     case TokenType::Semi:
//       break;
//     default:
//       throw InvalidSyntaxException ("Must be a valid expression");
//   }
//   match (TokenType::Semi);
// }

// /*
// <selection-stmt> ::= "if" "(" <expression> ")" <statement> |
//                      "if" "(" <expression> ")" <statement> "else" <statement>
// */
// void
// Parser::parseSelectionStmt ()
// {
//   match (TokenType::If);
//   match (TokenType::LParen);
//   parseExpression ();
//   match (TokenType::RParen);
//   parseStatement ();
//   if (current_.type == TokenType::Else)
//   {
//     match (TokenType::Else);
//     parseStatement ();
//   }
// }

// /*
// <iteration-stmt> ::= "while" "(" <expression> ")" <statement>
// */
// void
// Parser::parseIterationStmt ()
// {
//   match (TokenType::While);
//   match (TokenType::LParen);
//   parseExpression ();
//   match (TokenType::RParen);
//   parseStatement ();
// }

// /*
// <return-stmt> ::= "return" ";" | "return" <expression> ";"
// */
// void
// Parser::parseReturnStmt ()
// {
//   match (TokenType::Return);
//   if (current_.type != TokenType::Semi)
//     parseExpression ();
//   match (TokenType::Semi);
// }

// /*
// <expression> ::= <var> "=" <expression> | <simple-expression>
// */
// void
// Parser::parseExpression ()
// {
//   if (current_.type != TokenType::Id)
//   {
//     parseSimpleExpr ();
//     return;
//   }

//   match (TokenType::Id);

//   // If parsing <call>
//   if (current_.type == TokenType::LParen)
//   {
//     match (TokenType::LParen);
//     if (current_.type != TokenType::RParen)
//       parseArgs ();
//     match (TokenType::RParen);
//     parseSimpleExpr ();
//     return;
//   }

//   // If parsing <var>
//   else if (current_.type == TokenType::LBrack)
//   {
//     match (TokenType::LBrack);
//     parseExpression ();
//     match (TokenType::RBrack);
//   }

//   // If still parsing here, must have parsed: < ID | <var> >

//   // If < ID | <var> > was a factor
//   if (current_.type != TokenType::Assign)
//   {
//     graduateFactor ();
//     parseSimpleExprPostAddExpr ();
//   }

//   // If < ID | <var> > was part of "<var> = <expr>"
//   else
//   {
//     match (TokenType::Assign);
//     parseExpression ();
//   }
// }

// void
// Parser::graduateFactor ()
// {
//   // Graduate from <factor> to <term>
//   repeatWhile (
//     [this]
//     {
//       match (current_.type);
//       parseFactor ();
//     },
//     { TokenType::Multiply, TokenType::Divide });

//   // Graduate from <term> to <additive-expression>
//   repeatWhile (
//     [this]
//     {
//       match (current_.type);
//       parseTerm ();
//     },
//     { TokenType::Plus, TokenType::Minus });
// }

// /*
// <var> ::= ID | ID "[" <expression> "]"
// */
// void
// Parser::parseVar ()
// {
//   match (TokenType::Id);
//   if (current_.type == TokenType::LBrack)
//   {
//     match (TokenType::LBrack);
//     parseExpression ();
//     match (TokenType::RBrack);
//   }
// }

// /*
// <simple-expression> ::= <additive-expression> { <relop> <additive-expression>
// } <relop>             ::= "<=" | "<" | ">" | ">=" | "==" | "!="
// */
// void
// Parser::parseSimpleExpr ()
// {
//   parseAdditiveExpr ();
//   parseSimpleExprPostAddExpr ();
// }

// /*
// <simple-expression> ::= <additive-expression> { <relop> <additive-expression>
// }
// */
// void
// Parser::parseSimpleExprPostAddExpr ()
// {
//   switch (current_.type)
//   {
//     case TokenType::LessEqual:
//     case TokenType::Less:
//     case TokenType::Greater:
//     case TokenType::GreaterEqual:
//     case TokenType::Equal:
//     case TokenType::NotEqual:
//     {
//       match (current_.type);
//       parseAdditiveExpr ();
//       break;
//     }
//     default:
//       return;
//   }
// }

// /*
// <additive-expression>  ::= <additive-expression> <addop> <term> | <term>
// <addop>                ::= "+" | "-"
// */
// void
// Parser::parseAdditiveExpr ()
// {
//   parseTerm ();
//   repeatWhile (
//     [this]
//     {
//       match (current_.type);
//       parseTerm ();
//     },
//     { TokenType::Plus, TokenType::Minus });
// }

// /*
// <term>   ::= <term> <mulop> <factor> | <factor>
// <mulop>  ::= "*" | "/"
// */
// void
// Parser::parseTerm ()
// {
//   parseFactor ();
//   repeatWhile (
//     [this]
//     {
//       match (current_.type);
//       parseFactor ();
//     },
//     { TokenType::Multiply, TokenType::Divide });
// }

// /*
// <factor> ::= "(" <expression> ")" | <var> | <call> | NUM
// */
// void
// Parser::parseFactor ()
// {
//   switch (current_.type)
//   {
//     case TokenType::LParen:
//       match (TokenType::LParen);
//       parseExpression ();
//       match (TokenType::RParen);
//       break;
//     case TokenType::Num:
//       match (TokenType::Num);
//       break;
//     case TokenType::Id:
//       match (TokenType::Id);
//       switch (current_.type)
//       {
//         case TokenType::LBrack:
//           match (TokenType::LBrack);
//           parseExpression ();
//           match (TokenType::RBrack);
//           break;
//         case TokenType::LParen:
//           match (TokenType::LParen);
//           parseExpression ();
//           match (TokenType::RParen);
//           break;
//         default:
//           return;
//       }
//       break;
//     default:
//       throw InvalidSyntaxException ("");
//   }
// }

// void
// Parser::parseArgs ()
// {
//   parseExpression ();
//   repeatWhile (
//     [this]
//     {
//       match (TokenType::Comma);
//       parseExpression ();
//     },
//     { TokenType::Comma });
// }