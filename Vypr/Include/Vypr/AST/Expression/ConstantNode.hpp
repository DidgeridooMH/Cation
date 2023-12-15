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
    ConstantNode(std::unique_ptr<StorageType> &type, ConstantValue value,
                 size_t column, size_t line);

    std::wstring PrettyPrint(int level) const override;

    llvm::Value *GenerateCode(Context &context) const override;

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