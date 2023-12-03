#pragma once

#include "Vypr/AST/Expression/ValueType.hpp"
#include "Vypr/Lexer/CLangLexer.hpp"

namespace Vypr
{
  class ExpressionNode
  {
  public:
    ExpressionNode(ValueType type);

    static std::unique_ptr<ExpressionNode> Parse(CLangLexer &lexer,
                                                 int precedenceLevel = 15);

    virtual std::wstring PrettyPrint(int level) const;

    ValueType m_type;
  };
} // namespace Vypr