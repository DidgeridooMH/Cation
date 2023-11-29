#pragma once

#include <exception>
#include <iosfwd>
#include <string>
#include "Vypr/Lexer/CLangToken.hpp"

// TODO: Replace istream with a variant that counts col,line for you.
namespace Vypr
{
  // TODO: Document
  struct ParsingException : std::exception
  {
    ParsingException(std::wstring message, size_t column, size_t line);

    std::wstring message;
    size_t column;
    size_t line;
  };

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
    /// A <c>CLangToken</c> parsed from the stream. Token is of type
    /// <c>CLangToken::NoToken</c> when EOF is reached or a token
    /// couldn't be parsed.
    /// </returns>
    [[nodiscard]]
    CLangToken GetToken(std::wistream &source);

  private:
    // TODO: Document
    CLangToken ParsePunctuator(std::wistream &source);
    CLangToken ParseIdentifier(std::wistream &source);
    std::wstring ParseUniversalCharacter(std::wistream &source);
    CLangToken ParseNumericalConstant(std::wistream &source);

    /// <summary>
    /// Parses and convert a binary number from source.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>
    /// Binary number converted to decimal.
    /// </returns>
    /// <exception cref="ParsingException">
    /// Thrown when a binary number contains values other than 0 and 1.
    /// </exception>
    [[nodiscard]]
    std::wstring ParseBinaryConstant(std::wistream &source);

    /// <summary>
    /// Parses a number with a integral part, an optional fraction part, and
    /// an optional exponent part.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <param name="parser">Function to parse integral and fraction
    /// parts</param> <param name="exponentDelimiter">Delimiter between fraction
    /// and exponent.</param> <param name="requireExponent">Require exponent if
    /// floating point.</param> <returns>True if floating point and a integer or
    /// floating point string.</returns>
    [[nodiscard]]
    std::tuple<bool, std::wstring> ParseFloatableConstant(
        std::wistream &source,
        std::wstring (CLangLexer::*parser)(std::wistream &),
        char exponentDelimiter, bool requireExponent);

    /// <summary>
    /// Parses a hexadecimal number from source.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>
    /// Hexadecimal number in string format.
    /// </returns>
    /// <exception cref="ParsingException">
    /// Thrown when a hexadecimal number contains values other [0-9a-zA-Z].
    /// </exception>
    [[nodiscard]]
    std::wstring ParseHexadecimalSequence(std::wistream &source);

    /// <summary>
    /// Parses integer from source.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>
    /// Decimal integer.
    /// </returns>
    /// <exception cref="ParsingException">
    /// Thrown if sequence not found.
    /// </exception>
    [[nodiscard]]
    std::wstring ParseIntegerSequence(std::wistream &source);

    /// <summary>
    /// Parses an integer suffix of u, U, l, L, ll, LL.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>
    /// Integer suffix or empty string if one is not available.
    /// </returns>
    [[nodiscard]]
    std::wstring ParseIntegerSuffix(std::wistream &source);

    /// <summary>
    /// Parses an float suffix of f, F, l, L.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>
    /// Float suffix or empty string if one is not available.
    /// </returns>
    [[nodiscard]]
    std::wstring ParseFloatSuffix(std::wistream &source);

    /// <summary>
    /// Skips the stream past any whitespace.
    /// </summary>
    void SkipWhitespace(std::wistream &source);

    size_t m_currentLine;
    size_t m_currentColumn;
  };
} // namespace Vypr
