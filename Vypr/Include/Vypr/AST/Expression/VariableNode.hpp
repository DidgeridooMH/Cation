#pragma once

#include "Vypr/AST/Expression/ExpressionNode.hpp"

namespace Vypr
{
  /// <summary>
  /// Parse-tree node that denotes an access to a variable. This accesses
  /// read-write capabilities are defined in the type table using the
  /// <c>Parse</c> method.
  /// </summary>
  class VariableNode : public ExpressionNode
  {
  public:
    /// <summary>
    /// Construct a node containing a symbol to a variable in the
    /// <c>symbolTable</c>.
    /// </summary>
    /// <param name="type">Type of the symbol.</param>
    /// <param name="symbol">Name of the symbol.</param>
    /// <param name="column">
    /// Column in the source file where the symble name starts.
    /// </param>
    /// <param name="line">
    /// Line in the source file where the symbol name starts.
    /// </param>
    VariableNode(std::unique_ptr<StorageType> &type, std::wstring symbol,
                 size_t column, size_t line);

    /// <summary>
    /// Print information about the variable.
    /// </summary>
    /// <param name="level">Indentation level.</param>
    /// <returns>String containing information about the variable.</returns>
    std::wstring PrettyPrint(int level) const override;

    /// <summary>
    /// Generate a temporary variable for the symbol and its location in IR
    /// structure.
    /// </summary>
    /// <param name="context">Compiler context.</param>
    /// <returns>
    /// Reference to the value/location of the variable in the IR structure.
    /// </returns>
    llvm::Value *GenerateCode(Context &context) const override;

    /// <summary>
    /// Parse the variable from the lexer and retrieve its information from the
    /// typeTable.
    /// </summary>
    /// <param name="lexer">Lexer with the variable on the front.</param>
    /// <param name="symbolTable">
    /// Table containing symbols as the key and type
    /// as the value.
    /// </param>
    /// <returns>Variable node that was constructed.</returns>
    static std::unique_ptr<VariableNode> Parse(CLangLexer &lexer,
                                               TypeTable &symbolTable);

  private:
    std::wstring m_symbol;
  };
} // namespace Vypr