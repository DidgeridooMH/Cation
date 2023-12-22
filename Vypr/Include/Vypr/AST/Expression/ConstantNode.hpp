#pragma once

#include <memory>
#include <string>
#include <variant>

#include "Vypr/AST/Expression/ExpressionNode.hpp"

namespace Vypr
{
  /// @brief Union of all possible constant types.
  using ConstantValue = std::variant<int32_t, int64_t, uint8_t, uint32_t,
                                     uint64_t, float, double, std::string>;

  class CLangLexer;
  class CLangToken;
  /// @brief Parse-tree node that denotes a numeric constant, string literal, or
  /// character literal.
  class ConstantNode : public ExpressionNode
  {
  public:
    /// @brief Construct a node containing a numeric constant, string literal,
    /// or character literal.
    ///
    /// @param type Type of the constant being stored.
    /// @param value Value of the constant being stored.
    /// @param column Column in the source file where the constant
    /// starts.
    ///
    /// @param line Line in the source file where the
    /// constant starts.
    ConstantNode(std::unique_ptr<StorageType> &&type, ConstantValue value,
                 size_t column, size_t line);

    /// @brief Print information about the constant.
    ///
    /// @param level Indentation level.
    /// @returns String containing information about the constant.
    std::wstring PrettyPrint(int level) const override;

    /// @brief Generate temporary variable for this constant in IR structure.
    ///
    /// @param context Compiler context
    /// @returns Reference to the value of the constant in the IR
    /// structure.
    llvm::Value *GenerateCode(Context &context) const override;

    /// @brief Parse the constant from the lexer and convert any numerics to
    /// their bit representations.
    ///
    /// @param lexer Lexer with a constant token on the front.
    /// @returns Constant node that was constructed.
    static std::unique_ptr<ConstantNode> Parse(CLangLexer &lexer);

  private:
    static std::unique_ptr<ConstantNode> ParseIntegerConstant(
        const CLangToken &token);

    static std::unique_ptr<ConstantNode> ParseFloatConstant(
        const CLangToken &token);

    static std::unique_ptr<ConstantNode> ParseStringLiteral(
        const CLangToken &token, CLangLexer &lexer);

    ConstantValue m_value;
  };
} // namespace Vypr