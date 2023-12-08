#pragma once

#include <memory>

#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  class PointerType : public StorageType
  {
  public:
    PointerType(std::unique_ptr<StorageType> &storage, bool isConst,
                bool isLValue);

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

    std::unique_ptr<StorageType> CheckLogic(BinaryOp op,
                                            const StorageType *other) const;

    std::unique_ptr<StorageType> m_storage;
  };
} // namespace Vypr