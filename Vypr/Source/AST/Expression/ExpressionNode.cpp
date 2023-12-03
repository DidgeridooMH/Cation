#include "Vypr/AST/Expression/ExpressionNode.hpp"

#include "Vypr/AST/Expression/BinaryOpNode.hpp"
#include "Vypr/AST/Expression/ConstantNode.hpp"

namespace Vypr
{
  ExpressionNode::ExpressionNode(ValueType type) : m_type(type)
  {
  }

  std::wstring ExpressionNode::PrettyPrint(int level) const
  {
    std::wstring result;
    for (int i = 0; i < level; i++)
    {
      result += L"  ";
    }
    result += L"|> " + m_type.PrettyPrint() + L" ";
    return result;
  }

  // TODO: Prefix/postfix yikes
  std::unique_ptr<ExpressionNode> ExpressionNode::Parse(CLangLexer &lexer,
                                                        int precedenceLevel)
  {
    std::unique_ptr<ExpressionNode> base;

    CLangToken nextToken = lexer.PeekToken();
    switch (nextToken.type)
    {
    case CLangTokenType::IntegerConstant:
    case CLangTokenType::FloatConstant:
    case CLangTokenType::CharacterConstant:
    case CLangTokenType::StringLiteral:
      base = ConstantNode::Parse(lexer);
      break;
    case CLangTokenType::Identifier:
      // TODO: Variable node.
      break;
    }

    nextToken = lexer.PeekToken();
    while (BinaryOperations.contains(nextToken.type) &&
           precedenceLevel >= BinaryOperationPrecedence.at(
                                  BinaryOperations.at(nextToken.type)))
    {
      base = BinaryOpNode::Parse(std::move(base), lexer);
      nextToken = lexer.PeekToken();
    }

    return base;
  }
} // namespace Vypr