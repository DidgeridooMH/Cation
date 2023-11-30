#include "Vypr/Lexer/CLangLexer.hpp"

#include <istream>
#include <string>

#include "Vypr/Lexer/CLangTokenMap.hpp"
#include "Vypr/Lexer/CLangTokenType.hpp"

namespace Vypr
{
  ParsingException::ParsingException(std::wstring message, size_t column,
                                     size_t line)
      : message(message), column(column), line(line)
  {
  }

  CLangToken CLangLexer::GetToken(Scanner &source)
  {
    source.NextWhile(reinterpret_cast<bool (*)(wchar_t)>(iswspace));

    if (source.Finished())
    {
      return {};
    }

    if (iswdigit(source.LookAhead(0)) ||
        (source.LookAhead(0) == '.' && iswdigit(source.LookAhead(1))))
    {
      return ParseNumericalConstant(source);
    }
    else if (PunctuatorMap.contains(source.LookAhead(0, 1)))
    {
      return ParsePunctuator(source);
    }
    else if (source.LookAhead(0) == '\'')
    {
      return ParseCharacterConstant(source);
    }
    else if (source.LookAhead(0) == '"')
    {
      return ParseStringLiteral(source);
    }
    else
    {
      return ParseIdentifier(source);
    }

    throw ParsingException(L"Unknown token " + source.LookAhead(0, 1) +
                               L" found.",
                           source.GetColumn(), source.GetLine());
  }

  CLangToken CLangLexer::ParsePunctuator(Scanner &source)
  {
    CLangToken token = {.line = source.GetLine(), .column = source.GetColumn()};
    do
    {
      token.content += source.Next();
    } while (!source.Finished() &&
                 PunctuatorMap.contains(token.content + source.LookAhead(0)) ||
             PunctuatorMap.contains(token.content + source.LookAhead(0, 2)));
    token.type = PunctuatorMap.at(token.content);
    return token;
  }

  CLangToken CLangLexer::ParseIdentifier(Scanner &source)
  {
    CLangToken token = {.type = CLangTokenType::Identifier,
                        .line = source.GetLine(),
                        .column = source.GetColumn()};
    while (!source.Finished() &&
           !PunctuatorMap.contains(source.LookAhead(0, 1)) &&
           source.LookAhead(0) != '"' && source.LookAhead(0) != '\'' &&
           !iswspace(source.LookAhead(0)))
    {
      token.content += source.Next();
      if (token.content.back() == '\\')
      {
        token.content.pop_back();
        std::wstring uChar = ParseUniversalCharacter(source);
        if (uChar.empty())
        {
          throw ParsingException(L"Malformatted universal character",
                                 token.column, token.line);
        }
        token.content += uChar;
      }
    }

    if (KeywordMap.contains(token.content))
    {
      token.type = KeywordMap.at(token.content);
    }

    return token;
  }

  std::wstring CLangLexer::ParseUniversalCharacter(Scanner &source)
  {
    constexpr size_t Utf16CodeSize = 4;
    constexpr size_t Utf32CodeSize = 8;

    if (source.Finished() ||
        (source.LookAhead(0) != 'U' && source.LookAhead(0) != 'u'))
    {
      return L"";
    }

    size_t codeSize = source.Next() == 'u' ? Utf16CodeSize : Utf32CodeSize;

    std::wstring unicodeCharacter = source.Next(codeSize);
    if (unicodeCharacter.length() != codeSize)
    {
      return L"";
    }

    if (std::find_if(
            unicodeCharacter.begin(), unicodeCharacter.end(), [](wchar_t c) {
              return !iswdigit(c) && towlower(c) < 'a' && towlower(c) > 'f';
            }) != unicodeCharacter.end())
    {
      return L"";
    }

    std::wstring buffer;
    uint32_t codepoint = std::stoi(unicodeCharacter, nullptr, 16);
#ifdef _WIN32
    if (codeSize == Utf16CodeSize || codepoint < 0x10000)
    {
      buffer += static_cast<wchar_t>(codepoint);
    }
    else
    {
      codepoint -= 0x10000;
      buffer += static_cast<wchar_t>(((codepoint >> 10) & 0x3FF) | 0xD800);
      buffer += static_cast<wchar_t>((codepoint & 0x3FF) | 0xDC00);
    }
#else
    buffer += static_cast<wchar_t>(codepoint);
#endif

    return buffer;
  }

  CLangToken CLangLexer::ParseNumericalConstant(Scanner &source)
  {
    CLangToken token = {.type = CLangTokenType::IntegerConstant,
                        .line = source.GetLine(),
                        .column = source.GetColumn()};

    if (source.LookAhead(0, 2) == L"0b" || source.LookAhead(0, 2) == L"0B")
    {
      token.content += source.Next();
      token.content += towlower(source.Next());
      token.content += ParseBinaryConstant(source);
    }
    else if (source.LookAhead(0, 2) == L"0x" || source.LookAhead(0, 2) == L"0X")
    {
      token.content += source.Next();
      token.content += towlower(source.Next());

      const auto &[isFloatingPoint, content] = ParseFloatableConstant(
          source, &CLangLexer::ParseHexadecimalSequence, 'p', true);
      token.content += content;
      if (isFloatingPoint)
      {
        token.type = CLangTokenType::FloatConstant;
      }
    }
    else
    {
      const auto &[isFloatingPoint, content] = ParseFloatableConstant(
          source, &CLangLexer::ParseIntegerSequence, 'e', false);
      token.content += content;
      if (isFloatingPoint)
      {
        token.type = CLangTokenType::FloatConstant;
      }
    }

    if (token.type == CLangTokenType::IntegerConstant)
    {
      token.content += ParseIntegerSuffix(source);
    }
    else
    {
      token.content += ParseFloatSuffix(source);
    }

    return token;
  }

  std::wstring CLangLexer::ParseBinaryConstant(Scanner &source)
  {
    std::wstring buffer;
    while (iswdigit(source.LookAhead(0)))
    {
      buffer += source.Next();
      if (buffer.back() != '0' && buffer.back() != '1')
      {
        throw ParsingException(buffer + L" is not a valid binary number.",
                               source.GetColumn(), source.GetLine());
      }
    }

    if (buffer.empty())
    {
      throw ParsingException(L"Expected binary number.", source.GetColumn(),
                             source.GetLine());
    }

    return buffer;
  }

  std::tuple<bool, std::wstring> CLangLexer::ParseFloatableConstant(
      Scanner &source, std::wstring (CLangLexer::*parser)(Scanner &),
      char exponentDelimiter, bool requireExponent)
  {
    std::wstring buffer;
    bool isFloatingPoint = false;

    if (source.LookAhead(0) != '.')
    {
      buffer += (this->*parser)(source);
    }

    if (source.LookAhead(0) == '.')
    {
      isFloatingPoint = true;

      buffer += source.Next();

      try
      {
        buffer += (this->*parser)(source);
      }
      catch (const ParsingException &e)
      {
        if (buffer == L".")
        {
          throw e;
        }
      }
    }

    if (towlower(source.LookAhead(0)) == exponentDelimiter)
    {
      buffer += towlower(source.Next());

      if (source.LookAhead(0) == '-' || source.LookAhead(0) == '+')
      {
        buffer += source.Next();
      }

      buffer += ParseIntegerSequence(source);
    }
    else if (isFloatingPoint && requireExponent)
    {
      throw ParsingException(L"Invalid numerical constant.", source.GetColumn(),
                             source.GetLine());
    }

    return {isFloatingPoint, buffer};
  }

  std::wstring CLangLexer::ParseHexadecimalSequence(Scanner &source)
  {
    std::wstring buffer;
    while (iswxdigit(source.LookAhead(0)))
    {
      buffer += towlower(source.Next());
    }

    if (buffer.empty())
    {
      throw ParsingException(L"Expected hexadecimal number.",
                             source.GetColumn(), source.GetLine());
    }

    return buffer;
  }

  std::wstring CLangLexer::ParseIntegerSequence(Scanner &source)
  {
    std::wstring buffer;
    while (iswdigit(source.LookAhead(0)))
    {
      buffer += source.Next();
    }

    if (buffer.empty())
    {
      throw ParsingException(L"Expected decimal number.", source.GetColumn(),
                             source.GetLine());
    }

    return buffer;
  }

  std::wstring CLangLexer::ParseIntegerSuffix(Scanner &source)
  {
    std::wstring buffer;
    bool signUsed = false;
    bool sizeUsed = false;
    while (iswalpha(source.LookAhead(0)))
    {
      buffer += towlower(source.Next());

      if (buffer.back() == 'u')
      {
        if (signUsed)
        {
          throw ParsingException(L"Invalid suffix for integer constant " +
                                     source.LookAhead(0, 1),
                                 source.GetColumn(), source.GetLine());
        }
        signUsed = true;
      }
      else if (buffer.back() == 'l')
      {
        if (sizeUsed && buffer[buffer.size() - 2] != 'l')
        {
          throw ParsingException(L"Invalid suffix for integer constant " +
                                     source.LookAhead(0, 1),
                                 source.GetColumn(), source.GetLine());
        }
        sizeUsed = true;
      }
      else
      {
        throw ParsingException(L"Invalid suffix for integer constant " +
                                   source.LookAhead(0, 1),
                               source.GetColumn(), source.GetLine());
      }
    }
    return buffer;
  }

  std::wstring CLangLexer::ParseFloatSuffix(Scanner &source)
  {
    if (towlower(source.LookAhead(0)) == 'l' ||
        towlower(source.LookAhead(0)) == 'f')
    {
      return std::wstring{static_cast<wchar_t>(towlower(source.Next()))};
    }
    else if (iswalpha(source.LookAhead(0)))
    {
      throw ParsingException(L"Invalid suffix for floating point constant " +
                                 source.LookAhead(0, 1),
                             source.GetColumn(), source.GetLine());
    }

    return {};
  }

  CLangToken CLangLexer::ParseCharacterConstant(Scanner &source)
  {
    CLangToken token{.type = CLangTokenType::CharacterConstant,
                     .line = source.GetLine(),
                     .column = source.GetColumn()};
    source.Next();
    if (source.LookAhead(0) == '\\')
    {
      source.Next();
      token.content += ParseEscapeSequence(source);
    }
    else if (source.LookAhead(0) == '\'')
    {
      throw ParsingException(L"Expected '\'' to end character literal.",
                             source.GetColumn(), source.GetLine());
    }
    else
    {
      token.content += source.Next();
    }

    if (source.LookAhead(0) != '\'')
    {
      throw ParsingException(L"Expected '\'' to end character literal.",
                             source.GetColumn(), source.GetLine());
    }
    source.Next();

    return token;
  }

  wchar_t CLangLexer::ParseEscapeSequence(Scanner &source)
  {
    static const std::unordered_map<wchar_t, wchar_t> EscapeTranslations = {
        {'a', '\a'},  {'b', '\b'},  {'e', '\e'}, {'f', '\f'},
        {'n', '\n'},  {'r', '\r'},  {'t', '\t'}, {'v', '\v'},
        {'\\', '\\'}, {'\'', '\''}, {'"', '"'},  {'?', '\?'}};
    if (EscapeTranslations.contains(source.LookAhead(0)))
    {
      return EscapeTranslations.at(source.Next());
    }

    if (source.LookAhead(0) == 'x')
    {
      return stoi(source.NextWhile([](wchar_t c) { return iswxdigit(c) != 0; }),
                  nullptr, 16);
    }
    else if (source.LookAhead(0) == 'u' || source.LookAhead(0) == 'U')
    {
      return ParseUniversalCharacter(source)[0];
    }
    else if (isdigit(source.LookAhead(0)))
    {
      return stoi(source.NextWhile([](wchar_t c) { return isdigit(c) != 0; }),
                  nullptr, 8);
    }

    throw ParsingException(L"Unknown escape sequence " + source.LookAhead(0, 1),
                           source.GetColumn(), source.GetLine());
  }

  CLangToken CLangLexer::ParseStringLiteral(Scanner &source)
  {
    CLangToken token{.type = CLangTokenType::StringLiteral,
                     .line = source.GetLine(),
                     .column = source.GetColumn()};
    source.Next();
    while (!source.Finished() && source.LookAhead(0) != '\n' &&
           source.LookAhead(0) != '"')
    {
      if (source.LookAhead(0) == '\\')
      {
        source.Next();
        token.content += ParseEscapeSequence(source);
      }
      else
      {
        token.content += source.Next();
      }
    }
    if (source.LookAhead(0) != '"')
    {
      throw ParsingException(L"Expected \" at the end of string literal.",
                             source.GetColumn(), source.GetLine());
    }
    source.Next();
    return token;
  }
} // namespace Vypr
