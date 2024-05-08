#pragma once

#include "Vypr/AST/SymbolTable.hpp"

namespace Vypr
{
  // @todo This should all be in a separate file I think.

  /// @brief User defined type. Holds the member variables and information about
  /// a type defined in the current scope.
  ///
  /// @todo This should probably be a `StorageType`. This can't be a variant
  /// because at some point UnionUDT and StructUDT might have to reference
  /// themselves.
  struct UserDefinedType
  {
    enum UDTMetaType
    {
      Struct,
      Union,
      Enum
    };

    ~UserDefinedType(){};
  };

  /// @brief A union type for memory locations that can interpreted in multiple
  /// ways.
  /// @todo Implement member table.
  struct UnionUDT : public UserDefinedType
  {
    ~UnionUDT()
    {
    }
  };

  /// @brief A struct type for multi-variable objects.
  /// @todo Implement member table.
  /// @todo Implement bitfields.
  struct StructUDT : public UserDefinedType
  {
    ~StructUDT()
    {
    }
  };

  struct EnumUDT : public UserDefinedType
  {
    ~EnumUDT()
    {
    }
  };

  /**
   * @brief Global information needed during the creation of the abstract
   * syntax tree.
   */
  struct ASTContext
  {
    /**
     * @brief Table of symbol names to the types they represent. This applies
     * to any variables and functions.
     */
    TypeTable typeTable;

    /**
     * @brief Table of user defined types to the structures they represent.
     */
    SymbolTable<UserDefinedType> userDefinedTypeTable;

    /**
     * @brief Retrieves the type associated with a symbol.
     * @param symbol Name of the symbol that has an associated type.
     * @returns The type of the symbol if it exists and `nullptr` otherwise.
     */
    std::shared_ptr<StorageType> GetSymbolType(const std::wstring &symbol) const
    {
      // @todo Use the user-defined type table to decipher types.
      return typeTable.GetSymbol(symbol);
    }
  };
} // namespace Vypr
