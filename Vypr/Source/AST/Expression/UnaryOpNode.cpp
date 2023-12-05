#include "Vypr/AST/Expression/UnaryOpNode.hpp"

namespace Vypr
{
  const std::unordered_map<CLangTokenType, UnaryOp> UnaryOperations = {
      {CLangTokenType::Increment, UnaryOp::Increment},
      {CLangTokenType::Decrement, UnaryOp::Decrement},
      {CLangTokenType::Subtract, UnaryOp::Negate},
      {CLangTokenType::Exclamation, UnaryOp::LogicalNot},
      {CLangTokenType::Tilde, UnaryOp::Not},
      {CLangTokenType::Star, UnaryOp::Deref},
      {CLangTokenType::And, UnaryOp::AddressOf},
      {CLangTokenType::Sizeof, UnaryOp::Sizeof}};

  const std::unordered_map<UnaryOp, std::wstring> BinaryOperationNames = {
      {UnaryOp::Increment, L"Increment"},
      {UnaryOp::Decrement, L"Decrement"},
      {UnaryOp::Negate, L"Negate"},
      {UnaryOp::LogicalNot, L"LogicalNot"},
      {UnaryOp::Not, L"Not"},
      {UnaryOp::Deref, L"Deref"},
      {UnaryOp::AddressOf, L"AddressOf"},
      {UnaryOp::Sizeof, L"Sizeof"}};

  UnaryOpNode::UnaryOpNode(UnaryOp op,
                           std::unique_ptr<ExpressionNode> expression)
      : m_op(op), m_expression(std::move(expression))
  {
    switch (op)
    {
    case UnaryOp::Increment:
    case UnaryOp::Decrement:
      if (!m_expression->type.IsModifiable())
      {
        throw TypeException("Value must be modifiable.");
      }
    case UnaryOp::Negate:
      if (m_expression->type.metaType != ValueMetaType::Primitive &&
          m_expression->type.indirection.size() == 0)
      {
        throw TypeException("Value must be integral.");
      }
      type = m_expression->type;
      type.constant = false;
      type.lvalue = false;
      break;
    case UnaryOp::LogicalNot:
      if (m_expression->type.metaType != ValueMetaType::Primitive ||
          m_expression->type.indirection.size() == 0)
      {
        throw TypeException("Value must be integral.");
      }
      type = ValueType{.metaType = ValueMetaType::Primitive,
                       .lvalue = false,
                       .constant = false,
                       .info = PrimitiveType::Int};
      break;
    case UnaryOp::Not:
      if (m_expression->type.indirection.size() > 0 ||
          m_expression->type.metaType != ValueMetaType::Primitive ||
          (std::get<PrimitiveType>(m_expression->type.info) ==
               PrimitiveType::Float ||
           std::get<PrimitiveType>(m_expression->type.info) ==
               PrimitiveType::Double))
      {
        throw TypeException("Value must be integral.");
      }
      type = m_expression->type;
      type.constant = false;
      type.lvalue = false;
      break;
    case UnaryOp::Deref:
      if (m_expression->type.indirection.size() == 0)
      {
        throw TypeException("Value must be a pointer.");
      }
      type = m_expression->type;
      type.indirection.pop_back();
      break;
    case UnaryOp::AddressOf:
      type = m_expression->type;
      type.indirection.push_back(false);
      break;
    case UnaryOp::Sizeof:
      type = ValueType{.metaType = ValueMetaType::Primitive,
                       .lvalue = false,
                       .constant = false,
                       .info = PrimitiveType::ULong};
      break;
    }
  }

  std::wstring UnaryOpNode::PrettyPrint(int level) const
  {
    std::wstring result = ExpressionNode::PrettyPrint(level);
    result += L"UnaryOpNode(" + BinaryOperationNames.at(m_op) + L")\n";
    result += m_expression->PrettyPrint(level + 1);
    return result;
  }

  std::unique_ptr<ExpressionNode> UnaryOpNode::Parse(CLangLexer &lexer)
  {
    CLangToken opToken = lexer.GetToken();
    UnaryOp op = UnaryOperations.at(opToken.type);

    std::unique_ptr<ExpressionNode> expression =
        ExpressionNode::Parse(lexer, 1);

    return std::make_unique<UnaryOpNode>(op, std::move(expression));
  }
} // namespace Vypr