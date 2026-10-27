
#include "tokenizing/Token.hh"

std::string_view
tokenToString (TokenType type)
{
  switch (type)
  {
    case TokenType::If:
      return "if";
    case TokenType::Else:
      return "else";
    case TokenType::Int:
      return "int";
    case TokenType::Void:
      return "void";
    case TokenType::Return:
      return "return";
    case TokenType::While:
      return "while";
    case TokenType::Plus:
      return "+";
    case TokenType::Minus:
      return "-";
    case TokenType::Multiply:
      return "*";
    case TokenType::Divide:
      return "/";
    case TokenType::Less:
      return "<";
    case TokenType::LessEqual:
      return "<=";
    case TokenType::Greater:
      return ">";
    case TokenType::GreaterEqual:
      return ">=";
    case TokenType::Equal:
      return "==";
    case TokenType::NotEqual:
      return "!=";
    case TokenType::Assign:
      return "=";
    case TokenType::Semi:
      return ";";
    case TokenType::Comma:
      return ",";
    case TokenType::LParen:
      return "(";
    case TokenType::RParen:
      return ")";
    case TokenType::LBrack:
      return "[";
    case TokenType::RBrack:
      return "]";
    case TokenType::LBrace:
      return "{";
    case TokenType::RBrace:
      return "}";
    case TokenType::Id:
      return "Id";
    case TokenType::Num:
      return "Num";
    case TokenType::EndOfFile:
      return "EndOfFile";
    case TokenType::Error:
      return "Error";
    default:
      return "Unknown";
  }
}