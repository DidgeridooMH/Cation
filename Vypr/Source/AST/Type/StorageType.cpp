#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  StorageType::StorageType() : StorageType(StorageMetaType::Void, false, false)
  {
  }

  StorageType::StorageType(StorageMetaType type, bool isConst, bool isLValue)
      : m_type(type), isConst(isConst), isLValue(isLValue)
  {
  }

  std::unique_ptr<StorageType> StorageType::Clone() const
  {
    return std::make_unique<StorageType>(m_type, isConst, isLValue);
  };

  std::unique_ptr<StorageType> StorageType::Check(PostfixOp op) const
  {
    return nullptr;
  }

  std::unique_ptr<StorageType> StorageType::Check(UnaryOp op) const
  {
    return nullptr;
  }

  std::unique_ptr<StorageType> StorageType::Check(
      BinaryOp op, const StorageType *other) const
  {
    return nullptr;
  };

  std::wstring StorageType::PrettyPrint() const
  {
    std::wstring result;
    if (isLValue)
    {
      result += L"(L) ";
    }

    if (isConst)
    {
      result += L"const ";
    }

    if (m_type == StorageMetaType::Void)
    {
      result += L"Void";
    }

    return result;
  }

  StorageMetaType StorageType::GetType() const
  {
    return m_type;
  }
} // namespace Vypr
