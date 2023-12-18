#pragma once

#include <memory>

#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  /// <summary>
  /// Size information for integral types.
  /// </summary>
  enum class Integral
  {
    /// <summary>
    /// 1-bit value. Used for truthfulness where 1 is true and 0 is false.
    /// </summary>
    Bool,
    /// <summary>8-bit value</summary>
    Byte,
    /// <summary>16-bit value</summary>
    Short,
    /// <summary>32-bit value</summary>
    Int,
    /// <summary>64-bit value</summary>
    Long
  };

  /// <summary>
  /// Type that holds integer values. These values can be sized as specified by
  /// <c>Integral</c> and can either be signed or unsigned.
  /// </summary>
  class IntegralType : public StorageType
  {
  public:
    /// <summary>
    /// Constructs an integral type.
    /// </summary>
    /// <param name="integral">Size of the integer.</param>
    /// <param name="isUnsigned">True if unsigned, false otherwise</param>
    /// <param name="isConst">True if constant, false otherwise</param>
    /// <param name="isLValue">True if L-Value, false otherwise</param>
    IntegralType(Integral integral, bool isUnsigned, bool isConst,
                 bool isLValue);

    std::unique_ptr<StorageType> Clone() const override;

    std::unique_ptr<StorageType> Check(PostfixOp op) const override;

    std::unique_ptr<StorageType> Check(UnaryOp op) const override;

    std::unique_ptr<StorageType> Check(BinaryOp op,
                                       const StorageType &other) const override;

    std::wstring PrettyPrint() const override;

    /// <summary>
    /// Size of the integer.
    /// </summary>
    Integral integral;

    /// <summary>
    /// True if the type is unsigned, false otherwise.
    /// </summary>
    bool isUnsigned;

  private:
    std::unique_ptr<StorageType> CheckArithmetic(
        BinaryOp op, const StorageType &other) const;

    std::unique_ptr<StorageType> CheckBitwise(BinaryOp op,
                                              const StorageType &other) const;
  };
} // namespace Vypr