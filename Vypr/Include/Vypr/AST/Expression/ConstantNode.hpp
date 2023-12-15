#pragma once

#include <memory>
#include <string>
#include <variant>

#include "Vypr/AST/Expression/ExpressionNode.hpp"

namespace Vypr
{
  using ConstantValue = std::variant<int32_t, int64_t, uint8_t, uint32_t,
                                     uint64_t, float, double, std::string>;

  class CLangLexer;
  class CLangToken;
  class ConstantNode : public ExpressionNode
  {
  public:
    /// <summary>
    /// Construct a node containing a numeric constant, string literal, or
    /// character literal.
    /// </summary>
    /// <param name="type">Type of the constant being stored.</param>
    /// <param name="value">Value of the constant being stored.</param>
    /// <param name="column">
    /// Column in the source file where the constant
    /// starts.
    /// </param>
    /// <param name="line">
    /// Line in the source file where the
    /// constant starts.
    /// </param>
    ConstantNode(std::unique_ptr<StorageType> &type, ConstantValue value,
                 size_t column, size_t line);

    /// <summary>
    /// Print information about the constant.
    /// </summary>
    /// <param name="level">Indentation level.</param>
    /// <returns>String containing information about the constant.</returns>
    std::wstring PrettyPrint(int level) const override;

    /// <summary>
    /// Generate temporary variable for this constant in IR structure.
    /// </summary>
    /// <param name="context">Compiler context</param>
    /// <returns>
    /// Reference to the value of the constant in the IR
    /// structure.
    /// </returns>
    llvm::Value *GenerateCode(Context &context) const override;

    /// <summary>
    /// Parse the constant from the lexer and convert any numerics to their bit
    /// representations.
    /// </summary>
    /// <param name="lexer">Lexer with a constant token on the front.</param>
    /// <returns>Constant node that was constructed.</returns>
    static std::unique_ptr<ConstantNode> Parse(CLangLexer &lexer);

  private:
    static std::unique_ptr<ConstantNode> ParseIntegerConstant(
        const CLangToken &token);

    static std::unique_ptr<ConstantNode> ParseFloatConstant(
        const CLangToken &token);

    static std::unique_ptr<ConstantNode> ParseStringLiteral(
        const CLangToken &token);

    ConstantValue m_value;
  };
} // namespace Vypr