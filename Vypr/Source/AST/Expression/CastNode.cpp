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
    case StorageMetaType::Array:
      // TODO: Check if this is correct. I'm not certain how array types are
      // implemented yet.
      [[fallthrough]];
    case StorageMetaType::Pointer:
      result = context.builder.CreatePtrToInt(childExpression,
                                              context.builder.getInt64Ty());
      break;
    case StorageMetaType::Real:
      break;
    default:
      // TODO: Void is not checked here as the implementation will change at
      // some point.
      break;
    }

    return result;
  }

  llvm::Value *CastNode::CastIntegralToIntegral(
      Context &context, llvm::Value *childExpression) const
  {
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
      result = context.builder.CreateZExtOrTrunc(childExpression,
                                                 type->GetIRType(context));
    }
    else
    {
      result = context.builder.CreateSExtOrTrunc(childExpression,
                                                 type->GetIRType(context));
    }
    return result;
  }
} // namespace Vypr