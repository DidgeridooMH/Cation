#include "Vypr/AST/Expression/ExpressionNode.hpp"

#include "Vypr/AST/Expression/BinaryOpNode.hpp"
#include "Vypr/AST/Expression/ConstantNode.hpp"
#include "Vypr/AST/Expression/PostfixOpNode.hpp"
#include "Vypr/AST/Expression/UnaryOpNode.hpp"
#include "Vypr/AST/UnexpectedTokenException.hpp"

namespace Vypr
{

  ExpressionNode::ExpressionNode() : column(0), line(0)
  {
  }

  ExpressionNode::ExpressionNode(std::unique_ptr<StorageType> &&type,
                                 size_t column, size_t line)
      : type(std::move(type)), column(column), line(line)
  {
  }

  std::wstring ExpressionNode::PrettyPrint(int level) const
  {
    std::wstring result;
    for (int i = 0; i < level; i++)
    {
      result += L"  ";
    }
    result += L"|> <" + type->PrettyPrint() + L"> ";
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
      case CLangTokenType::LeftParenthesis: {
        lexer.GetToken();
        base = ExpressionNode::Parse(lexer);
        CLangToken groupClose = lexer.GetToken();
        if (groupClose.type != CLangTokenType::RightParenthesis)
        {
          throw UnexpectedTokenException(")", groupClose.line,
                                         groupClose.column);
        }
        break;
      }
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
            precedenceLevel >= 1) ||
           nextToken.type == CLangTokenType::LeftParenthesis ||
           nextToken.type == CLangTokenType::RightParenthesis)
    {
      if (nextToken.type == CLangTokenType::Increment)
      {
        lexer.GetToken();
        base = std::make_unique<PostfixOpNode>(
            PostfixOp::Increment, base, nextToken.column, nextToken.line);
      }
      else if (nextToken.type == CLangTokenType::Decrement)
      {
        lexer.GetToken();
        base = std::make_unique<PostfixOpNode>(
            PostfixOp::Decrement, base, nextToken.column, nextToken.line);
      }
      else if (nextToken.type == CLangTokenType::RightParenthesis)
      {
        break;
      }
      else
      {
        base = BinaryOpNode::Parse(base, lexer);
      }
      nextToken = lexer.PeekToken();
    }

    return base;
  }
} // namespace Vypr