#pragma once

#include <memory>

#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  class PointerObject : public StorageType
  {
  public:
    PointerObject(std::unique_ptr<StorageType> &&storage, bool isConst)
        : StorageType(StorageMetaType::Pointer, isConst),
          m_storage(std::move(storage))
    {
    }

  private:
    std::unique_ptr<StorageType> m_storage;
  };
} // namespace Vypr