#pragma once

#include <format>
#include <ostream>

#include "semantic_analysis/Ast.hh"

class PrintVisitor
{
public:
  explicit PrintVisitor (std::ostream& out);

  // Entry point
  void
  visit (const AstNodePtr& node);

  // One overload per NodeVariant alternative (called by std::visit)
  void
  operator() (const ProgramNode& node);
  void
  operator() (const VarDeclNode& node);
  void
  operator() (const FunDeclNode& node);
  void
  operator() (const ParamDeclNode& node);
  void
  operator() (const CompoundStmtNode& node);
  void
  operator() (const ExprStmtNode& node);
  void
  operator() (const IfStmtNode& node);
  void
  operator() (const WhileStmtNode& node);
  void
  operator() (const ReturnStmtNode& node);
  void
  operator() (const AssignExprNode& node);
  void
  operator() (const BinaryExprNode& node);
  void
  operator() (const VarExprNode& node);
  void
  operator() (const CallExprNode& node);
  void
  operator() (const NumExprNode& node);

private:
  template<typename... Args>
  void
  printLine (std::format_string<Args...> fmt, Args&&... args);

  static const char*
  toString (TypeSpecifier type);

  std::ostream& out_;
  int depth_ { 0 };
};