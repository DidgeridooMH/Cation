#include "Vypr/AST/SymbolTable.hpp"

#include <llvm/IR/IRBuilder.h>

namespace Vypr
{
  template <typename T> void SymbolTable<T>::PushScope()
  {
    m_tables.push_back({});
  }

  template <typename T> void SymbolTable<T>::PopScope()
  {
    m_tables.pop_back();
  }

  template <typename T>
  void SymbolTable<T>::AddSymbol(const std::wstring &symbol, T type)
  {
    if (!m_tables.back().contains(symbol))
    {
      m_tables.back().insert({symbol, std::move(type)});
    }
    else
    {
      // TODO: make an actual exception.
      throw;
    }
  }

  template <typename T>
  T SymbolTable<T>::GetSymbol(const std::wstring &symbol) const
  {
    for (auto table = m_tables.rbegin(); table != m_tables.rend(); table++)
    {
      if (table->contains(symbol))
      {
        return table->at(symbol);
      }
    }
    return nullptr;
  }

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
  template class SymbolTable<llvm::AllocaInst *>;
} // namespace Vypr