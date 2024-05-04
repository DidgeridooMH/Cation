#pragma once

#include <memory>

#include "Vypr/AST/Expression/ExpressionNode.hpp"

namespace Vypr
{
  /// @brief Node to cast an expression to a new type.
  struct CastNode : public ExpressionNode
  {
    /// @brief Create a cast node in the AST tree.
    /// @param castType Type to cast `expression` to.
    /// @param expression Expression to to `castType`.
    CastNode(std::unique_ptr<StorageType> &&castType,
             std::unique_ptr<ExpressionNode> &&expression);

    /// @brief Print the AST tree for the cast node.
    /// @param level Indentation level of the current tree position.
    /// @return String containing the remaining sub-tree starting at this node.
    std::wstring PrettyPrint(int level) const override;

    /// @brief Generate IR tree based on the sub-tree starting at this node.
    /// @param context Current compilation context.
    /// @return Value computed from this IR sub-tree.
    llvm::Value *GenerateCode(Context &context) const override;

    static std::unique_ptr<ExpressionNode> Parse(CLangLexer &lexer,
                                                 TypeTable &symbolTable);

    /// @brief Expression to cast to a new type.
    std::unique_ptr<ExpressionNode> expression;

  private:
    llvm::Value *CastToIntegral(Context &context,
                                llvm::Value *childExpression) const;

    llvm::Value *CastToReal(Context &context,
                            llvm::Value *childExpression) const;

    llvm::Value *CastToPointer(Context &context,
                               llvm::Value *childExpression) const;

    llvm::Value *CastIntegralToIntegral(Context &context,
                                        llvm::Value *childExpression) const;
  };
} // namespace Vypr