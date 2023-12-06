#include "Vypr/AST/Expression/ValueType.hpp"

#include <unordered_map>

namespace Vypr
{

  static const std::unordered_map<PrimitiveType, std::wstring>
      PrimitiveTypeNames = {
          {PrimitiveType::Byte, L"Byte"},   {PrimitiveType::Short, L"Short"},
          {PrimitiveType::Int, L"Int"},     {PrimitiveType::Long, L"Long"},
          {PrimitiveType::UByte, L"UByte"}, {PrimitiveType::UShort, L"UShort"},
          {PrimitiveType::UInt, L"UInt"},   {PrimitiveType::ULong, L"ULong"},
          {PrimitiveType::Float, L"Float"}, {PrimitiveType::Double, L"Double"}};

  std::wstring ValueType::PrettyPrint() const
  {
    std::wstring result = L"<";
    if (constant)
    {
      result += L"const ";
    }

    if (std::holds_alternative<PrimitiveType>(storage))
    {
      result += PrimitiveTypeNames.at(std::get<PrimitiveType>(storage));
    }
    else if (std::holds_alternative<UserDefinedType>(storage))
    {
      result += std::get<UserDefinedType>(storage);
    }
    else if (std::holds_alternative<FunctionType>(storage))
    {
      result += L"FunctionPtr";
    }
    else
    {
      result += L"void";
    }

    for (bool isConstantPointer : indirection)
    {
      if (result.back() != L'*')
      {
        result += L" ";
      }
      result += L"*";

      if (isConstantPointer)
      {
        result += L" const";
      }
    }

    result += L">";
    return result;
  }
} // namespace Vypr