#include "Vypr/AST/Expression/CastNode.hpp"

#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/RealType.hpp"

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
    case StorageMetaType::Real:
      resultValue = CastToReal(context, resultValue);
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
      [[fallthrough]];
    default:
      // TODO: Void is not checked here as the implementation will change at
      // some point.
      throw CompileError(CompileErrorId::UnimplementedFeature, 0, 0);
    }

    return result;
  }

  llvm::Value *CastNode::CastToReal(Context &context,
                                    llvm::Value *childExpression) const
  {
    switch (expression->type->GetType())
    {
    case StorageMetaType::Integral:
      if (dynamic_cast<IntegralType *>(expression->type.get())->isUnsigned)
      {
        return context.builder.CreateUIToFP(childExpression,
                                            type->GetIRType(context));
      }
      return context.builder.CreateSIToFP(childExpression,
                                          type->GetIRType(context));
    case StorageMetaType::Real:
      if (dynamic_cast<RealType *>(type.get())->real <
          dynamic_cast<RealType *>(expression->type.get())->real)
      {
        return context.builder.CreateFPTrunc(childExpression,
                                             type->GetIRType(context));
      }
      return context.builder.CreateFPExt(childExpression,
                                         type->GetIRType(context));
    default:
      throw CompileError(CompileErrorId::InvalidCast, 0, 0);
    }
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