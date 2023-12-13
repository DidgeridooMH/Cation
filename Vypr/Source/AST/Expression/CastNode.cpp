#include "Vypr/AST/Expression/CastNode.hpp"

#include "Vypr/AST/Type/IntegralType.hpp"

namespace Vypr
{
  CastNode::CastNode(std::unique_ptr<StorageType> &castType,
                     std::unique_ptr<ExpressionNode> &expression)
      : ExpressionNode(std::move(castType), expression->column,
                       expression->line),
        expression(std::move(expression))
  {
  }

  std::wstring CastNode::PrettyPrint(int level) const
  {
    std::wstring result = ExpressionNode::PrettyPrint(level);
    result += L"CastNode\n";
    result += expression->PrettyPrint(level + 1);
    return result;
  }

  llvm::Value *CastNode::GenerateCode(Context &context) const
  {
    llvm::Value *resultValue = expression->GenerateCode(context);

    switch (type->GetType())
    {
    case StorageMetaType::Integral:
      resultValue = CastToIntegral(context, resultValue);
      break;
    default:
      // TODO:
      break;
    }

    return resultValue;
  }

  llvm::Value *CastNode::CastToIntegral(Context &context,
                                        llvm::Value *childExpression) const
  {
    llvm::Value *result = childExpression;

    switch (expression->type->GetType())
    {
    case StorageMetaType::Integral:
      result = CastIntegralToIntegral(context, childExpression);
      break;
    case StorageMetaType::Pointer:
      // TODO: After adding variable node.
      break;
    case StorageMetaType::Array:
      // TODO: Throw an error.
      break;
    case StorageMetaType::Real:
      // TODO
      break;
    default:
      // TODO:
      break;
    }

    return result;
  }

  llvm::Value *CastNode::CastIntegralToIntegral(
      Context &context, llvm::Value *childExpression) const
  {
    static const std::unordered_map<Integral,
                                    llvm::IntegerType *(llvm::IRBuilder<>::*)()>
        TypeBuilders = {{Integral::Byte, &llvm::IRBuilder<>::getInt8Ty},
                        {Integral::Short, &llvm::IRBuilder<>::getInt16Ty},
                        {Integral::Int, &llvm::IRBuilder<>::getInt32Ty},
                        {Integral::Long, &llvm::IRBuilder<>::getInt64Ty}};

    auto integralType = dynamic_cast<IntegralType *>(type.get());
    auto expressionType = dynamic_cast<IntegralType *>(expression->type.get());

    llvm::Value *result = childExpression;
    if (integralType->integral == Integral::Bool)
    {
      result = context.builder.CreateICmpNE(
          childExpression,
          llvm::ConstantInt::get(context.builder.getInt1Ty(), 0));
    }
    else if (integralType->isUnsigned)
    {
      result = context.builder.CreateZExtOrTrunc(
          childExpression,
          (context.builder.*(TypeBuilders.at(integralType->integral)))());
    }
    else
    {
      result = context.builder.CreateSExtOrTrunc(
          childExpression,
          (context.builder.*(TypeBuilders.at(integralType->integral)))());
    }
    return result;
  }
} // namespace Vypr