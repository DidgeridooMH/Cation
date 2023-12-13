#include "Vypr/AST/Expression/ConstantNode.hpp"

#include <codecvt>
#include <locale>

#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/PointerType.hpp"
#include "Vypr/AST/UnexpectedTokenException.hpp"
#include "Vypr/Util/Overload.hpp"

namespace Vypr
{
  ConstantNode::ConstantNode(std::unique_ptr<StorageType> &type,
                             ConstantValue value, size_t column, size_t line)
      : ExpressionNode(std::move(type), column, line), m_value(value)
  {
  }

  std::wstring ConstantNode::PrettyPrint(int level) const
  {
    std::wstring result = ExpressionNode::PrettyPrint(level);
    result += L"Constant(";
    std::visit(
        overloaded{
            [&](auto arg) { result += std::to_wstring(arg); },
            [&](const std::string &arg) {
              std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
              result += L"\"" + converter.from_bytes(arg) + L"\"";
            }},
        m_value);
    result += L")\n";
    return result;
  }

  llvm::Value *ConstantNode::GenerateCode(Context &context) const
  {
    return std::visit(
        [&](const auto &constant) -> llvm::Value * {
          using T = std::decay_t<decltype(constant)>;
          if constexpr (std::is_same_v<T, int32_t>)
          {
            return llvm::ConstantInt::get(
                llvm::Type::getInt32Ty(context.context), constant, true);
          }
          else if constexpr (std::is_same_v<T, int64_t>)
          {
            return llvm::ConstantInt::get(
                llvm::Type::getInt64Ty(context.context), constant, true);
          }
          else if constexpr (std::is_same_v<T, uint8_t>)
          {
            return llvm::ConstantInt::get(
                llvm::Type::getInt8Ty(context.context), constant, false);
          }
          else if constexpr (std::is_same_v<T, uint32_t>)
          {
            return llvm::ConstantInt::get(
                llvm::Type::getInt32Ty(context.context), constant, false);
          }
          else if constexpr (std::is_same_v<T, uint64_t>)
          {
            return llvm::ConstantInt::get(
                llvm::Type::getInt64Ty(context.context), constant, false);
          }
          else if constexpr (std::is_same_v<T, float>)
          {
            return llvm::ConstantInt::get(
                llvm::Type::getFloatTy(context.context), constant);
          }
          else if constexpr (std::is_same_v<T, double>)
          {
            return llvm::ConstantInt::get(
                llvm::Type::getDoubleTy(context.context), constant);
          }
          else
          {
            return nullptr;
          }
        },
        m_value);
  }

  std::unique_ptr<ConstantNode> ConstantNode::Parse(CLangLexer &lexer)
  {
    CLangToken nextToken = lexer.GetToken();
    switch (nextToken.type)
    {
    case CLangTokenType::IntegerConstant:
      return ParseIntegerConstant(nextToken);
    case CLangTokenType::FloatConstant:
      return ParseFloatConstant(nextToken);
    case CLangTokenType::CharacterConstant: {
      std::unique_ptr<StorageType> constantType =
          std::make_unique<IntegralType>(Integral::Byte, false, false, false);
      return std::make_unique<ConstantNode>(
          constantType,
          (uint8_t)std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>()
              .to_bytes(nextToken.content)[0],
          nextToken.column, nextToken.line);
    }
    case CLangTokenType::StringLiteral:
      return ParseStringLiteral(nextToken);
    default:
      throw UnexpectedTokenException("constant or literal", nextToken.line,
                                     nextToken.column);
    }
  }

  std::unique_ptr<ConstantNode> ConstantNode::ParseIntegerConstant(
      const CLangToken &token)
  {
    bool isUnsigned = false;
    int longCount = 0;

    int postfix = 0;
    auto iter = token.content.crbegin();
    while (iter != token.content.crend() && (*iter == 'u' || *iter == 'l'))
    {
      if (*iter == 'u')
      {
        isUnsigned = true;
      }
      else if (*iter == 'l')
      {
        longCount += 1;
      }
      postfix += 1;
      iter++;
    }

    int prefix = 0;
    int radix = 10;
    if (token.content.starts_with(L"0b"))
    {
      prefix = 2;
      radix = 2;
    }
    else if (token.content.starts_with(L"0x"))
    {
      prefix = 2;
      radix = 16;
    }

    std::wstring constant =
        token.content.substr(prefix, token.content.length() - postfix - prefix);

    std::unique_ptr<StorageType> constantType;
    ConstantValue value;
    if (isUnsigned)
    {
      if (longCount < 2)
      {
        constantType =
            std::make_unique<IntegralType>(Integral::Int, true, false, false);
        value = (uint32_t)std::stoul(constant, nullptr, radix);
      }
      else
      {
        constantType =
            std::make_unique<IntegralType>(Integral::Long, true, false, false);
        value = (uint64_t)std::stoull(constant, nullptr, radix);
      }
    }
    else
    {
      if (longCount < 2)
      {
        constantType =
            std::make_unique<IntegralType>(Integral::Int, false, false, false);
        value = (int32_t)std::stol(constant, nullptr, radix);
      }
      else
      {
        constantType =
            std::make_unique<IntegralType>(Integral::Long, false, false, false);
        value = (int64_t)std::stoll(constant, nullptr, radix);
      }
    }
    return std::make_unique<ConstantNode>(constantType, value, token.column,
                                          token.line);
  }

  std::unique_ptr<ConstantNode> ConstantNode::ParseFloatConstant(
      const CLangToken &token)
  {
    bool isDouble = true;
    bool isLong = false;

    int postfix = 0;
    auto iter = token.content.crbegin();
    while (iter != token.content.crend() && (*iter == 'f' || *iter == 'l'))
    {
      if (*iter == 'l')
      {
        isLong = true;
      }
      else if (*iter == 'f')
      {
        isDouble = false;
      }
      postfix += 1;
      iter++;
    }

    std::wstring constant =
        token.content.substr(0, token.content.length() - postfix);
    if (isLong)
    {
      // TODO: Once real type is done.
    }
    else if (isDouble)
    {
      /*return std::make_unique<ConstantNode>(
          ValueType{.storage = PrimitiveType::Double},
          std::stof(constant, nullptr), token.column, token.line);*/
    }
    else
    {
      /*return std::make_unique<ConstantNode>(
          ValueType{.storage = PrimitiveType::Float},
          std::stod(constant, nullptr), token.column, token.line);*/
    }
    return nullptr;
  }

  std::unique_ptr<ConstantNode> ConstantNode::ParseStringLiteral(
      const CLangToken &token)
  {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::string stringLiteral = converter.to_bytes(token.content);

    std::unique_ptr<StorageType> storageType =
        std::make_unique<IntegralType>(Integral::Byte, false, false, false);
    std::unique_ptr<StorageType> pointerType =
        std::make_unique<PointerType>(storageType, true, false);
    return std::make_unique<ConstantNode>(pointerType, stringLiteral,
                                          token.column, token.line);
  }
} // namespace Vypr