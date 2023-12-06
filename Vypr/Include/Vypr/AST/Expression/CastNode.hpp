#pragma once

#include <memory>

#include "Vypr/AST/Expression/ExpressionNode.hpp"
#include "Vypr/AST/Expression/ValueType.hpp"

namespace Vypr
{
  struct CastNode : public ExpressionNode
  {
    CastNode(ValueType castType, std::unique_ptr<ExpressionNode> expression)
        : expression(std::move(expression))
    {
      type = castType;
    }

    std::wstring PrettyPrint(int level) const
    {
      std::wstring result = ExpressionNode::PrettyPrint(level);
      result += L"CastNode\n";
      result += expression->PrettyPrint(level + 1);
      return result;
    }

    std::unique_ptr<ExpressionNode> expression;
  };
} // namespace Vypr