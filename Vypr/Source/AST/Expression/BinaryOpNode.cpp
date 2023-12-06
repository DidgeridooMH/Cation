#include "Vypr/AST/Expression/BinaryOpNode.hpp"

#include "Vypr/AST/Expression/CastNode.hpp"

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
                             std::unique_ptr<ExpressionNode> rhs, size_t column,
                             size_t line)
      : ExpressionNode(lhs->type, column, line), m_op(op),
        m_lhs(std::move(lhs)), m_rhs(std::move(rhs))
  {
    if ((!m_lhs->type.IsPrimitive() && !m_lhs->type.IsPointer()) ||
        (!m_rhs->type.IsPrimitive() && !m_rhs->type.IsPointer()))
    {
      throw TypeException("Operands must be pointer, real, or integral types.",
                          column, line);
    }

    switch (op)
    {
    case BinaryOp::Modulo:
    case BinaryOp::ShiftLeft:
    case BinaryOp::ShiftRight:
    case BinaryOp::And:
    case BinaryOp::Xor:
    case BinaryOp::Or:
      if (!m_lhs->type.IsIntegral() || !m_rhs->type.IsIntegral() ||
          m_lhs->type.IsPointer() || m_rhs->type.IsPointer())
      {
        throw TypeException("Operands must be integral.", column, line);
      }
      CastType();
      break;
    case BinaryOp::Multiply:
    case BinaryOp::Divide:
      if (m_lhs->type.IsPointer() || m_rhs->type.IsPointer())
      {
        throw TypeException("Operands must be integral.", column, line);
      }
      CastType();
      break;
    case BinaryOp::Add:
    case BinaryOp::Subtract:
      if ((m_lhs->type.IsPointer() && m_rhs->type.IsPointer()) ||
          (m_lhs->type.IsPointer() && m_rhs->type.IsReal()) ||
          (m_lhs->type.IsReal() && m_rhs->type.IsPointer()))
      {
        throw TypeException("Operands must be real or integral and pointer.",
                            column, line);
      }
      CastType();
      break;
    case BinaryOp::LogicalAnd:
    case BinaryOp::LogicalOr:
    case BinaryOp::LessThan:
    case BinaryOp::LessEqual:
    case BinaryOp::GreaterThan:
    case BinaryOp::GreaterEqual:
    case BinaryOp::Equal:
    case BinaryOp::NotEqual:
      if ((m_lhs->type.IsPointer() && m_rhs->type.IsReal()) ||
          (m_lhs->type.IsReal() && m_rhs->type.IsPointer()))
      {
        throw TypeException("Operands must be real or integral and pointer.",
                            column, line);
      }
      type = ValueType{.storage = PrimitiveType::Int};
      break;
    }

    type.lvalue = false;
  }

  void BinaryOpNode::CastType()
  {
    if ((m_lhs->type.IsPointer() && m_rhs->type.IsPointer()) ||
        (m_lhs->type.IsPointer() && m_rhs->type.IsPrimitive()))
    {
      type = m_lhs->type;
    }
    else if (m_lhs->type.IsPrimitive() && m_rhs->type.IsPointer())
    {
      type = m_rhs->type;
    }
    else if (std::get<PrimitiveType>(m_lhs->type.storage) <
             std::get<PrimitiveType>(m_rhs->type.storage))
    {
      m_lhs = std::make_unique<CastNode>(m_rhs->type, std::move(m_lhs));
      type = m_lhs->type;
    }
    else if (std::get<PrimitiveType>(m_lhs->type.storage) >
             std::get<PrimitiveType>(m_rhs->type.storage))
    {
      m_rhs = std::make_unique<CastNode>(m_lhs->type, std::move(m_rhs));
      type = m_rhs->type;
    }
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

    return std::make_unique<BinaryOpNode>(op, std::move(base), std::move(rhs),
                                          opToken.column, opToken.line);
  }
} // namespace Vypr