#include <format>
#include <istream>
#include <string>
#include "Vypr/Lexer/CLangLexer.hpp"
#include "Vypr/Lexer/CLangTokenMap.hpp"
#include "Vypr/Lexer/CLangTokenType.hpp"

namespace Vypr
{
  ParsingException::ParsingException(std::wstring message,
    size_t column, size_t line)
    : message(message), column(column), line(line)
  {
  }

  CLangLexer::CLangLexer() : m_currentLine(1), m_currentColumn(1)
  {
  }

  CLangToken CLangLexer::GetToken(std::wistream& source)
  {
    SkipWhitespace(source);

    if (!source.good() ||
      source.peek() == std::char_traits<wchar_t>::eof())
    {
      return {};
    }

    // Corner case for floating point that can be either a period or start of a
    // number.
    source.ignore();
    wchar_t lookAhead = source.peek();
    source.unget();

    if (iswdigit(source.peek()) ||
      (source.peek() == '.' && iswdigit(lookAhead)))
    {
      return ParseNumericalConstant(source);
    }
    else if (PunctuatorMap.contains(std::wstring{ source.peek() }))
    {
      return ParsePunctuator(source);
    }
    else if (source.peek() == '\'')
    {
      // TODO
    }
    else if (source.peek() == '"')
    {
      // TODO
    }
    else
    {
      return ParseIdentifier(source);
    }

    throw ParsingException(std::format(L"Unknown token {} found.",
      source.peek()), m_currentColumn, m_currentLine);
  }

  void CLangLexer::SkipWhitespace(std::wistream& source)
  {
    while (source.good() &&
      source.peek() != std::char_traits<wchar_t>::eof() &&
      iswspace(source.peek()))
    {
      m_currentColumn += 1;
      if (source.peek() == '\n')
      {
        m_currentLine += 1;
        m_currentColumn = 1;
      }
      source.ignore();
    }
  }

  CLangToken CLangLexer::ParsePunctuator(std::wistream& source)
  {
    CLangToken token = {
      .line = m_currentLine,
      .column = m_currentColumn
    };
    while (source.good() &&
      source.peek() != std::char_traits<wchar_t>::eof())
    {
      token.content += source.get();
      m_currentColumn += 1;

      if (!PunctuatorMap.contains(token.content) &&
        !(token.content == L".." && source.peek() == '.'))
      {
        token.content.pop_back();
        source.unget();
        m_currentColumn -= 1;
        break;
      }
    }
    token.type = PunctuatorMap.at(token.content);
    return token;
  }

  CLangToken CLangLexer::ParseIdentifier(std::wistream& source)
  {
    CLangToken token = {
      .type = CLangTokenType::Identifier,
      .line = m_currentLine,
      .column = m_currentColumn
    };
    while (source.good() &&
      source.peek() != std::char_traits<wchar_t>::eof() &&
      !PunctuatorMap.contains(std::wstring{ source.peek() }) &&
      source.peek() != '"' && source.peek() != '\'' &&
      !iswspace(source.peek()))
    {
      token.content += source.get();
      m_currentColumn += 1;
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

  std::wstring CLangLexer::ParseUniversalCharacter(std::wistream& source)
  {
    constexpr size_t Utf16CodeSize = 4;
    constexpr size_t Utf32CodeSize = 8;

    if (!source.good() || (source.peek() != 'U' && source.peek() != 'u'))
    {
      return L"";
    }

    size_t codeSize = source.get() == 'u' ? Utf16CodeSize : Utf32CodeSize;
    m_currentColumn += 1;

    std::wstring unicodeCharacter(codeSize, '0');
    source.read(unicodeCharacter.data(), codeSize);
    m_currentColumn += codeSize;

    if (source.fail() ||
      std::find_if(unicodeCharacter.begin(), unicodeCharacter.end(),
        [](wchar_t c)
        {
          return !iswdigit(c) && towlower(c) < 'a' && towlower(c) > 'f';
        }) != unicodeCharacter.end())
    {
      return L"";
    }

    std::wstring buffer;
    uint32_t codepoint = std::stoi(unicodeCharacter, nullptr, 16);
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

    return buffer;
  }

  CLangToken CLangLexer::ParseNumericalConstant(std::wistream& source)
  {
    CLangToken token = {
      .type = CLangTokenType::IntegerConstant,
      .line = m_currentLine,
      .column = m_currentColumn
    };

    // TODO: A better stream needs to be made.
    source.ignore();
    wchar_t lookAhead = towlower(source.peek());
    source.unget();

    if (source.peek() == '0' && lookAhead == 'b')
    {
      token.content += source.get();
      token.content += towlower(source.get());
      m_currentColumn += 2;
      token.content += ParseBinaryConstant(source);
    }
    else if (source.peek() == '0' && lookAhead == 'x')
    {
      token.content += source.get();
      token.content += towlower(source.get());
      m_currentColumn += 2;

      const auto& [isFloatingPoint, content] = ParseFloatableConstant(source,
        &CLangLexer::ParseHexadecimalSequence, 'p', true);
      token.content += content;
      if (isFloatingPoint)
      {
        token.type = CLangTokenType::FloatConstant;
      }
    }
    else
    {
      const auto& [isFloatingPoint, content] = ParseFloatableConstant(source,
        &CLangLexer::ParseIntegerSequence, 'e', false);
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

  std::wstring CLangLexer::ParseBinaryConstant(std::wistream& source)
  {
    std::wstring buffer;
    while (source.good() &&
      source.peek() != std::char_traits<wchar_t>::eof() &&
      iswdigit(source.peek()))
    {
      buffer += source.get();
      m_currentColumn += 1;
      if (buffer.back() != '0' && buffer.back() != '1')
      {
        throw ParsingException(
          std::format(L"{} is not a valid binary number.", buffer),
          m_currentColumn, m_currentLine);
      }
    }

    if (buffer.empty())
    {
      throw ParsingException(L"Expected binary number.", m_currentColumn,
        m_currentLine);
    }

    return buffer;
  }

  std::tuple<bool, std::wstring> CLangLexer::ParseFloatableConstant(
    std::wistream& source,
     std::wstring(CLangLexer::* parser)(std::wistream&),
    char exponentDelimiter, bool requireExponent)
  {
    std::wstring buffer;
    bool isFloatingPoint = false;

    if (source.peek() != '.')
    {
      buffer += (this->*parser)(source);
    }

    if (source.peek() == '.')
    {
      isFloatingPoint = true;

      buffer += source.get();
      m_currentColumn += 1;

      try
      {
        buffer += (this->*parser)(source);
      }
      catch (const ParsingException& e)
      {
        if (buffer == L".")
        {
          throw e;
        }
      }
    }

    if (towlower(source.peek()) == exponentDelimiter)
    {
      buffer += towlower(source.get());
      m_currentColumn += 1;

      if (source.peek() == '-' || source.peek() == '+')
      {
        buffer += source.get();
        m_currentColumn += 1;
      }

      buffer += ParseIntegerSequence(source);
    }
    else if (isFloatingPoint && requireExponent)
    {
      throw ParsingException(L"Invalid numerical constant.", m_currentColumn,
        m_currentLine);
    }

    return { isFloatingPoint, buffer };
  }

  std::wstring CLangLexer::ParseHexadecimalSequence(std::wistream& source)
  {
    std::wstring buffer;
    while (source.good() &&
      source.peek() != std::char_traits<wchar_t>::eof() &&
      iswxdigit(source.peek()))
    {
      buffer += towlower(source.get());
      m_currentColumn += 1;
    }

    if (buffer.empty())
    {
      throw ParsingException(L"Expected hexadecimal number.", m_currentColumn,
        m_currentLine);
    }

    return buffer;
  }

  std::wstring CLangLexer::ParseIntegerSequence(std::wistream& source)
  {
    std::wstring buffer;
    while (source.good() &&
      source.peek() != std::char_traits<wchar_t>::eof() &&
      iswdigit(source.peek()))
    {
      buffer += source.get();
      m_currentColumn += 1;
    }

    if (buffer.empty())
    {
      throw ParsingException(L"Expected decimal number.", m_currentColumn,
        m_currentLine);
    }

    return buffer;
  }

  std::wstring CLangLexer::ParseIntegerSuffix(std::wistream& source)
  {
    std::wstring buffer;
    bool signUsed = false;
    bool sizeUsed = false;
    while (source.good() &&
      source.peek() != std::char_traits<wchar_t>::eof() &&
      iswalpha(source.peek()))
    {
      buffer += towlower(source.get());
      m_currentColumn += 1;

      if (buffer.back() == 'u')
      {
        if (signUsed)
        {
          throw ParsingException(
            std::format(L"Invalid suffix for integer constant {}",
              source.get()), m_currentColumn, m_currentLine);
        }
        signUsed = true;
      }
      else if (buffer.back() == 'l')
      {
        if (sizeUsed && buffer[buffer.size() - 2] != 'l')
        {
          throw ParsingException(
            std::format(L"Invalid suffix for integer constant {}",
              source.get()), m_currentColumn, m_currentLine);
        }
        sizeUsed = true;
      }
      else
      {
        throw ParsingException(
          std::format(L"Invalid suffix for integer constant {}",
            source.get()), m_currentColumn, m_currentLine);
      }
    }
    return buffer;
  }

  std::wstring CLangLexer::ParseFloatSuffix(std::wistream& source)
  {
    if (towlower(source.peek()) == 'l')
    {
      source.ignore();
      m_currentColumn += 1;
      return L"l";
    }
    else if (towlower(source.peek()) == 'f')
    {
      source.ignore();
      m_currentColumn += 1;
      return L"f";
    }
    else if (iswalpha(source.peek()))
    {
      throw ParsingException(
        std::format(L"Invalid suffix for floating point constant {}",
          source.get()), m_currentColumn, m_currentLine);
    }

    return {};
  }
}
