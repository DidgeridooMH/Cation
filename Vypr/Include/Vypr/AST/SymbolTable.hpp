#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace Vypr
{
  template <typename T> class SymbolTable
  {
  public:
    SymbolTable();

    void PushScope();

    void PopScope();

    bool IsDuplicate(const std::wstring &symbol) const;

    void AddSymbol(const std::wstring &symbol, T type);

    T GetSymbol(const std::wstring &symbol) const;

  private:
    std::vector<std::unordered_map<std::wstring, T>> m_tables;
  };

  class StorageType;
  using TypeTable = SymbolTable<std::shared_ptr<StorageType>>;
} // namespace Vypr