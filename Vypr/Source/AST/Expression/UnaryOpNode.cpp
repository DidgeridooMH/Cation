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
                           std::unique_ptr<ExpressionNode> expression,
                           ValueType type)
      : ExpressionNode(type), m_op(op), m_expression(std::move(expression))
  {
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

    // TODO: Implement casting.

    return std::make_unique<UnaryOpNode>(op, std::move(expression),
                                         expression->m_type);
  }
} // namespace Vypr