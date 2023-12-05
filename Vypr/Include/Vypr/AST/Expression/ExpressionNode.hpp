#pragma once

#include "Vypr/AST/Expression/ValueType.hpp"
#include "Vypr/Lexer/CLangLexer.hpp"

namespace Vypr
{
  class ExpressionNode
  {
  public:
    ExpressionNode() = default;

    ExpressionNode(ValueType type);

    static std::unique_ptr<ExpressionNode> Parse(CLangLexer &lexer,
                                                 int precedenceLevel = 15);

    virtual std::wstring PrettyPrint(int level) const;

    ValueType type;
  };
} // namespace Vypr