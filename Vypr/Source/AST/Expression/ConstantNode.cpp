#include "Vypr/AST/Expression/ConstantNode.hpp"

#include <codecvt>
#include <locale>

#include "Vypr/AST/UnknownTokenException.hpp"

template <class... Ts> struct overloaded : Ts...
{
  using Ts::operator()...;
};

#ifdef __APPLE__
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
#endif

namespace Vypr
{
  ConstantNode::ConstantNode(ValueType type, ConstantValue value, size_t column,
                             size_t line)
      : ExpressionNode(type, column, line), m_value(value)
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

  std::unique_ptr<ConstantNode> ConstantNode::Parse(CLangLexer &lexer)
  {
    CLangToken nextToken = lexer.GetToken();
    switch (nextToken.type)
    {
    case CLangTokenType::IntegerConstant:
      return ParseIntegerConstant(nextToken);
    case CLangTokenType::FloatConstant:
      return ParseFloatConstant(nextToken);
    case CLangTokenType::CharacterConstant:
      return std::make_unique<ConstantNode>(
          ValueType{
              .storage = PrimitiveType::Byte,
          },
          (uint8_t)std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>()
              .to_bytes(nextToken.content)[0],
          nextToken.column, nextToken.line);
      break;
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
    while (iter != token.content.crend() && *iter == 'u' && *iter == 'l')
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
    if (isUnsigned)
    {
      if (longCount < 2)
      {
        return std::make_unique<ConstantNode>(
            ValueType{.storage = PrimitiveType::UInt},
            (uint32_t)std::stoul(constant, nullptr, radix), token.column,
            token.line);
      }
      else
      {
        return std::make_unique<ConstantNode>(
            ValueType{.storage = PrimitiveType::ULong},
            (uint64_t)std::stoull(constant, nullptr, radix), token.column,
            token.line);
      }
    }
    else
    {
      if (longCount < 2)
      {
        return std::make_unique<ConstantNode>(
            ValueType{.storage = PrimitiveType::Int},
            (int32_t)std::stol(constant, nullptr, radix), token.column,
            token.line);
      }
      else
      {
        return std::make_unique<ConstantNode>(
            ValueType{.storage = PrimitiveType::Long},
            (int64_t)std::stoll(constant, nullptr, radix), token.column,
            token.line);
      }
    }
  }

  std::unique_ptr<ConstantNode> ConstantNode::ParseFloatConstant(
      const CLangToken &token)
  {
    bool isDouble = true;

    int postfix = 0;
    auto iter = token.content.crbegin();
    while (iter != token.content.crend() && (*iter == 'f' || *iter == 'l'))
    {
      // TODO: Add long double type
      if (*iter == 'f')
      {
        isDouble = false;
      }
      postfix += 1;
      iter++;
    }

    std::wstring constant =
        token.content.substr(0, token.content.length() - postfix);
    if (isDouble)
    {
      return std::make_unique<ConstantNode>(
          ValueType{.storage = PrimitiveType::Double},
          std::stof(constant, nullptr), token.column, token.line);
    }
    else
    {
      return std::make_unique<ConstantNode>(
          ValueType{.storage = PrimitiveType::Float},
          std::stod(constant, nullptr), token.column, token.line);
    }
  }

  std::unique_ptr<ConstantNode> ConstantNode::ParseStringLiteral(
      const CLangToken &token)
  {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::string stringLiteral = converter.to_bytes(token.content);
    return std::make_unique<ConstantNode>(
        ValueType{.constant = true,
                  .indirection = {false},
                  .storage = PrimitiveType::Byte},
        stringLiteral, token.column, token.line);
  }
} // namespace Vypr