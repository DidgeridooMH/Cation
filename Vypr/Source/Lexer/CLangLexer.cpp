#include "Vypr/Lexer/CLangLexer.hpp"

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

  CLangLexer::CLangLexer(std::unique_ptr<Scanner> scanner)
      : m_scanner(std::move(scanner))
  {
  }

  CLangToken CLangLexer::GetToken()
  {
    if (m_lookAheadBuffer.has_value())
    {
      CLangToken token = *m_lookAheadBuffer;
      m_lookAheadBuffer = {};
      return token;
    }

    while (!m_scanner->Finished())
    {
      m_scanner->NextWhile(reinterpret_cast<bool (*)(wchar_t)>(iswspace));

      if (m_scanner->LookAhead(0, 2) == L"//")
      {
        m_scanner->NextWhile([](wchar_t c) { return c != '\n'; });
      }
      else if (m_scanner->LookAhead(0, 2) == L"/*")
      {
        m_scanner->Next(2);
        while (!m_scanner->Finished() && m_scanner->LookAhead(0, 2) != L"*/")
        {
          m_scanner->Next();
        }
        m_scanner->Next(2);
      }
      else if (iswdigit(m_scanner->LookAhead(0)) ||
               (m_scanner->LookAhead(0) == '.' &&
                iswdigit(m_scanner->LookAhead(1))))
      {
        return ParseNumericalConstant();
      }
      else if (PunctuatorMap.contains(m_scanner->LookAhead(0, 1)))
      {
        return ParsePunctuator();
      }
      else if (m_scanner->LookAhead(0) == '\'')
      {
        return ParseCharacterConstant();
      }
      else if (m_scanner->LookAhead(0) == '"')
      {
        return ParseStringLiteral();
      }
      else if (!m_scanner->Finished())
      {
        return ParseIdentifier();
      }
    }

    return {};
  }

  CLangToken CLangLexer::PeekToken()
  {
    if (!m_lookAheadBuffer.has_value())
    {
      m_lookAheadBuffer = GetToken();
    }
    return *m_lookAheadBuffer;
  }

  CLangToken CLangLexer::ParsePunctuator()
  {
    CLangToken token = {.line = m_scanner->GetLine(),
                        .column = m_scanner->GetColumn()};
    do
    {
      token.content += m_scanner->Next();
    } while (
        !m_scanner->Finished() &&
            PunctuatorMap.contains(token.content + m_scanner->LookAhead(0)) ||
        PunctuatorMap.contains(token.content + m_scanner->LookAhead(0, 2)));
    token.type = PunctuatorMap.at(token.content);
    return token;
  }

  CLangToken CLangLexer::ParseIdentifier()
  {
    CLangToken token = {.type = CLangTokenType::Identifier,
                        .line = m_scanner->GetLine(),
                        .column = m_scanner->GetColumn()};
    while (!m_scanner->Finished() &&
           !PunctuatorMap.contains(m_scanner->LookAhead(0, 1)) &&
           m_scanner->LookAhead(0) != '"' && m_scanner->LookAhead(0) != '\'' &&
           !iswspace(m_scanner->LookAhead(0)))
    {
      token.content += m_scanner->Next();
      if (token.content.back() == '\\')
      {
        token.content.pop_back();
        std::wstring uChar = ParseUniversalCharacter();
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

  std::wstring CLangLexer::ParseUniversalCharacter()
  {
    constexpr size_t Utf16CodeSize = 4;
    constexpr size_t Utf32CodeSize = 8;

    if (m_scanner->Finished() ||
        (m_scanner->LookAhead(0) != 'U' && m_scanner->LookAhead(0) != 'u'))
    {
      return L"";
    }

    size_t codeSize = m_scanner->Next() == 'u' ? Utf16CodeSize : Utf32CodeSize;

    std::wstring unicodeCharacter = m_scanner->Next(codeSize);
    if (unicodeCharacter.length() != codeSize)
    {
      return L"";
    }

    for (wchar_t codepoint : unicodeCharacter)
    {
      if (!iswdigit(codepoint) && towlower(codepoint) < 'a' &&
          towlower(codepoint) > 'f')
      {
        return L"";
      }
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

  CLangToken CLangLexer::ParseNumericalConstant()
  {
    CLangToken token = {.type = CLangTokenType::IntegerConstant,
                        .line = m_scanner->GetLine(),
                        .column = m_scanner->GetColumn()};

    if (m_scanner->LookAhead(0, 2) == L"0b" ||
        m_scanner->LookAhead(0, 2) == L"0B")
    {
      token.content += m_scanner->Next();
      token.content += towlower(m_scanner->Next());
      token.content += ParseBinaryConstant();
    }
    else if (m_scanner->LookAhead(0, 2) == L"0x" ||
             m_scanner->LookAhead(0, 2) == L"0X")
    {
      token.content += m_scanner->Next();
      token.content += towlower(m_scanner->Next());

      const auto &[isFloatingPoint, content] = ParseFloatableConstant(
          &CLangLexer::ParseHexadecimalSequence, 'p', true);
      token.content += content;
      if (isFloatingPoint)
      {
        token.type = CLangTokenType::FloatConstant;
      }
    }
    else
    {
      const auto &[isFloatingPoint, content] =
          ParseFloatableConstant(&CLangLexer::ParseIntegerSequence, 'e', false);
      token.content += content;
      if (isFloatingPoint)
      {
        token.type = CLangTokenType::FloatConstant;
      }
    }

    if (token.type == CLangTokenType::IntegerConstant)
    {
      token.content += ParseIntegerSuffix();
    }
    else
    {
      token.content += ParseFloatSuffix();
    }

    return token;
  }

  std::wstring CLangLexer::ParseBinaryConstant()
  {
    std::wstring buffer;
    while (iswdigit(m_scanner->LookAhead(0)))
    {
      buffer += m_scanner->Next();
      if (buffer.back() != '0' && buffer.back() != '1')
      {
        throw ParsingException(buffer + L" is not a valid binary number.",
                               m_scanner->GetColumn(), m_scanner->GetLine());
      }
    }

    if (buffer.empty())
    {
      throw ParsingException(L"Expected binary number.", m_scanner->GetColumn(),
                             m_scanner->GetLine());
    }

    return buffer;
  }

  std::tuple<bool, std::wstring> CLangLexer::ParseFloatableConstant(
      std::wstring (CLangLexer::*parser)(), char exponentDelimiter,
      bool requireExponent)
  {
    std::wstring buffer;
    bool isFloatingPoint = false;

    if (m_scanner->LookAhead(0) != '.')
    {
      buffer += (this->*parser)();
    }

    if (m_scanner->LookAhead(0) == '.')
    {
      isFloatingPoint = true;

      buffer += m_scanner->Next();

      try
      {
        buffer += (this->*parser)();
      }
      catch (const ParsingException &e)
      {
        if (buffer == L".")
        {
          throw e;
        }
      }
    }

    if (towlower(m_scanner->LookAhead(0)) == exponentDelimiter)
    {
      buffer += towlower(m_scanner->Next());

      if (m_scanner->LookAhead(0) == '-' || m_scanner->LookAhead(0) == '+')
      {
        buffer += m_scanner->Next();
      }

      buffer += ParseIntegerSequence();
    }
    else if (isFloatingPoint && requireExponent)
    {
      throw ParsingException(L"Invalid numerical constant.",
                             m_scanner->GetColumn(), m_scanner->GetLine());
    }

    return {isFloatingPoint, buffer};
  }

  std::wstring CLangLexer::ParseHexadecimalSequence()
  {
    std::wstring buffer;
    while (iswxdigit(m_scanner->LookAhead(0)))
    {
      buffer += towlower(m_scanner->Next());
    }

    if (buffer.empty())
    {
      throw ParsingException(L"Expected hexadecimal number.",
                             m_scanner->GetColumn(), m_scanner->GetLine());
    }

    return buffer;
  }

  std::wstring CLangLexer::ParseIntegerSequence()
  {
    std::wstring buffer;
    while (iswdigit(m_scanner->LookAhead(0)))
    {
      buffer += m_scanner->Next();
    }

    if (buffer.empty())
    {
      throw ParsingException(L"Expected decimal number.",
                             m_scanner->GetColumn(), m_scanner->GetLine());
    }

    return buffer;
  }

  std::wstring CLangLexer::ParseIntegerSuffix()
  {
    std::wstring buffer;
    bool signUsed = false;
    bool sizeUsed = false;
    while (iswalpha(m_scanner->LookAhead(0)))
    {
      buffer += towlower(m_scanner->Next());

      if (buffer.back() == 'u')
      {
        if (signUsed)
        {
          throw ParsingException(L"Invalid suffix for integer constant " +
                                     m_scanner->LookAhead(0, 1),
                                 m_scanner->GetColumn(), m_scanner->GetLine());
        }
        signUsed = true;
      }
      else if (buffer.back() == 'l')
      {
        if (sizeUsed && buffer[buffer.size() - 2] != 'l')
        {
          throw ParsingException(L"Invalid suffix for integer constant " +
                                     m_scanner->LookAhead(0, 1),
                                 m_scanner->GetColumn(), m_scanner->GetLine());
        }
        sizeUsed = true;
      }
      else
      {
        throw ParsingException(L"Invalid suffix for integer constant " +
                                   m_scanner->LookAhead(0, 1),
                               m_scanner->GetColumn(), m_scanner->GetLine());
      }
    }
    return buffer;
  }

  std::wstring CLangLexer::ParseFloatSuffix()
  {
    if (towlower(m_scanner->LookAhead(0)) == 'l' ||
        towlower(m_scanner->LookAhead(0)) == 'f')
    {
      return std::wstring{static_cast<wchar_t>(towlower(m_scanner->Next()))};
    }
    else if (iswalpha(m_scanner->LookAhead(0)))
    {
      throw ParsingException(L"Invalid suffix for floating point constant " +
                                 m_scanner->LookAhead(0, 1),
                             m_scanner->GetColumn(), m_scanner->GetLine());
    }

    return {};
  }

  CLangToken CLangLexer::ParseCharacterConstant()
  {
    CLangToken token{.type = CLangTokenType::CharacterConstant,
                     .line = m_scanner->GetLine(),
                     .column = m_scanner->GetColumn()};
    m_scanner->Next();
    if (m_scanner->LookAhead(0) == '\\')
    {
      m_scanner->Next();
      token.content += ParseEscapeSequence();
    }
    else if (m_scanner->LookAhead(0) == '\'')
    {
      throw ParsingException(L"Expected '\'' to end character literal.",
                             m_scanner->GetColumn(), m_scanner->GetLine());
    }
    else
    {
      token.content += m_scanner->Next();
    }

    if (m_scanner->LookAhead(0) != '\'')
    {
      throw ParsingException(L"Expected '\'' to end character literal.",
                             m_scanner->GetColumn(), m_scanner->GetLine());
    }
    m_scanner->Next();

    return token;
  }

  wchar_t CLangLexer::ParseEscapeSequence()
  {
    static const std::unordered_map<wchar_t, wchar_t> EscapeTranslations = {
        {'a', '\a'},  {'b', '\b'},  {'e', '\e'}, {'f', '\f'},
        {'n', '\n'},  {'r', '\r'},  {'t', '\t'}, {'v', '\v'},
        {'\\', '\\'}, {'\'', '\''}, {'"', '"'},  {'?', '\?'}};
    if (EscapeTranslations.contains(m_scanner->LookAhead(0)))
    {
      return EscapeTranslations.at(m_scanner->Next());
    }

    if (m_scanner->LookAhead(0) == 'x')
    {
      return stoi(
          m_scanner->NextWhile([](wchar_t c) { return iswxdigit(c) != 0; }),
          nullptr, 16);
    }
    else if (m_scanner->LookAhead(0) == 'u' || m_scanner->LookAhead(0) == 'U')
    {
      return ParseUniversalCharacter()[0];
    }
    else if (isdigit(m_scanner->LookAhead(0)))
    {
      return stoi(
          m_scanner->NextWhile([](wchar_t c) { return isdigit(c) != 0; }),
          nullptr, 8);
    }

    throw ParsingException(L"Unknown escape sequence " +
                               m_scanner->LookAhead(0, 1),
                           m_scanner->GetColumn(), m_scanner->GetLine());
  }

  CLangToken CLangLexer::ParseStringLiteral()
  {
    CLangToken token{.type = CLangTokenType::StringLiteral,
                     .line = m_scanner->GetLine(),
                     .column = m_scanner->GetColumn()};
    m_scanner->Next();
    while (!m_scanner->Finished() && m_scanner->LookAhead(0) != '\n' &&
           m_scanner->LookAhead(0) != '"')
    {
      if (m_scanner->LookAhead(0) == '\\')
      {
        m_scanner->Next();
        token.content += ParseEscapeSequence();
      }
      else
      {
        token.content += m_scanner->Next();
      }
    }
    if (m_scanner->LookAhead(0) != '"')
    {
      throw ParsingException(L"Expected \" at the end of string literal.",
                             m_scanner->GetColumn(), m_scanner->GetLine());
    }
    m_scanner->Next();
    return token;
  }
} // namespace Vypr
