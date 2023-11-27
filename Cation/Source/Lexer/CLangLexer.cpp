#include <istream>
#include <string>
#include <unordered_map>
#include "CLangLexer.hpp"
#include "CLangToken.hpp"

namespace Cation
{
  using KeyToTokenMap =
    std::unordered_map<std::wstring, CLangToken::TokenType>;

  static const KeyToTokenMap PunctuatorSet = {
    {L"[", CLangToken::LeftBracket},
    {L"]", CLangToken::RightBracket},
    {L"(", CLangToken::LeftParenthesis},
    {L")", CLangToken::RightParenthesis},
    {L"{", CLangToken::LeftDragon},
    {L"}", CLangToken::RightDragon},
    {L".", CLangToken::Period},
    {L"->", CLangToken::Arrow},
    {L"++", CLangToken::Increment},
    {L"--", CLangToken::Decrement},
    {L"*", CLangToken::Star},
    {L"+", CLangToken::Add},
    {L"-", CLangToken::Subtract},
    {L"~", CLangToken::Tilde},
    {L"!", CLangToken::Exclamation},
    {L"/", CLangToken::Divide},
    {L"%", CLangToken::Modulo},
    {L"<<", CLangToken::ShiftLeft},
    {L">>", CLangToken::ShiftRight},
    {L"<", CLangToken::LessThan},
    {L">", CLangToken::GreaterThan},
    {L"<=", CLangToken::LessEqual},
    {L">=", CLangToken::GreaterEqual},
    {L"==", CLangToken::Equal},
    {L"!=", CLangToken::NotEqual},
    {L"&", CLangToken::And},
    {L"|", CLangToken::Or},
    {L"^", CLangToken::Xor},
    {L"&&", CLangToken::LogicalAnd},
    {L"||", CLangToken::LogicalOr},
    {L"?", CLangToken::TernaryProposition},
    {L":", CLangToken::TernaryDecision},
    {L";", CLangToken::StatementDelimiter},
    {L"...", CLangToken::Variadic},
    {L"=", CLangToken::Assign},
    {L"*=", CLangToken::MultiplyAssign},
    {L"/=", CLangToken::DivideAssign},
    {L"%=", CLangToken::ModuloAssign},
    {L"+=", CLangToken::AddAssign},
    {L"-=", CLangToken::SubtractAssign},
    {L"<<=", CLangToken::LeftShiftAssign},
    {L">>=", CLangToken::RightShiftAssign},
    {L"&=", CLangToken::AndAssign},
    {L"^=", CLangToken::XorAssign},
    {L"|=", CLangToken::OrAssign},
    {L",", CLangToken::Comma},
    {L"#", CLangToken::Preprocessor},
    {L"##", CLangToken::PreprocessorConcat}
  };

  CLangLexer::CLangLexer() : m_currentLine(1), m_currentColumn(1)
  {
  }

  CLangToken CLangLexer::GetToken(std::wistream& source)
  {
    SkipWhitespace(source);

    if (!source.good())
    {
      return CLangToken(CLangToken::NoToken);
    }

    size_t currentColumn = m_currentColumn;
    std::wstring tokenBuffer{ source.get() };
    m_currentColumn += 1;

    CLangToken::TokenType tokenType = CLangToken::NoToken;
    if (PunctuatorSet.contains(tokenBuffer))
    {
      tokenType = ParsePunctuator(source, tokenBuffer);
    }
    else if (tokenBuffer[0] == '\'')
    {

    }
    else if (tokenBuffer[0] == '"')
    {

    }
    else if (iswdigit(tokenBuffer[0]))
    {

    }
    else
    {
      tokenType = ParseIdentifier(source, tokenBuffer);
    }

    return CLangToken(tokenType, tokenBuffer, m_currentLine, currentColumn);
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

  CLangToken::TokenType CLangLexer::ParsePunctuator(std::wistream& source,
    std::wstring& buffer)
  {
    while (source.good())
    {
      buffer += source.get();
      m_currentColumn += 1;

      if (!PunctuatorSet.contains(buffer) &&
        !(buffer == L".." && source.peek() == '.'))
      {
        buffer.pop_back();
        source.unget();
        m_currentColumn -= 1;
        break;
      }
    }
    return PunctuatorSet.at(buffer);
  }

  CLangToken::TokenType CLangLexer::ParseIdentifier(std::wistream& source,
    std::wstring& buffer)
  {
    // If we need to parse a carriage, we want to do a look ahead for the
    // carriage code.
    if (buffer.back() == '\\')
    {
      buffer.pop_back();
      source.unget();
      m_currentColumn -= 1;
    }

    while (source.good() &&
      source.peek() != std::char_traits<wchar_t>::eof() &&
      !PunctuatorSet.contains(std::wstring{ source.peek() }) &&
      source.peek() != '"' && source.peek() != '\'')
    {
      buffer += source.get();
      m_currentColumn += 1;
      if (buffer.back() == '\\')
      {
        buffer.pop_back();

        if (!ParseUniversalCharacter(source, buffer))
        {
          return CLangToken::BadToken;
        }
      }
    }
    return CLangToken::Identifier;
  }

  bool CLangLexer::ParseUniversalCharacter(std::wistream& source,
    std::wstring& buffer)
  {
    constexpr size_t Utf16CodeSize = 4;
    constexpr size_t Utf32CodeSize = 8;

    if (!source.good() || (source.peek() != 'U' && source.peek() != 'u'))
    {
      return false;
    }

    size_t codeSize = source.get() == 'u' ? Utf16CodeSize : Utf32CodeSize;
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
      return false;
    }

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

    return true;
  }
}
