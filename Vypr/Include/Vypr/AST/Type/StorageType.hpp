#pragma once

#include <memory>
#include <string>

#include "Vypr/AST/Expression/BinaryOp.hpp"
#include "Vypr/AST/Expression/PostfixOp.hpp"
#include "Vypr/AST/Expression/UnaryOp.hpp"

namespace Vypr
{
  enum class StorageMetaType
  {
    Void,
    Integral,
    Real,
    Pointer,
    Array
  };

  class StorageType
  {
  public:
    StorageType();

    StorageType(StorageMetaType type, bool isConst, bool isLValue);

    virtual std::unique_ptr<StorageType> Clone() const;

    virtual std::unique_ptr<StorageType> Check(PostfixOp op) const;

    virtual std::unique_ptr<StorageType> Check(UnaryOp op) const;

    virtual std::unique_ptr<StorageType> Check(BinaryOp op,
                                               const StorageType &other) const;

    virtual std::wstring PrettyPrint() const;

    StorageMetaType GetType() const;

    bool isConst;
    bool isLValue;

  private:
    StorageMetaType m_type;
  };
} // namespace Vypr
