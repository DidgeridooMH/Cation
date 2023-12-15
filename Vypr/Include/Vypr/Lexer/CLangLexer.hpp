#pragma once

#include <exception>
#include <memory>
#include <optional>
#include <string>

#include "Vypr/Lexer/CLangToken.hpp"
#include "Vypr/Scanner/Scanner.hpp"

namespace Vypr
{
  /// <summary>
  /// Exception that can happen while trying to parse an input.
  /// </summary>
  struct ParsingException : std::exception
  {
    ParsingException(std::wstring message, size_t column, size_t line);

    /// <summary>
    /// Brief explanation of the exception that was encountered.
    /// </summary>
    std::wstring message;

    /// <summary>
    /// 1-indexed column that the exception was encountered at.
    /// </summary>
    size_t column;

    /// <summary>
    /// 1-indexed line that the exception was encountered at.
    /// </summary>
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
    CLangLexer(std::unique_ptr<Scanner> scanner);

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
    CLangToken GetToken();

    /// <summary>
    /// Peek a token from the front of the stream. Whitespace is ignored and
    /// tokens are not parsed across vertical whitespace.
    /// </summary>
    /// <param name="source">Stream to peek token from.</param>
    /// <returns>
    /// A <c>CLangToken</c> parsed from the stream. Token is of type
    /// <c>CLangToken::NoToken</c> when EOF is reached or a token
    /// couldn't be parsed.
    /// </returns>
    CLangToken PeekToken();

  private:
    /// <summary>
    /// Parses a punctuator from the source.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>Token of the punctuator.</returns>
    CLangToken ParsePunctuator();

    /// <summary>
    /// Parses an identifier or keyword from source.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>Token of the keyword or identifier.</returns>
    CLangToken ParseIdentifier();

    /// <summary>
    /// Parses a unicode character from source. Front of source should be either
    /// <c>'u'</c> for UTF-16 or <c>'U'</c> for UTF-32.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>
    /// String containing the unicode character.
    /// </returns>
    std::wstring ParseUniversalCharacter();

    /// <summary>
    /// Parses a numerical constant from source.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>
    /// Token containing either a float constant or integer constant.
    /// </returns>
    CLangToken ParseNumericalConstant();

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
    std::wstring ParseBinaryConstant();

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
    std::tuple<bool, std::wstring> ParseFloatableConstant(
        std::wstring (CLangLexer::*parser)(), char exponentDelimiter,
        bool requireExponent);

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
    std::wstring ParseHexadecimalSequence();

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
    std::wstring ParseIntegerSequence();

    /// <summary>
    /// Parses an integer suffix of u, U, l, L, ll, LL.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>
    /// Integer suffix or empty string if one is not available.
    /// </returns>
    std::wstring ParseIntegerSuffix();

    /// <summary>
    /// Parses an float suffix of f, F, l, L.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>
    /// Float suffix or empty string if one is not available.
    /// </returns>
    std::wstring ParseFloatSuffix();

    /// <summary>
    /// Parses a character literal
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>
    /// Character literal sans the quotation.
    /// </returns>
    CLangToken ParseCharacterConstant();

    /// <summary>
    /// Parse an escape sequence.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>
    /// Character derived from the escape sequence.
    /// </returns>
    wchar_t ParseEscapeSequence();

    /// <summary>
    /// Parse a string literal.
    /// </summary>
    /// <param name="source">Stream of wchar_t.</param>
    /// <returns>
    /// String literal sans the quotation.
    /// </returns>
    CLangToken ParseStringLiteral();

    std::unique_ptr<Scanner> m_scanner;
    std::optional<CLangToken> m_lookAheadBuffer;
  };
} // namespace Vypr
