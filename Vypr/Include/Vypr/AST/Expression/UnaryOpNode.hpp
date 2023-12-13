#pragma once

#include <memory>
#include <unordered_map>

#include "Vypr/AST/Expression/ExpressionNode.hpp"
#include "Vypr/AST/Expression/UnaryOp.hpp"

namespace Vypr
{
  class UnaryOpNode : public ExpressionNode
  {
  public:
    UnaryOpNode(UnaryOp op, std::unique_ptr<ExpressionNode> &expression,
                size_t column, size_t line);

    std::wstring PrettyPrint(int level) const override;

    static std::unique_ptr<ExpressionNode> Parse(CLangLexer &lexer,
                                                 TypeTable &symbolTable);

  private:
    UnaryOp m_op;
    std::unique_ptr<ExpressionNode> m_expression;
  };

  extern const std::unordered_map<CLangTokenType, UnaryOp> UnaryOperations;
} // namespace Vypr