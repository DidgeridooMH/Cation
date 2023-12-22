#pragma once

#include <memory>

#include "Vypr/AST/Expression/ExpressionNode.hpp"
#include "Vypr/AST/Expression/PostfixOp.hpp"

namespace Vypr
{
  class PostfixOpNode : public ExpressionNode
  {
  public:
    PostfixOpNode(PostfixOp op, std::unique_ptr<ExpressionNode> &&expression,
                  size_t column, size_t line);

    std::wstring PrettyPrint(int level) const override;

  private:
    PostfixOp m_op;
    std::unique_ptr<ExpressionNode> m_expression;
  };
} // namespace Vypr