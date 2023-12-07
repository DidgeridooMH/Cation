#pragma once

#include "Vypr/AST/Type/StorageType.hpp"
#include "Vypr/Lexer/CLangLexer.hpp"

namespace Vypr
{
  class ExpressionNode
  {
  public:
    ExpressionNode();

    ExpressionNode(std::unique_ptr<StorageType> &&type, size_t column,
                   size_t line);

    static std::unique_ptr<ExpressionNode> Parse(CLangLexer &lexer,
                                                 int precedenceLevel = 15);

    virtual std::wstring PrettyPrint(int level) const;

    std::unique_ptr<StorageType> type;
    size_t column;
    size_t line;
  };
} // namespace Vypr