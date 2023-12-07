#include "Vypr/AST/Expression/CastNode.hpp"

namespace Vypr
{
  CastNode::CastNode(std::unique_ptr<StorageType> &castType,
                     std::unique_ptr<ExpressionNode> &expression)
      : ExpressionNode(std::move(castType), expression->column,
                       expression->line),
        expression(std::move(expression))
  {
  }

  std::wstring CastNode::PrettyPrint(int level) const
  {
    std::wstring result = ExpressionNode::PrettyPrint(level);
    result += L"CastNode\n";
    result += expression->PrettyPrint(level + 1);
    return result;
  }
} // namespace Vypr