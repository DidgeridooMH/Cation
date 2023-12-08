#pragma once

#include <memory>

#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  class ArrayType : public StorageType
  {
  public:
    ArrayType(std::unique_ptr<StorageType> &storage, size_t size, int isLValue);

    std::unique_ptr<StorageType> Clone() const override;

    std::unique_ptr<StorageType> Check(PostfixOp op) const override;

    std::unique_ptr<StorageType> Check(UnaryOp op) const override;

    std::unique_ptr<StorageType> Check(BinaryOp op,
                                       const StorageType *other) const override;

    std::wstring PrettyPrint() const override;

  private:
    std::unique_ptr<StorageType> CheckArithmetic(
        BinaryOp op, const StorageType *other) const;

    std::unique_ptr<StorageType> CheckComparison(
        BinaryOp op, const StorageType *other) const;

    std::unique_ptr<StorageType> m_storage;
    size_t m_size;
  };
} // namespace Vypr