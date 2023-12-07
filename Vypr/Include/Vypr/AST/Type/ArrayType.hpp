#pragma once

#include <memory>

#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  class ArrayObject : public StorageType
  {
  public:
    ArrayObject(std::unique_ptr<StorageType> &&storage, size_t size,
                bool isConst)
        : StorageType(StorageMetaType::Array, isConst), m_storage(storage),
          m_size(size)
    {
    }

  private:
    std::unique_ptr<StorageType> m_storage;
    size_t m_size;
  };
} // namespace Vypr