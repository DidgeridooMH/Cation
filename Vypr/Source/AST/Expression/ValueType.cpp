#include "Vypr/AST/Expression/ValueType.hpp"

#include <unordered_map>

namespace Vypr
{

  static const std::unordered_map<PrimitiveValueType, std::wstring>
      ValueTypeName = {{PrimitiveValueType::Void, L"Void"},
                       {PrimitiveValueType::Byte, L"Byte"},
                       {PrimitiveValueType::Short, L"Short"},
                       {PrimitiveValueType::Int, L"Int"},
                       {PrimitiveValueType::Long, L"Long"},
                       {PrimitiveValueType::UByte, L"UByte"},
                       {PrimitiveValueType::UShort, L"UShort"},
                       {PrimitiveValueType::UInt, L"UInt"},
                       {PrimitiveValueType::ULong, L"ULong"},
                       {PrimitiveValueType::Float, L"Float"},
                       {PrimitiveValueType::Double, L"Double"},
                       {PrimitiveValueType::Struct, L"Struct"}};

  std::wstring ValueType::PrettyPrint() const
  {
    const int *const *i = nullptr;

    std::wstring result = L"<";
    if (constant)
    {
      result += L"const ";
    }

    result += ValueTypeName.at(type);

    for (bool isConstantPointer : indirection)
    {
      if (result.back() == L'*')
      {
        result += L"*";
      }
      else
      {
        result += L" *";
      }

      if (isConstantPointer)
      {
        result += L" const";
      }
    }

    result += L">";
    return result;
  }
} // namespace Vypr