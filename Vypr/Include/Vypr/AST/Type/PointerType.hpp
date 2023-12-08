#pragma once

#include <memory>

#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  class PointerObject : public StorageType
  {
  public:
    PointerObject(std::unique_ptr<StorageType> &storage, bool isConst,
                  bool isLValue)
        : StorageType(StorageMetaType::Pointer, isConst, isLValue),
          m_storage(std::move(storage))
    {
    }
    std::unique_ptr<StorageType> Clone() const override;

    std::unique_ptr<StorageType> Check(PostfixOp op) const override;

    std::unique_ptr<StorageType> Check(UnaryOp op) const override;

    std::unique_ptr<StorageType> Check(BinaryOp op,
                                       const StorageType *other) const override;

    std::wstring PrettyPrint() const override;

  private:
    std::unique_ptr<StorageType> m_storage;
  };
} // namespace Vypr