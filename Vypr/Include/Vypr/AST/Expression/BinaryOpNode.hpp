#pragma once

#include <memory>
#include <unordered_map>

#include "Vypr/AST/Expression/ExpressionNode.hpp"

namespace Vypr
{
  enum class BinaryOp
  {
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,
    ShiftLeft,
    ShiftRight,
    LessThan,
    LessEqual,
    GreaterThan,
    GreaterEqual,
    Equal,
    NotEqual,
    And,
    Xor,
    Or,
    LogicalAnd,
    LogicalOr
  };

  class BinaryOpNode : public ExpressionNode
  {
  public:
    BinaryOpNode(BinaryOp op, std::unique_ptr<ExpressionNode> lhs,
                 std::unique_ptr<ExpressionNode> rhs, size_t column,
                 size_t line);

    std::wstring PrettyPrint(int level) const override;

    static std::unique_ptr<ExpressionNode> Parse(
        std::unique_ptr<ExpressionNode> base, CLangLexer &lexer);

  private:
    void CastType();

    BinaryOp m_op;
    std::unique_ptr<ExpressionNode> m_lhs;
    std::unique_ptr<ExpressionNode> m_rhs;
  };

  extern const std::unordered_map<CLangTokenType, BinaryOp> BinaryOperations;
  extern const std::unordered_map<BinaryOp, int> BinaryOperationPrecedence;
} // namespace Vypr