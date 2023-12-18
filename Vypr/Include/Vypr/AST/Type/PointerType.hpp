#pragma once

#include <memory>

#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  /// <summary>
  /// Type that denotes a pointer to another storage type. The stored type can
  /// be any of the derivations of <c>StorageType</c>.
  /// </summary>
  class PointerType : public StorageType
  {
  public:
    /// <summary>
    /// Constructs a type that points to another type.
    /// </summary>
    /// <param name="storage">Type pointed to by this pointer type.</param>
    /// <param name="isConst">True if constant, false otherwise.</param>
    /// <param name="isLValue">True if L-Value, false otherwise.</param>
    PointerType(std::unique_ptr<StorageType> &storage, bool isConst,
                bool isLValue);

    std::unique_ptr<StorageType> Clone() const override;

    std::unique_ptr<StorageType> Check(PostfixOp op) const override;

    std::unique_ptr<StorageType> Check(UnaryOp op) const override;

    std::unique_ptr<StorageType> Check(BinaryOp op,
                                       const StorageType &other) const override;

    std::wstring PrettyPrint() const override;

  private:
    std::unique_ptr<StorageType> CheckArithmetic(
        BinaryOp op, const StorageType &other) const;

    std::unique_ptr<StorageType> m_storage;
  };
} // namespace Vypr