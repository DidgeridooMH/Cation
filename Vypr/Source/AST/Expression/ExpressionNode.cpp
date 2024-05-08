#include "Vypr/AST/Expression/ExpressionNode.hpp"

#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Expression/BinaryOpNode.hpp"
#include "Vypr/AST/Expression/CastNode.hpp"
#include "Vypr/AST/Expression/ConstantNode.hpp"
#include "Vypr/AST/Expression/PostfixOpNode.hpp"
#include "Vypr/AST/Expression/UnaryOpNode.hpp"
#include "Vypr/AST/Expression/VariableNode.hpp"
#include "Vypr/Lexer/CLangTokenMap.hpp"

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

  llvm::Value *ExpressionNode::GenerateCode(Context &context) const
  {
    return nullptr;
  }

  std::unique_ptr<ExpressionNode> ExpressionNode::Parse(
      CLangLexer &lexer, const ASTContext &context, int precedenceLevel)
  {
    std::unique_ptr<ExpressionNode> base;

    CLangToken nextToken = lexer.PeekToken();
    if (UnaryOperations.contains(nextToken.type))
    {
      base = UnaryOpNode::Parse(lexer, context);
    }
    else
    {
      switch (nextToken.type)
      {
      case CLangTokenType::LeftParenthesis: {
        lexer.GetToken();
        if (TypePrefixSet.contains(lexer.PeekToken().type))
        {
          base = CastNode::Parse(lexer, context);
        }
        else
        {
          base = ExpressionNode::Parse(lexer, context);
          CLangToken groupClose = lexer.GetToken();
          if (groupClose.type != CLangTokenType::RightParenthesis)
          {
            throw CompileError(CompileErrorId::ExpectedGroupEnd,
                               groupClose.line, groupClose.column);
          }
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
        base = VariableNode::Parse(lexer, context);
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
            PostfixOp::Increment, std::move(base), nextToken.column,
            nextToken.line);
      }
      else if (nextToken.type == CLangTokenType::Decrement)
      {
        lexer.GetToken();
        base = std::make_unique<PostfixOpNode>(
            PostfixOp::Decrement, std::move(base), nextToken.column,
            nextToken.line);
      }
      else if (nextToken.type == CLangTokenType::RightParenthesis)
      {
        break;
      }
      else
      {
        base = BinaryOpNode::Parse(base, lexer, context);
      }
      nextToken = lexer.PeekToken();
    }

    return base;
  }
} // namespace Vypr
