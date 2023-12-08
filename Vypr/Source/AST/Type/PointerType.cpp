#include "Vypr/AST/Type/PointerType.hpp"

#include "Vypr/AST/Type/IntegralType.hpp"

namespace Vypr
{
  PointerType::PointerType(std::unique_ptr<StorageType> &storage, bool isConst,
                           bool isLValue)
      : StorageType(StorageMetaType::Pointer, isConst, isLValue),
        m_storage(std::move(storage))
  {
  }

  std::unique_ptr<StorageType> PointerType::Clone() const
  {
    std::unique_ptr<StorageType> storageClone = m_storage->Clone();
    return std::make_unique<PointerType>(storageClone, isConst, false);
  }

  std::unique_ptr<StorageType> PointerType::Check(PostfixOp op) const
  {
    return (isLValue && !isConst) ? Clone() : nullptr;
  }

  std::unique_ptr<StorageType> PointerType::Check(UnaryOp op) const
  {
    std::unique_ptr<StorageType> resultType;

    switch (op)
    {
    case UnaryOp::Increment:
    case UnaryOp::Decrement:
      if (isLValue && !isConst)
      {
        resultType = Clone();
      }
      break;
    case UnaryOp::LogicalNot:
      resultType =
          std::make_unique<IntegralType>(Integral::Bool, false, false, false);
      break;
    case UnaryOp::Deref:
      resultType = m_storage->Clone();
      resultType->isConst = m_storage->isConst;
      resultType->isLValue = true;
      break;
    case UnaryOp::AddressOf:
      resultType = Clone();
      resultType = std::make_unique<PointerType>(resultType, false, false);
      break;
    case UnaryOp::Sizeof:
      resultType =
          std::make_unique<IntegralType>(Integral::Long, true, false, false);
      break;
    default:
      break;
    }

    return resultType;
  }

  std::unique_ptr<StorageType> PointerType::Check(
      BinaryOp op, const StorageType *other) const
  {
    std::unique_ptr<StorageType> resultType;

    switch (op)
    {
    case BinaryOp::Add:
    case BinaryOp::Subtract:
      resultType = CheckArithmetic(op, other);
      break;
    case BinaryOp::LessThan:
    case BinaryOp::LessEqual:
    case BinaryOp::GreaterThan:
    case BinaryOp::GreaterEqual:
    case BinaryOp::Equal:
    case BinaryOp::NotEqual:
      resultType = CheckComparison(op, other);
      break;
    case BinaryOp::LogicalAnd:
    case BinaryOp::LogicalOr:
      resultType = CheckLogic(op, other);
      break;
    default:
      break;
    }

    return resultType;
  }

  std::wstring PointerType::PrettyPrint() const
  {
    std::wstring result = StorageType::PrettyPrint();
    result += L"Pointer(";
    result += m_storage->PrettyPrint();
    result += L")";
    return result;
  }

  std::unique_ptr<StorageType> PointerType::CheckArithmetic(
      BinaryOp op, const StorageType *other) const
  {
    std::unique_ptr<StorageType> resultType;

    switch (other->GetType())
    {
    case StorageMetaType::Integral:
      resultType = Clone();
      break;
    case StorageMetaType::Pointer:
    case StorageMetaType::Array:
      if (op == BinaryOp::Subtract)
      {
        resultType =
            std::make_unique<IntegralType>(Integral::Long, false, false, false);
      }
      break;
    default:
      break;
    }

    return resultType;
  }

  std::unique_ptr<StorageType> PointerType::CheckComparison(
      BinaryOp op, const StorageType *other) const
  {
    if (other->GetType() == StorageMetaType::Void)
    {
      return nullptr;
    }
    return std::make_unique<IntegralType>(Integral::Bool, false, false, false);
  }

  std::unique_ptr<StorageType> PointerType::CheckLogic(
      BinaryOp op, const StorageType *other) const
  {
    return std::make_unique<IntegralType>(Integral::Bool, false, false, false);
  }
} // namespace Vypr