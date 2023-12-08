#include "Vypr/AST/Type/PointerType.hpp"

namespace Vypr
{
  PointerObject::PointerObject(std::unique_ptr<StorageType> &storage,
                               bool isConst, bool isLValue)
      : StorageType(StorageMetaType::Pointer, isConst, isLValue),
        m_storage(std::move(storage))
  {
  }

  std::unique_ptr<StorageType> PointerObject::Clone() const
  {
  }

  std::unique_ptr<StorageType> PointerObject::Check(PostfixOp op) const
  {
  }

  std::unique_ptr<StorageType> PointerObject::Check(UnaryOp op) const
  {
  }

  std::unique_ptr<StorageType> PointerObject::Check(
      BinaryOp op, const StorageType *other) const
  {
  }

  std::wstring PointerObject::PrettyPrint() const
  {
  }
} // namespace Vypr