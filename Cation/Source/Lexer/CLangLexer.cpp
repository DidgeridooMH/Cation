#include <format>
#include <istream>
#include <string>
#include "CLangLexer.hpp"
#include "CLangTokenMap.hpp"
#include "CLangTokenType.hpp"

namespace Cation
{
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

    if (PunctuatorMap.contains(std::wstring{ source.peek() }))
    {
      return ParsePunctuator(source);
    }
    else if (source.peek() == '\'')
    {

    }
    else if (source.peek() == '"')
    {

    }
    else if (iswdigit(source.peek()))
    {

    }
    else
    {
      return ParseIdentifier(source);
    }

    return CLangToken{
      .type = CLangTokenType::BadToken,
      .content = std::format(L"Unknown token: {}", source.peek()),
      .line = m_currentLine,
      .column = m_currentColumn
    };
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
          return CLangToken{
            .type = CLangTokenType::BadToken,
            .content = L"Malformatted universal character"
          };
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
}
