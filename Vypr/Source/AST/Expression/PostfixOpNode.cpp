#include "Vypr/AST/Expression/PostfixOpNode.hpp"

namespace Vypr
{
  PostfixOpNode::PostfixOpNode(PostfixOp op,
                               std::unique_ptr<ExpressionNode> expression,
                               ValueType type)
      : ExpressionNode(type), m_op(op), m_expression(std::move(expression))
  {
  }

  std::wstring PostfixOpNode::PrettyPrint(int level) const
  {
    std::wstring result = ExpressionNode::PrettyPrint(level);
    result += L"PostfixOp(";
    if (m_op == PostfixOp::Increment)
    {
      result += L"Increment";
    }
    else if (m_op == PostfixOp::Decrement)
    {
      result += L"Decrement";
    }
    result += L")\n";
    result += m_expression->PrettyPrint(level + 1);
    return result;
  }
} // namespace Vypr