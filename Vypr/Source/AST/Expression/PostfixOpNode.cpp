#include "Vypr/AST/Expression/PostfixOpNode.hpp"

#include "Vypr/AST/Type/TypeException.hpp"

namespace Vypr
{
  PostfixOpNode::PostfixOpNode(PostfixOp op,
                               std::unique_ptr<ExpressionNode> &expression,
                               size_t column, size_t line)
      : ExpressionNode(nullptr, column, line), m_op(op),
        m_expression(std::move(expression))
  {
    type = m_expression->type->Check(op);
    if (type == nullptr)
    {
      throw TypeException("Invalid operands", column, line);
    }
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