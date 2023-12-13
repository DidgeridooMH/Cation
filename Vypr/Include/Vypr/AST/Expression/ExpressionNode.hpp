#pragma once

#include "llvm/IR/Value.h"
#include "Vypr/AST/SymbolTable.hpp"
#include "Vypr/AST/Type/StorageType.hpp"
#include "Vypr/CodeGen/Context.hpp"
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
                                                 TypeTable &symbolTable,
                                                 int precedenceLevel = 15);

    virtual std::wstring PrettyPrint(int level) const;

    virtual llvm::Value *GenerateCode(Context &context) const
    {
      return nullptr;
    };

    std::unique_ptr<StorageType> type;
    size_t column;
    size_t line;
  };
} // namespace Vypr