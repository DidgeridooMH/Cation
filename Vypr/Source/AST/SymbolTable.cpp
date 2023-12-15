#include "Vypr/AST/SymbolTable.hpp"

#include <llvm/IR/Instructions.h>

#include "Vypr/AST/CompileError.hpp"

namespace Vypr
{
  template <typename T> SymbolTable<T>::SymbolTable<T>()
  {
    m_tables.push_back({});
  }

  template <typename T> void SymbolTable<T>::PushScope()
  {
    m_tables.push_back({});
  }

  template <typename T> void SymbolTable<T>::PopScope()
  {
    m_tables.pop_back();
  }

  template <typename T>
  bool SymbolTable<T>::IsDuplicate(const std::wstring &symbol) const
  {
    return m_tables.back().contains(symbol);
  }

  template <typename T>
  void SymbolTable<T>::AddSymbol(const std::wstring &symbol, T type)
  {
    if (!m_tables.back().contains(symbol))
    {
      m_tables.back().insert({symbol, std::move(type)});
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
    return {};
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