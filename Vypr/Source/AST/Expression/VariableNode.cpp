#include "Vypr/AST/Expression/VariableNode.hpp"

#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/RealType.hpp"

namespace Vypr
{
  VariableNode::VariableNode(std::unique_ptr<StorageType> &&type,
                             std::wstring symbol, size_t column, size_t line)
      : ExpressionNode(std::move(type), column, line), m_symbol(symbol)
  {
  }

  std::wstring VariableNode::PrettyPrint(int level) const
  {
    return ExpressionNode::PrettyPrint(level) + L"Variable(" + m_symbol +
           L")\n";
  }

  llvm::Value *VariableNode::GenerateCode(Context &context) const
  {
    llvm::Value *result = nullptr;

    switch (type->GetType())
    {
    case StorageMetaType::Integral:
      switch (dynamic_cast<IntegralType *>(type.get())->integral)
      {
      case Integral::Bool:
        result =
            context.builder.CreateLoad(context.builder.getInt1Ty(),
                                       context.symbolTable.GetSymbol(m_symbol));
        break;
      case Integral::Int:
        result =
            context.builder.CreateLoad(context.builder.getInt32Ty(),
                                       context.symbolTable.GetSymbol(m_symbol));
        break;
      case Integral::Byte:
        result =
            context.builder.CreateLoad(context.builder.getInt8Ty(),
                                       context.symbolTable.GetSymbol(m_symbol));
        break;
      case Integral::Short:
        result =
            context.builder.CreateLoad(context.builder.getInt16Ty(),
                                       context.symbolTable.GetSymbol(m_symbol));
        break;
      case Integral::Long:
        result =
            context.builder.CreateLoad(context.builder.getInt64Ty(),
                                       context.symbolTable.GetSymbol(m_symbol));
        break;
      }
      break;
    case StorageMetaType::Real:
      switch (dynamic_cast<RealType *>(type.get())->real)
      {
      case Real::Float:
        result =
            context.builder.CreateLoad(context.builder.getFloatTy(),
                                       context.symbolTable.GetSymbol(m_symbol));
        break;
      case Real::Double:
        result =
            context.builder.CreateLoad(context.builder.getDoubleTy(),
                                       context.symbolTable.GetSymbol(m_symbol));
        break;
      }
      break;
    case StorageMetaType::Array:
    case StorageMetaType::Pointer:
      result = context.builder.CreateLoad(
          context.builder.getPtrTy(), context.symbolTable.GetSymbol(m_symbol));
      break;
    default:
      break;
    }

    return result;
  }

  std::unique_ptr<VariableNode> VariableNode::Parse(CLangLexer &lexer,
                                                    const ASTContext &context)
  {
    CLangToken nextToken = lexer.GetToken();
    if (nextToken.type != CLangTokenType::Identifier)
    {
      throw CompileError(CompileErrorId::ExpectedIdentifier, nextToken.line,
                         nextToken.column);
    }

    std::shared_ptr<Vypr::StorageType> symbol =
        context.GetSymbolType(nextToken.content);
    if (symbol == nullptr)
    {
      throw CompileError(CompileErrorId::UndefinedSymbol, nextToken.column,
                         nextToken.line, nextToken.content);
    }

    std::unique_ptr<StorageType> symbolType = symbol->Clone();
    symbolType->isLValue = true;
    return std::make_unique<VariableNode>(std::move(symbolType),
                                          nextToken.content, nextToken.column,
                                          nextToken.line);
  }
} // namespace Vypr
