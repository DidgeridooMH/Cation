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

  CLangLexer::CLangLexer() : m_currentLine(1), m_currentColumn(1) { }

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
