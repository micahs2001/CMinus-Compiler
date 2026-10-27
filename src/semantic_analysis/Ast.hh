#pragma once

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <llvm/IR/Value.h>

#include "tokenizing/Token.hh"

struct ProgramNode;
struct VarDeclNode;
struct FunDeclNode;
struct ParamDeclNode;
struct CompoundStmtNode;
struct ExprStmtNode;
struct IfStmtNode;
struct WhileStmtNode;
struct ReturnStmtNode;
struct AssignExprNode;
struct BinaryExprNode;
struct VarExprNode;
struct CallExprNode;
struct NumExprNode;

using AstNodePtr = std::unique_ptr<struct AstNode>;

// Symbol reference for variable usage.
// Can point to a local/global or parameter.
using VarDeclSymbol = std::variant<VarDeclNode*, ParamDeclNode*>;

enum class TypeSpecifier
{
  Int,
  Void
};

struct ProgramNode
{
  std::vector<AstNodePtr> declarations;
};

struct VarDeclNode
{
  TypeSpecifier type;
  std::string identifier;
  std::optional<int> arraySize;
  // Attached during CodeGen
  mutable llvm::Value* llvmValue { nullptr };
};

struct FunDeclNode
{
  TypeSpecifier returnType;
  std::string identifier;
  std::vector<AstNodePtr> params;
  AstNodePtr body;
  // Attached during CodeGen
  mutable llvm::Value* llvmValue { nullptr };
};

struct ParamDeclNode
{
  TypeSpecifier type;
  std::string identifier;
  bool isArray { false };
  // Attached during CodeGen
  mutable llvm::Value* llvmValue { nullptr };
};

struct CompoundStmtNode
{
  std::vector<AstNodePtr> localDecls;
  std::vector<AstNodePtr> stmtList;
};

struct ExprStmtNode
{
  AstNodePtr expr;
};

struct IfStmtNode
{
  AstNodePtr condition;
  AstNodePtr thenStmt;
  AstNodePtr elseStmt;
};

struct WhileStmtNode
{
  AstNodePtr condition;
  AstNodePtr body;
};

struct ReturnStmtNode
{
  AstNodePtr expr;
};

struct AssignExprNode
{
  AstNodePtr var;
  AstNodePtr expression;
};

struct BinaryExprNode
{
  TokenType op;
  AstNodePtr left;
  AstNodePtr right;
};

struct VarExprNode
{
  std::string identifier;
  AstNodePtr index { nullptr };
  // Attached by SymbolTableVisitor.
  VarDeclSymbol declNode;
};

struct CallExprNode
{
  std::string identifier;
  std::vector<AstNodePtr> args;
  // Attached by SymbolTableVisitor.
  FunDeclNode* declNode { nullptr };
};

struct NumExprNode
{
  int value;
};

// NodeVariant uses std::variant to represent any concrete AST node.
using NodeVariant = std::variant<ProgramNode,
                                 VarDeclNode,
                                 FunDeclNode,
                                 ParamDeclNode,
                                 CompoundStmtNode,
                                 ExprStmtNode,
                                 IfStmtNode,
                                 WhileStmtNode,
                                 ReturnStmtNode,
                                 AssignExprNode,
                                 BinaryExprNode,
                                 VarExprNode,
                                 CallExprNode,
                                 NumExprNode>;

struct AstNode
{
  NodeVariant data;

  template<typename T>
  explicit AstNode (T&& val)
      : data (std::forward<T> (val))
  {
  }
};

template<typename T, typename... Args>
AstNodePtr
makeAstNode (Args&&... args)
{
  return std::make_unique<AstNode> (T { std::forward<Args> (args)... });
}
