#include "Vypr/AST/SymbolTable.hpp"

#include <llvm/IR/Value.h>

#include "Vypr/AST/ASTContext.hpp"
#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  template <>
  std::shared_ptr<StorageType> SymbolTable<
      std::shared_ptr<StorageType>>::GetSymbol(const std::wstring &symbol) const
  {
    for (auto table = m_tables.rbegin(); table != m_tables.rend(); table++)
    {
      if (table->contains(symbol))
      {
        return table->at(symbol)->Clone();
      }
    }
    return nullptr;
  }

  template class SymbolTable<std::shared_ptr<StorageType>>;
} // namespace Vypr
