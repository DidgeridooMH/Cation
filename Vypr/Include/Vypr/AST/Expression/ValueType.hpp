#pragma once

#include <string>
#include <vector>

namespace Vypr
{
  enum class PrimitiveValueType
  {
    Void,
    Byte,
    Short,
    Int,
    Long,
    UByte,
    UShort,
    UInt,
    ULong,
    Float,
    Double,
    Struct
  };

  struct ValueType
  {
    bool constant = false;
    std::vector<bool> indirection;
    PrimitiveValueType type = PrimitiveValueType::Void;
    std::wstring userType;

    std::wstring PrettyPrint() const;
  };
} // namespace Vypr