#pragma once

#include <string>
#include <vector>

namespace Vypr
{
  struct TypeException : public std::exception
  {
    TypeException(std::string message) : message(message)
    {
    }

    const char *what() const noexcept override
    {
      return message.c_str();
    }

    std::string message;
  };

  enum class ValueMetaType
  {
    Primitive,
    UserDefined,
    Function,
    Void
  };

  enum class PrimitiveType
  {
    Byte,
    Short,
    Int,
    Long,
    UByte,
    UShort,
    UInt,
    ULong,
    Float,
    Double
  };

  struct ValueType;
  struct FunctionType
  {
    std::shared_ptr<ValueType> returnType;
    std::vector<std::shared_ptr<ValueType>> arguments;
  };

  struct ValueType
  {
    ValueMetaType metaType;
    bool lvalue;
    bool constant;
    std::vector<bool> indirection;
    std::variant<PrimitiveType, std::wstring, FunctionType> info;

    std::wstring PrettyPrint() const;

    inline bool IsModifiable() const
    {
      return lvalue && !constant;
    }
  };
} // namespace Vypr