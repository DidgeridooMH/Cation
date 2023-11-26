#pragma once

#include <iosfwd>
#include <string>
#include "CLangToken.hpp"

namespace Cation {
  /// <summary>
  /// C language lexer. Parses a raw file or string stream into tokens dfiend
  /// in the C language grammar. Only metadata about a stream is held by the
  /// lexer. Streams are passed when requesting a token.
  /// </summary>
  class CLangLexer
  {
  public:
    /// <summary>
    /// Constructs a new lexer.
    /// </summary>
    CLangLexer();

    /// <summary>
    /// Fetch a token from the front of the stream. Whitespace is ignored and
    /// tokens are not parsed across vertical whitespace.
    /// </summary>
    /// <param name="source">Stream to fetch token from.</param>
    /// <returns>
    /// A <code>CLangToken</code> parsed from the stream. Token is of type
    /// <code>CLangToken::NoToken</code> when EOF is reached or a token
    /// couldn't be parsed.
    /// </returns>
    CLangToken GetToken(std::wistream& source);

  private:
    void SkipWhitespace(std::wistream& source);
    CLangToken::TokenType ParsePunctuator(std::wistream& source,
      std::wstring& buffer);

    size_t m_currentLine;
    size_t m_currentColumn;
  };
}

