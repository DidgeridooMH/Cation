#include <cctype>
#include <istream>
#include <string>
#include <unordered_map>
#include "CLangLexer.hpp"
#include "CLangToken.hpp"

namespace Cation {
  static const std::unordered_map<std::wstring, CLangToken::TokenType> PunctuatorSet = {
    {L"[", CLangToken::LeftBracket},
    {L"]", CLangToken::RightBracket},
    {L"(", CLangToken::LeftParenthesis},
    {L")", CLangToken::RightParenthesis},
    {L"{", CLangToken::LeftDragon},
    {L"}", CLangToken::RightDragon},
    {L".", CLangToken::Period},
    {L"...", CLangToken::Variadic}
  };

  CLangLexer::CLangLexer() : m_currentLine(1), m_currentColumn(1) {
  }

  CLangToken CLangLexer::GetToken(std::wistream& source) {
    SkipWhitespace(source);

    if (source.eof()) {
      return CLangToken(CLangToken::NoToken);
    }

    std::wstring tokenBuffer;
    tokenBuffer += source.get();

    CLangToken::TokenType tokenType = CLangToken::NoToken;
    if (PunctuatorSet.contains(tokenBuffer)) {
      tokenType = ParsePunctuator(source, tokenBuffer);
    }

    size_t column = m_currentColumn;
    m_currentColumn += tokenBuffer.size();

    return CLangToken(tokenType, tokenBuffer, m_currentLine, column);
  }

  void CLangLexer::SkipWhitespace(std::wistream& source) {
    while (!source.eof() && isspace(source.peek())) {
      m_currentColumn += 1;
      if (source.peek() == '\n') {
        m_currentLine += 1;
        m_currentColumn = 1;
      }
      source.get();
    }
  }

  CLangToken::TokenType CLangLexer::ParsePunctuator(std::wistream& source,
    std::wstring& buffer) {
    while (!source.eof()) {
      wchar_t lookAhead = source.get();
      buffer += lookAhead;

      if (!PunctuatorSet.contains(buffer) &&
        !(buffer == L".." && source.peek() == '.')) {
        buffer.pop_back();
        source.unget();
        break;
      }
    }
    return PunctuatorSet.at(buffer);
  }
}
