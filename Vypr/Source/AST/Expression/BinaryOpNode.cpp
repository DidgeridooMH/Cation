#include "Vypr/AST/Expression/BinaryOpNode.hpp"

#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Expression/CastNode.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/PointerType.hpp"

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

  BinaryOpNode::BinaryOpNode(BinaryOp op, std::unique_ptr<ExpressionNode> &lhs,
                             std::unique_ptr<ExpressionNode> &rhs,
                             size_t column, size_t line)
      : ExpressionNode(nullptr, column, line), m_op(op), m_lhs(std::move(lhs)),
        m_rhs(std::move(rhs))
  {
    type = m_lhs->type->Check(op, *m_rhs->type);
    if (type == nullptr)
    {
      throw CompileError(CompileErrorId::InvalidOperands, column, line);
    }
    CastType();
  }

  void BinaryOpNode::CastType()
  {
    switch (m_op)
    {
    case BinaryOp::Add:
    case BinaryOp::Subtract:
    case BinaryOp::Multiply:
    case BinaryOp::Divide:
    case BinaryOp::Modulo:
      CastTypeArithmetic(m_op);
      break;
    case BinaryOp::And:
    case BinaryOp::Xor:
    case BinaryOp::Or:
      CastTypeBitwise(m_op);
      break;
    case BinaryOp::LessThan:
    case BinaryOp::LessEqual:
    case BinaryOp::GreaterThan:
    case BinaryOp::GreaterEqual:
    case BinaryOp::Equal:
    case BinaryOp::NotEqual:
      CastTypeComparison(m_op);
      break;
    case BinaryOp::LogicalAnd:
    case BinaryOp::LogicalOr:
      CastTypeLogic(m_op);
      break;
    default:
      break;
    }
  }

  void BinaryOpNode::CastTypeIntegral()
  {
    IntegralType *exprType = dynamic_cast<IntegralType *>(type.get());
    IntegralType *lhs = dynamic_cast<IntegralType *>(m_lhs->type.get());

    if (lhs->integral < exprType->integral ||
        lhs->isUnsigned != exprType->isUnsigned)
    {
      std::unique_ptr<StorageType> castType = exprType->Clone();
      m_lhs = std::make_unique<CastNode>(castType, m_lhs);
    }

    IntegralType *rhs = dynamic_cast<IntegralType *>(m_rhs->type.get());
    if (rhs->integral < exprType->integral ||
        rhs->isUnsigned != exprType->isUnsigned)
    {
      std::unique_ptr<StorageType> castType = exprType->Clone();
      m_rhs = std::make_unique<CastNode>(castType, m_rhs);
    }
  }

  void BinaryOpNode::CastTypeIntegralComparison()
  {
    IntegralType *lhs = dynamic_cast<IntegralType *>(m_lhs->type.get());
    IntegralType *rhs = dynamic_cast<IntegralType *>(m_rhs->type.get());

    if (lhs->integral < rhs->integral)
    {
      std::unique_ptr<StorageType> castType = m_rhs->type->Clone();
      dynamic_cast<IntegralType *>(castType.get())->isUnsigned |=
          rhs->isUnsigned;
      m_lhs = std::make_unique<CastNode>(castType, m_lhs);
    }

    if (rhs->integral < lhs->integral)
    {
      std::unique_ptr<StorageType> castType = lhs->Clone();
      dynamic_cast<IntegralType *>(castType.get())->isUnsigned |=
          rhs->isUnsigned;
      m_rhs = std::make_unique<CastNode>(castType, m_rhs);
    }
  }

  void BinaryOpNode::CastTypePointer()
  {
    if (m_lhs->type->GetType() == StorageMetaType::Array)
    {
      std::unique_ptr<StorageType> castType = m_lhs->type->Clone();
      m_lhs = std::make_unique<CastNode>(castType, m_lhs);
    }
    else if (m_lhs->type->GetType() != StorageMetaType::Pointer)
    {
      std::unique_ptr<StorageType> voidType = std::make_unique<StorageType>();
      std::unique_ptr<StorageType> castType =
          std::make_unique<PointerType>(voidType, false, false);
      m_lhs = std::make_unique<CastNode>(castType, m_lhs);
    }

    if (m_rhs->type->GetType() == StorageMetaType::Array)
    {
      std::unique_ptr<StorageType> castType = m_rhs->type->Clone();
      m_rhs = std::make_unique<CastNode>(castType, m_rhs);
    }
    else if (m_rhs->type->GetType() != StorageMetaType::Pointer)
    {
      std::unique_ptr<StorageType> voidType = std::make_unique<StorageType>();
      std::unique_ptr<StorageType> castType =
          std::make_unique<PointerType>(voidType, false, false);
      m_rhs = std::make_unique<CastNode>(castType, m_rhs);
    }
  }

  void BinaryOpNode::CastTypeArithmetic(BinaryOp op)
  {
    switch (type->GetType())
    {
    case StorageMetaType::Integral:
      CastTypeIntegral();
      break;
    default:
      break;
    }
  }

  void BinaryOpNode::CastTypeBitwise(BinaryOp op)
  {
    CastTypeIntegral();
  }

  // TODO: This will always be bool since comparisons are bool
  void BinaryOpNode::CastTypeComparison(BinaryOp op)
  {
    switch (type->GetType())
    {
    case StorageMetaType::Integral:
      CastTypeIntegralComparison();
      break;
    case StorageMetaType::Array:
    case StorageMetaType::Pointer:
      CastTypePointer();
      break;
    default:
      break;
    }
  }

  void BinaryOpNode::CastTypeLogic(BinaryOp op)
  {
    if (m_lhs->type->GetType() != StorageMetaType::Integral ||
        dynamic_cast<IntegralType *>(m_lhs->type.get())->integral !=
            Integral::Bool)
    {
      std::unique_ptr<StorageType> lhsType =
          std::make_unique<IntegralType>(Integral::Bool, false, false, false);
      m_lhs = std::make_unique<CastNode>(lhsType, m_lhs);
    }

    if (m_rhs->type->GetType() != StorageMetaType::Integral ||
        dynamic_cast<IntegralType *>(m_rhs->type.get())->integral !=
            Integral::Bool)
    {
      std::unique_ptr<StorageType> rhsType =
          std::make_unique<IntegralType>(Integral::Bool, false, false, false);
      m_rhs = std::make_unique<CastNode>(rhsType, m_rhs);
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
      std::unique_ptr<ExpressionNode> &base, CLangLexer &lexer,
      TypeTable &symbolTable)
  {
    CLangToken opToken = lexer.GetToken();
    BinaryOp op = BinaryOperations.at(opToken.type);

    std::unique_ptr<ExpressionNode> rhs = ExpressionNode::Parse(
        lexer, symbolTable, BinaryOperationPrecedence.at(op));
    if (rhs == nullptr)
    {
      throw CompileError(CompileErrorId::ExpectedExpression, opToken.column,
                         opToken.line);
    }

    return std::make_unique<BinaryOpNode>(op, base, rhs, opToken.column,
                                          opToken.line);
  }

  llvm::Value *BinaryOpNode::GenerateCode(Context &context) const
  {
    llvm::Value *lhs = m_lhs->GenerateCode(context);
    llvm::Value *rhs = m_rhs->GenerateCode(context);

    llvm::Value *result = nullptr;
    switch (m_op)
    {
    case BinaryOp::Add:
      result = context.builder.CreateAdd(lhs, rhs);
      break;
    case BinaryOp::Subtract:
      result = context.builder.CreateSub(lhs, rhs);
      break;
    case BinaryOp::Multiply:
      result = context.builder.CreateMul(lhs, rhs);
      break;
    case BinaryOp::Divide:
      if (type->GetType() == StorageMetaType::Integral)
      {
        if (dynamic_cast<IntegralType *>(type.get())->isUnsigned)
        {
          result = context.builder.CreateUDiv(lhs, rhs);
        }
        else
        {
          result = context.builder.CreateSDiv(lhs, rhs);
        }
      }
      else
      {
        result = context.builder.CreateFDiv(lhs, rhs);
      }
      break;
    case BinaryOp::Modulo:
      if (dynamic_cast<IntegralType *>(type.get())->isUnsigned)
      {
        result = context.builder.CreateURem(lhs, rhs);
      }
      else
      {
        result = context.builder.CreateSRem(lhs, rhs);
      }
      break;
    case BinaryOp::LogicalAnd:
    case BinaryOp::And:
      result = context.builder.CreateAnd(lhs, rhs);
      break;
    case BinaryOp::Xor:
      result = context.builder.CreateXor(lhs, rhs);
      break;
    case BinaryOp::LogicalOr:
    case BinaryOp::Or:
      result = context.builder.CreateOr(lhs, rhs);
      break;
    case BinaryOp::LessThan:
      if (type->GetType() == StorageMetaType::Integral)
      {
        if (dynamic_cast<IntegralType *>(type.get())->isUnsigned)
        {
          result = context.builder.CreateICmpSLT(lhs, rhs);
        }
        else
        {
          result = context.builder.CreateICmpULT(lhs, rhs);
        }
      }
      else
      {
        result = context.builder.CreateFCmpOLT(lhs, rhs);
      }
      break;
    case BinaryOp::LessEqual:
      if (type->GetType() == StorageMetaType::Integral)
      {
        if (dynamic_cast<IntegralType *>(type.get())->isUnsigned)
        {
          result = context.builder.CreateICmpSLE(lhs, rhs);
        }
        else
        {
          result = context.builder.CreateICmpULE(lhs, rhs);
        }
      }
      else
      {
        result = context.builder.CreateFCmpOLE(lhs, rhs);
      }
      break;
    case BinaryOp::GreaterThan:
      if (type->GetType() == StorageMetaType::Integral)
      {
        if (dynamic_cast<IntegralType *>(type.get())->isUnsigned)
        {
          result = context.builder.CreateICmpSGT(lhs, rhs);
        }
        else
        {
          result = context.builder.CreateICmpUGT(lhs, rhs);
        }
      }
      else
      {
        result = context.builder.CreateFCmpOGT(lhs, rhs);
      }
      break;
    case BinaryOp::GreaterEqual:
      if (type->GetType() == StorageMetaType::Integral)
      {
        if (dynamic_cast<IntegralType *>(type.get())->isUnsigned)
        {
          result = context.builder.CreateICmpSGE(lhs, rhs);
        }
        else
        {
          result = context.builder.CreateICmpUGE(lhs, rhs);
        }
      }
      else
      {
        result = context.builder.CreateFCmpOGE(lhs, rhs);
      }
      break;
    case BinaryOp::Equal:
      if (type->GetType() == StorageMetaType::Integral)
      {
        result = context.builder.CreateICmpEQ(lhs, rhs);
      }
      else
      {
        result = context.builder.CreateFCmpOEQ(lhs, rhs);
      }
      break;
    case BinaryOp::NotEqual:
      if (type->GetType() == StorageMetaType::Integral)
      {
        result = context.builder.CreateICmpNE(lhs, rhs);
      }
      else
      {
        result = context.builder.CreateFCmpONE(lhs, rhs);
      }
      break;
    default:
      break;
    }

    return result;
  }
} // namespace Vypr