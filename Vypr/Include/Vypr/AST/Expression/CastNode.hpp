#pragma once

#include <memory>

#include "Vypr/AST/Expression/ExpressionNode.hpp"

namespace Vypr
{
  struct CastNode : public ExpressionNode
  {
    CastNode(std::unique_ptr<StorageType> &castType,
             std::unique_ptr<ExpressionNode> &expression);

    std::wstring PrettyPrint(int level) const override;

    llvm::Value *GenerateCode(Context &context) const override;

    std::unique_ptr<ExpressionNode> expression;

  private:
    llvm::Value *CastToIntegral(Context &context,
                                llvm::Value *childExpression) const;

    llvm::Value *CastIntegralToIntegral(Context &context,
                                        llvm::Value *childExpression) const;
  };
} // namespace Vypr