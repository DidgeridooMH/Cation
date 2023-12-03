#include "Vypr/AST/Expression/BinaryOpNode.hpp"

namespace Vypr
{
  const std::unordered_map<CLangTokenType, BinaryOp> BinaryOperations = {
      {CLangTokenType::Add, BinaryOp::Add},
      {CLangTokenType::Subtract, BinaryOp::Subtract},
      {CLangTokenType::Star, BinaryOp::Multiply},
      {CLangTokenType::Divide, BinaryOp::Divide},
      {CLangTokenType::Modulo, BinaryOp::Modulo},
      {CLangTokenType::ShiftLeft, BinaryOp::ShiftLeft},
      {CLangTokenType::ShiftRight, BinaryOp::ShiftRight},
      {CLangTokenType::LessThan, BinaryOp::LessThan},
      {CLangTokenType::LessEqual, BinaryOp::LessEqual},
      {CLangTokenType::GreaterThan, BinaryOp::GreaterThan},
      {CLangTokenType::GreaterEqual, BinaryOp::GreaterEqual},
      {CLangTokenType::Equal, BinaryOp::Equal},
      {CLangTokenType::NotEqual, BinaryOp::NotEqual},
      {CLangTokenType::And, BinaryOp::And},
      {CLangTokenType::Xor, BinaryOp::Xor},
      {CLangTokenType::Or, BinaryOp::Or},
      {CLangTokenType::LogicalAnd, BinaryOp::LogicalAnd},
      {CLangTokenType::LogicalOr, BinaryOp::LogicalOr}};

  const std::unordered_map<BinaryOp, int> BinaryOperationPrecedence = {
      {BinaryOp::Add, 3},          {BinaryOp::Subtract, 3},
      {BinaryOp::Multiply, 2},     {BinaryOp::Divide, 2},
      {BinaryOp::Modulo, 2},       {BinaryOp::ShiftLeft, 4},
      {BinaryOp::ShiftRight, 4},   {BinaryOp::LessThan, 5},
      {BinaryOp::LessEqual, 5},    {BinaryOp::GreaterThan, 5},
      {BinaryOp::GreaterEqual, 5}, {BinaryOp::Equal, 6},
      {BinaryOp::NotEqual, 6},     {BinaryOp::And, 7},
      {BinaryOp::Xor, 8},          {BinaryOp::Or, 9},
      {BinaryOp::LogicalAnd, 10},  {BinaryOp::LogicalOr, 11}};

  const std::unordered_map<BinaryOp, std::wstring> BinaryOperationNames = {
      {BinaryOp::Add, L"Add"},
      {BinaryOp::Subtract, L"Subtract"},
      {BinaryOp::Multiply, L"Multiply"},
      {BinaryOp::Divide, L"Divide"},
      {BinaryOp::Modulo, L"Modulo"},
      {BinaryOp::ShiftLeft, L"ShiftLeft"},
      {BinaryOp::ShiftRight, L"ShiftRight"},
      {BinaryOp::LessThan, L"LessThan"},
      {BinaryOp::LessEqual, L"LessEqual"},
      {BinaryOp::GreaterThan, L"GreaterThan"},
      {BinaryOp::GreaterEqual, L"GreaterEqual"},
      {BinaryOp::Equal, L"Equal"},
      {BinaryOp::NotEqual, L"NotEqual"},
      {BinaryOp::And, L"And"},
      {BinaryOp::Xor, L"Xor"},
      {BinaryOp::Or, L"Or"},
      {BinaryOp::LogicalAnd, L"LogicalAnd"},
      {BinaryOp::LogicalOr, L"LogicalOr"}};

  BinaryOpNode::BinaryOpNode(BinaryOp op, std::unique_ptr<ExpressionNode> lhs,
                             std::unique_ptr<ExpressionNode> rhs,
                             ValueType type)
      : ExpressionNode(type), m_op(op), m_lhs(std::move(lhs)),
        m_rhs(std::move(rhs))
  {
  }

  std::wstring BinaryOpNode::PrettyPrint(int level) const
  {
    std::wstring result = ExpressionNode::PrettyPrint(level);
    result += L"BinaryOpNode(" + BinaryOperationNames.at(m_op) + L")\n";
    result += m_lhs->PrettyPrint(level + 1);
    result += m_rhs->PrettyPrint(level + 1);
    return result;
  }

  std::unique_ptr<ExpressionNode> BinaryOpNode::Parse(
      std::unique_ptr<ExpressionNode> base, CLangLexer &lexer)
  {
    CLangToken opToken = lexer.GetToken();
    BinaryOp op = BinaryOperations.at(opToken.type);

    std::unique_ptr<ExpressionNode> rhs =
        ExpressionNode::Parse(lexer, BinaryOperationPrecedence.at(op));

    // TODO: Implement casting.

    return std::make_unique<BinaryOpNode>(op, std::move(base), std::move(rhs),
                                          base->m_type);
  }
} // namespace Vypr