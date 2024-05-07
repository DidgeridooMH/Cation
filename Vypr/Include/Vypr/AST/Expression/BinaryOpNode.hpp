#pragma once

#include <memory>
#include <unordered_map>

#include "Vypr/AST/Expression/BinaryOp.hpp"
#include "Vypr/AST/Expression/ExpressionNode.hpp"

namespace Vypr
{
  class BinaryOpNode : public ExpressionNode
  {
  public:
    BinaryOpNode(BinaryOp op, std::unique_ptr<ExpressionNode> &&lhs,
                 std::unique_ptr<ExpressionNode> &&rhs, size_t column,
                 size_t line);

    std::wstring PrettyPrint(int level) const override;

    static std::unique_ptr<ExpressionNode> Parse(
        std::unique_ptr<ExpressionNode> &base, CLangLexer &lexer,
        const ASTContext &context);

    llvm::Value *GenerateCode(Context &context) const override;

  private:
    void CastType();
    void CastTypeIntegral();
    void CastTypeIntegralComparison();
    void CastTypeReal();
    void CastTypePointer();

    void CastTypeArithmetic(BinaryOp op);
    void CastTypeBitwise(BinaryOp op);
    void CastTypeComparison(BinaryOp op);
    void CastTypeLogic(BinaryOp op);

    BinaryOp m_op;
    std::unique_ptr<ExpressionNode> m_lhs;
    std::unique_ptr<ExpressionNode> m_rhs;
  };

  extern const std::unordered_map<CLangTokenType, BinaryOp> BinaryOperations;
  extern const std::unordered_map<BinaryOp, int> BinaryOperationPrecedence;
} // namespace Vypr