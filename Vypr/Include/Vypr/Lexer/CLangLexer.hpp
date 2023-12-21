#pragma once

#include <exception>
#include <memory>
#include <optional>
#include <string>

#include "Vypr/Lexer/CLangToken.hpp"
#include "Vypr/Scanner/Scanner.hpp"

namespace Vypr
{
  /// @brief Exception that can happen while trying to parse an input.
  struct ParsingException : std::exception
  {
    ParsingException(std::wstring message, size_t column, size_t line);

    /// @brief Brief explanation of the exception that was encountered.
    std::wstring message;

    /// @brief 1-indexed column that the exception was encountered at.
    size_t column;

    /// @brief 1-indexed line that the exception was encountered at.
    size_t line;
  };

  /// @brief C language lexer. Parses a raw file or string stream into tokens
  /// defined in the C language grammar.
  ///
  /// Only metadata about a stream is held by the lexer. Streams are passed when
  /// requesting a token.
  class CLangLexer
  {
  public:
    CLangLexer(std::unique_ptr<Scanner> scanner);

    /// @brief Fetch a token from the front of the stream. Whitespace is ignored
    /// and tokens are not parsed across vertical whitespace.
    ///
    /// @param source Stream to fetch token from.
    /// @returns A `CLangToken` parsed from the stream. Token is of type
    /// `CLangToken::NoToken` when EOF is reached or a token
    /// couldn't be parsed.
    CLangToken GetToken();

    /// @brief Peek a token from the front of the stream. Whitespace is ignored
    /// and tokens are not parsed across vertical whitespace.
    ///
    /// @param source Stream to peek token from.
    /// @returns A `CLangToken` parsed from the stream. Token is of
    /// type `CLangToken::NoToken` when EOF is reached or a token
    /// couldn't be parsed.
    CLangToken PeekToken();

  private:
    /// @brief Parses a punctuator from the source.
    ///
    /// @param source Stream of `wchar_t`.
    /// @returns Token of the punctuator.
    CLangToken ParsePunctuator();

    /// @brief Parses an identifier or keyword from source.
    ///
    /// @param source Stream of `wchar_t`.
    /// @returns Token of the keyword or identifier.
    CLangToken ParseIdentifier();

    /// @brief Parses a unicode character from source. Front of source should be
    /// either
    /// `'u'` for UTF-16 or `'U'` for UTF-32.
    ///
    /// @param source Stream of `wchar_t`.
    /// @returns String containing the unicode character.
    std::wstring ParseUniversalCharacter();

    /// @brief Parses a numerical constant from source.
    ///
    /// @param source Stream of `wchar_t`.
    /// @returns Token containing either a float constant or integer constant.
    CLangToken ParseNumericalConstant();

    /// @brief Parses and convert a binary number from source.
    ///
    /// @param source Stream of `wchar_t`.
    /// @returns Binary number converted to decimal.
    ///
    /// @throws `ParsingException` Thrown when a binary number contains values
    /// other than 0 and 1.
    std::wstring ParseBinaryConstant();

    /// @brief Parses a number with a integral part, an optional fraction part,
    /// and an optional exponent part.
    ///
    /// @param source Stream of `wchar_t`.
    /// @param parser Function to parse integral and fraction parts
    /// @param exponentDelimiter Delimiter between fraction and exponent.
    /// @param requireExponent Require exponent if floating point.
    /// @returns True if floating point and a integer or floating point string.
    std::tuple<bool, std::wstring> ParseFloatableConstant(
        std::wstring (CLangLexer::*parser)(), char exponentDelimiter,
        bool requireExponent);

    /// @brief Parses a hexadecimal number from source.
    ///
    /// @param source Stream of `wchar_t`.
    /// @returns Hexadecimal number in string format.
    ///
    /// @throws `ParsingException` Thrown when a hexadecimal number contains
    /// values other [0-9a-zA-Z].
    std::wstring ParseHexadecimalSequence();

    /// @brief Parses integer from source.
    ///
    /// @param source Stream of `wchar_t`.
    /// @returns Decimal integer.
    ///
    /// @throws `ParsingException` Thrown if sequence not found.
    std::wstring ParseIntegerSequence();

    /// @brief Parses an integer suffix of u, U, l, L, ll, LL.
    ///
    /// @param source Stream of `wchar_t`.
    /// @returns Integer suffix or empty string if one is not available.
    std::wstring ParseIntegerSuffix();

    /// @brief Parses an float suffix of f, F, l, L.
    ///
    /// @param source Stream of `wchar_t`.
    /// @returns Float suffix or empty string if one is not available.
    std::wstring ParseFloatSuffix();

    /// @brief Parses a character literal
    ///
    /// @param source Stream of `wchar_t`.
    ///  @returns     /// Character literal sans the quotation.
    ///
    CLangToken ParseCharacterConstant();

    /// @brief Parse an escape sequence.
    ///
    /// @param source Stream of `wchar_t`.
    /// @returns Character derived from the escape sequence.
    wchar_t ParseEscapeSequence();

    /// @brief Parse a string literal.
    ///
    /// @param source Stream of `wchar_t`.
    /// @returns String literal sans the quotation.
    CLangToken ParseStringLiteral();

    std::unique_ptr<Scanner> m_scanner;
    std::optional<CLangToken> m_lookAheadBuffer;
  };
} // namespace Vypr
