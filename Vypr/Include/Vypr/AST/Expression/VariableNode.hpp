#pragma once

#include "Vypr/AST/Expression/ExpressionNode.hpp"

namespace Vypr
{
  /// @brief Parse-tree node that denotes an access to a variable.
  ///
  /// This accesses read-write capabilities are defined in the type table using
  /// the `Parse` method.
  class VariableNode : public ExpressionNode
  {
  public:
    /// @brief Construct a node containing a symbol to a variable in the
    /// `symbolTable`.
    ///
    /// @param type Type of the symbol.
    /// @param symbol Name of the symbol.
    /// @param column Column in the source file where the symble name starts.
    /// @param line Line in the source file where the symbol name starts.
    VariableNode(std::unique_ptr<StorageType> &type, std::wstring symbol,
                 size_t column, size_t line);

    /// @brief Print information about the variable.
    ///
    /// @param level Indentation level.
    /// @returns String containing information about the variable.
    std::wstring PrettyPrint(int level) const override;

    /// @brief Generate a temporary variable for the symbol and its
    /// location in IR structure.
    ///
    /// @param context Compiler context.
    /// @returns Reference to the value/location of the variable in the
    /// IR structure.
    llvm::Value *GenerateCode(Context &context) const override;

    /// @brief Parse the variable from the lexer and retrieve its
    /// information from the typeTable.
    ///
    /// @param lexer Lexer with the variable on the front.
    /// @param symbolTable Table containing symbols as the key and type
    /// as the value.
    ///
    /// @returns Variable node that was constructed.
    static std::unique_ptr<VariableNode> Parse(CLangLexer &lexer,
                                               TypeTable &symbolTable);

  private:
    std::wstring m_symbol;
  };
} // namespace Vypr