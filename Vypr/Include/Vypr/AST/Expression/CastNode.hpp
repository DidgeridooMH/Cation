#pragma once

#include <memory>

#include "Vypr/AST/Expression/ExpressionNode.hpp"

namespace Vypr
{
  struct CastNode : public ExpressionNode
  {
    CastNode(std::unique_ptr<StorageType> &castType,
             std::unique_ptr<ExpressionNode> &expression);

    std::wstring PrettyPrint(int level) const;

    std::unique_ptr<ExpressionNode> expression;
  };
} // namespace Vypr