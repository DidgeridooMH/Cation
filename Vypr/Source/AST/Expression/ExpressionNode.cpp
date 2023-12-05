#include "Vypr/AST/Expression/ExpressionNode.hpp"

#include "Vypr/AST/Expression/BinaryOpNode.hpp"
#include "Vypr/AST/Expression/ConstantNode.hpp"
#include "Vypr/AST/Expression/PostfixOpNode.hpp"
#include "Vypr/AST/Expression/UnaryOpNode.hpp"

namespace Vypr
{
  ExpressionNode::ExpressionNode(ValueType type) : type(type)
  {
  }

  std::wstring ExpressionNode::PrettyPrint(int level) const
  {
    std::wstring result;
    for (int i = 0; i < level; i++)
    {
      result += L"  ";
    }
    result += L"|> " + type.PrettyPrint() + L" ";
    return result;
  }

  std::unique_ptr<ExpressionNode> ExpressionNode::Parse(CLangLexer &lexer,
                                                        int precedenceLevel)
  {
    std::unique_ptr<ExpressionNode> base;

    CLangToken nextToken = lexer.PeekToken();
    if (UnaryOperations.contains(nextToken.type))
    {
      base = UnaryOpNode::Parse(lexer);
    }
    else
    {
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
      default:
        break;
      }
    }

    nextToken = lexer.PeekToken();
    while ((BinaryOperations.contains(nextToken.type) &&
            precedenceLevel >= BinaryOperationPrecedence.at(
                                   BinaryOperations.at(nextToken.type))) ||
           ((nextToken.type == CLangTokenType::Increment ||
             nextToken.type == CLangTokenType::Decrement) &&
            precedenceLevel >= 1))
    {
      if (nextToken.type == CLangTokenType::Increment)
      {
        lexer.GetToken();
        base = std::make_unique<PostfixOpNode>(PostfixOp::Increment,
                                               std::move(base), base->type);
      }
      else if (nextToken.type == CLangTokenType::Decrement)
      {
        lexer.GetToken();
        base = std::make_unique<PostfixOpNode>(PostfixOp::Decrement,
                                               std::move(base), base->type);
      }
      else
      {
        base = BinaryOpNode::Parse(std::move(base), lexer);
      }
      nextToken = lexer.PeekToken();
    }

    return base;
  }
} // namespace Vypr