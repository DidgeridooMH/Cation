#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace Vypr
{
  struct TypeException : public std::exception
  {
    TypeException(std::string message, size_t column, size_t line)
        : message(message), column(column), line(line)
    {
    }

    const char *what() const noexcept override
    {
      return message.c_str();
    }

    std::string message;
    size_t column;
    size_t line;
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

  using UserDefinedType = std::wstring;

  struct ValueType
  {
    bool lvalue = false;
    bool constant = false;
    std::vector<bool> indirection;
    std::variant<PrimitiveType, UserDefinedType, FunctionType> storage;

    std::wstring PrettyPrint() const;

    inline bool IsModifiable() const
    {
      return lvalue && !constant;
    }

    inline bool IsPrimitive() const
    {
      return std::holds_alternative<PrimitiveType>(storage);
    }

    inline bool IsPointer() const
    {
      return !indirection.empty();
    }

    inline bool IsIntegral() const
    {
      auto primitiveType = std::get<PrimitiveType>(storage);
      return !IsPointer() && IsPrimitive() &&
             (primitiveType != PrimitiveType::Float &&
              primitiveType != PrimitiveType::Double);
    }

    inline bool IsReal() const
    {
      auto primitiveType = std::get<PrimitiveType>(storage);
      return !IsPointer() && IsPrimitive() &&
             (primitiveType == PrimitiveType::Float ||
              primitiveType == PrimitiveType::Double);
    }

    inline bool IsFunctionPointer() const
    {
      return std::holds_alternative<FunctionType>(storage);
    }
  };
} // namespace Vypr