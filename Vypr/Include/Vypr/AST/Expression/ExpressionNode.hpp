#pragma once

#include "Vypr/AST/ASTContext.hpp"
#include "Vypr/AST/Type/StorageType.hpp"
#include "Vypr/CodeGen/Context.hpp"
#include "Vypr/Lexer/CLangLexer.hpp"
#include "llvm/IR/Value.h"

namespace Vypr
{
  class ExpressionNode
  {
  public:
    ExpressionNode();

    ExpressionNode(std::unique_ptr<StorageType> &&type, size_t column,
                   size_t line);

    static std::unique_ptr<ExpressionNode> Parse(CLangLexer &lexer,
                                                 const TypeTable &context,
                                                 int precedenceLevel = 15);

    virtual std::wstring PrettyPrint(int level) const;

    virtual llvm::Value *GenerateCode(Context &context) const;

    std::unique_ptr<StorageType> type;
    size_t column;
    size_t line;
  };
} // namespace Vypr
