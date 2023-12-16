#pragma once

#include "Vypr/AST/Expression/ExpressionNode.hpp"

namespace Vypr
{
  class VariableNode : public ExpressionNode
  {
  public:
    VariableNode(std::unique_ptr<StorageType> &type, std::wstring symbol,
                 size_t column, size_t line);

    std::wstring PrettyPrint(int level) const override;

    llvm::Value *GenerateCode(Context &context) const override;

    static std::unique_ptr<VariableNode> Parse(CLangLexer &lexer,
                                               TypeTable &symbolTable);

  private:
    std::wstring m_symbol;
  };
} // namespace Vypr