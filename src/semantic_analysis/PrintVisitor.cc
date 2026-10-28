#include <format>
#include <ostream>
#include <print>
#include <ranges>

#include "semantic_analysis/Ast.hh"
#include "semantic_analysis/PrintVisitor.hh"
#include "tokenizing/Token.hh"

PrintVisitor::PrintVisitor (std::ostream& out)
    : out_ (out)
{
}

// Entry point
void
PrintVisitor::visit (const AstNodePtr& node)
{
  std::visit (*this, node->data);
}

// One overload per NodeVariant alternative (called by std::visit)
void
PrintVisitor::operator() (const ProgramNode& node)
{
  printLine ("ProgramNode:");
  ++depth_;
  for (const auto& decl : node.declarations)
    visit (decl);
  --depth_;
}

void
PrintVisitor::operator() (const VarDeclNode& node)
{
  printLine ("VarDeclNode: {} {}{}",
             toString (node.type),
             node.identifier,
             (node.arraySize) ? std::format ("[{}]", *node.arraySize) : "");
}

void
PrintVisitor::operator() (const FunDeclNode& node)
{
  printLine ("FunDeclNode: {} {}", toString (node.returnType), node.identifier);
  ++depth_;
  printLine ("Params:");
  ++depth_;
  for (const auto& param : node.params)
    visit (param);
  --depth_;
  printLine ("Body:");
  ++depth_;
  visit (node.body);
  depth_ -= 2;
}

void
PrintVisitor::operator() (const ParamDeclNode& node)
{
  printLine ("ParamDeclNode: {} {}{}",
             toString (node.type),
             node.identifier,
             (node.isArray) ? "[]" : "");
}

void
PrintVisitor::operator() (const CompoundStmtNode& node)
{
  printLine ("CompoundStmtNode:");
  ++depth_;
  if (not node.localDecls.empty ())
  {
    printLine ("Local Decls:");
    ++depth_;
    for (const auto& decl : node.localDecls)
      visit (decl);
    --depth_;
  }
  printLine ("Statements:");
  ++depth_;
  for (const auto& stmt : node.stmtList)
    visit (stmt);
  depth_ -= 2;
}

void
PrintVisitor::operator() (const ExprStmtNode& node)
{
  printLine ("ExprStmtNode:");
  ++depth_;
  visit (node.expr);
  --depth_;
}

void
PrintVisitor::operator() (const IfStmtNode& node)
{
  printLine ("IfStmtNode:");
  ++depth_;
  printLine ("Condition:");
  ++depth_;
  visit (node.condition);
  --depth_;
  printLine ("Then:");
  ++depth_;
  visit (node.thenStmt);
  --depth_;
  if (node.elseStmt != nullptr)
  {
    printLine ("Else:");
    ++depth_;
    visit (node.elseStmt);
    --depth_;
  }
  --depth_;
}

void
PrintVisitor::operator() (const WhileStmtNode& node)
{
  printLine ("WhileStmtNode:");
  ++depth_;
  printLine ("Condition:");
  ++depth_;
  visit (node.condition);
  --depth_;
  printLine ("Body:");
  ++depth_;
  visit (node.body);
  depth_ -= 2;
}

void
PrintVisitor::operator() (const ReturnStmtNode& node)
{
  printLine ("ReturnStmtNode:");
  ++depth_;
  visit (node.expr);
  --depth_;
}

void
PrintVisitor::operator() (const AssignExprNode& node)
{
  printLine ("AssignExprNode:");
  ++depth_;
  printLine ("LHS:");
  ++depth_;
  visit (node.var);
  --depth_;
  printLine ("RHS:");
  ++depth_;
  visit (node.expression);
  depth_ -= 2;
}

void
PrintVisitor::operator() (const BinaryExprNode& node)
{
  printLine ("BinaryExprNode ({}):", tokenToString (node.op));
  ++depth_;
  visit (node.left);
  visit (node.right);
  --depth_;
}

void
PrintVisitor::operator() (const VarExprNode& node)
{
  printLine ("VarExprNode: {}", node.identifier);
  if (node.index)
  {
    ++depth_;
    printLine ("Index:");
    ++depth_;
    visit (node.index);
    depth_ -= 2;
  }
}

void
PrintVisitor::operator() (const CallExprNode& node)
{
  printLine ("CallExprNode: {}", node.identifier);
  ++depth_;
  printLine ("Args:");
  ++depth_;
  for (const auto& arg : node.args)
    visit (arg);
  depth_ -= 2;
}

void
PrintVisitor::operator() (const NumExprNode& node)
{
  printLine ("NumExprNode: {}", node.value);
}

template<typename... Args>
void
PrintVisitor::printLine (std::format_string<Args...> fmt, Args&&... args)
{
  for (int _ : std::views::iota (0u, (unsigned) depth_))
  {
    out_ << "  ";
  }
  std::println (out_, fmt, std::forward<Args> (args)...);
}

const char*
PrintVisitor::toString (TypeSpecifier type)
{
  switch (type)
  {
    case TypeSpecifier::Int:
      return "int";
    case TypeSpecifier::Void:
      return "void";
    default:
      return "unknown-type";
  }
}