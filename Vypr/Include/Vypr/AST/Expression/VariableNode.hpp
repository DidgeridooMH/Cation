#pragma once

#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Expression/ExpressionNode.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"

namespace Vypr
{
  class VariableNode : public ExpressionNode
  {
  public:
    VariableNode(std::unique_ptr<StorageType> &type, std::wstring symbol,
                 size_t column, size_t line)
        : ExpressionNode(std::move(type), column, line), m_symbol(symbol)
    {
    }

    std::wstring PrettyPrint(int level) const override
    {
      return ExpressionNode::PrettyPrint(level) + L"Variable(" + m_symbol +
             L")\n";
    }

    llvm::Value *GenerateCode(Context &context) const override
    {
      llvm::Value *result = nullptr;

      switch (type->GetType())
      {
      case StorageMetaType::Integral:
        switch (dynamic_cast<IntegralType *>(type.get())->integral)
        {
        case Integral::Int:
        case Integral::Bool:
          result = context.builder.CreateLoad(
              context.builder.getInt32Ty(),
              context.symbolTable.GetSymbol(m_symbol));
          break;
        case Integral::Byte:
          result = context.builder.CreateLoad(
              context.builder.getInt8Ty(),
              context.symbolTable.GetSymbol(m_symbol));
          break;
        case Integral::Short:
          result = context.builder.CreateLoad(
              context.builder.getInt16Ty(),
              context.symbolTable.GetSymbol(m_symbol));
          break;
        case Integral::Long:
          result = context.builder.CreateLoad(
              context.builder.getInt64Ty(),
              context.symbolTable.GetSymbol(m_symbol));
          break;
        }
        break;
      default:
        // TODO
        break;
      }

      return result;
    }

    static std::unique_ptr<VariableNode> Parse(CLangLexer &lexer,
                                               TypeTable &symbolTable)
    {
      CLangToken nextToken = lexer.GetToken();
      if (nextToken.type != CLangTokenType::Identifier)
      {
        throw CompileError(CompileErrorId::ExpectedIdentifier, nextToken.line,
                           nextToken.column);
      }

      std::shared_ptr<Vypr::StorageType> symbol =
          symbolTable.GetSymbol(nextToken.content);
      if (symbol == nullptr)
      {
        throw CompileError(CompileErrorId::UndefinedSymbol, nextToken.column,
                           nextToken.line, nextToken.content);
      }

      std::unique_ptr<StorageType> symbolType = symbol->Clone();

      return std::make_unique<VariableNode>(symbolType, nextToken.content,
                                            nextToken.column, nextToken.line);
    }

  private:
    std::wstring m_symbol;
  };
} // namespace Vypr