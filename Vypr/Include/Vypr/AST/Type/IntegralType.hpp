#pragma once

#include <memory>

#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  enum class Integral
  {
    Bool,
    Byte,
    Short,
    Int,
    Long
  };

  class IntegralType : public StorageType
  {
  public:
    IntegralType(Integral integral, bool isUnsigned, bool isConst,
                 bool isLValue);

    std::unique_ptr<StorageType> Clone() const override;

    std::unique_ptr<StorageType> Check(PostfixOp op) const override;

    std::unique_ptr<StorageType> Check(UnaryOp op) const override;

    std::unique_ptr<StorageType> Check(BinaryOp op,
                                       const StorageType *other) const override;

    std::wstring PrettyPrint() const override;

    Integral integral;
    bool isUnsigned;

  private:
    std::unique_ptr<StorageType> CheckArithmetic(
        BinaryOp op, const StorageType *other) const;

    std::unique_ptr<StorageType> CheckBitwise(BinaryOp op,
                                              const StorageType *other) const;

    std::unique_ptr<StorageType> CheckComparison(
        BinaryOp op, const StorageType *other) const;

    std::unique_ptr<StorageType> CheckLogic(BinaryOp op,
                                            const StorageType *other) const;
  };
} // namespace Vypr