#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  template <typename T> class SymbolTable
  {
  public:
    void PushScope();

    void PopScope();

    bool IsDuplicate(const std::wstring &symbol) const;

    void AddSymbol(const std::wstring &symbol, T type);

    T GetSymbol(const std::wstring &symbol) const;

  private:
    std::vector<std::unordered_map<std::wstring, T>> m_tables;
  };

  using TypeTable = SymbolTable<std::shared_ptr<StorageType>>;
} // namespace Vypr