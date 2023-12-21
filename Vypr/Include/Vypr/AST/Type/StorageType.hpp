#pragma once

#include <memory>
#include <string>

#include "Vypr/AST/Expression/BinaryOp.hpp"
#include "Vypr/AST/Expression/PostfixOp.hpp"
#include "Vypr/AST/Expression/UnaryOp.hpp"

namespace Vypr
{
  /// @brief Type of the type stored.
  /// @todo Remove `Void` and just have a nullptr be `void`.
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
    /// @brief Construct a `void` type that isn't constant or an l-value.
    StorageType();

    /// @brief Construct a type with the specified parameters.
    /// @param type Meta type used. This corresponds to a specific derivation of
    /// `StorageType`.
    /// @param isConst Whether the type is mutable or not.
    /// @param isLValue Whether the type describes a location in memory that can
    /// be modified.
    StorageType(StorageMetaType type, bool isConst, bool isLValue);

    /// @brief Creates a copy of the type.
    /// @return A deep copy of the `StorageType`.
    virtual std::unique_ptr<StorageType> Clone() const;

    /// @brief Checks if a postfix operation is valid on the current type.
    /// @param op Postfix operation to check.
    /// @return Type of the result of the postfix operation.
    virtual std::unique_ptr<StorageType> Check(PostfixOp op) const;

    /// @brief Checks if a unary operation is valid on the current type.
    /// @param op Unary operation to check.
    /// @return Type of the result of the unary operation.
    virtual std::unique_ptr<StorageType> Check(UnaryOp op) const;

    /// @brief Checks if a binary operation is valid on the current type and
    /// operand.
    /// @param op Binary operation to check.
    /// @param other Operand to check on right side of binary operation.
    /// @return Type of the result of the binary operation.
    virtual std::unique_ptr<StorageType> Check(BinaryOp op,
                                               const StorageType &other) const;

    /// @brief Print the type in a readable format.
    /// @return Wide string containing the readable type.
    virtual std::wstring PrettyPrint() const;

    StorageMetaType GetType() const;

    bool isConst;
    bool isLValue;

  private:
    StorageMetaType m_type;
  };
} // namespace Vypr
