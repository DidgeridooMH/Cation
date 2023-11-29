#pragma once

#include <string>

namespace Vypr
{
  /// <summary>
  /// Retrieves characters from a source and keeps track of line and column
  /// counts.
  /// </summary>
  class Scanner
  {
  public:
    /// <summary>
    /// Initializes line and column state.
    /// </summary>
    Scanner();

    /// <summary>
    /// Builds a string from the scanner while <c>condition</c> holds true or
    /// there is no more characters in the source.
    /// </summary>
    /// <param name="condition">
    /// Condition function if true continues to append
    /// characters to resulting string.
    /// </param>
    /// <returns>
    /// String prefix of the
    /// scanner source such that all resulting character hold true for
    /// <c>condition</c>.
    /// </returns>
    std::wstring NextWhile(bool (*condition)(wchar_t));

    /// <returns>
    /// 1-indexed line count of the current cursor.
    /// </returns>
    inline size_t GetLine() const
    {
      return m_line;
    };

    /// <returns>
    /// 1-indexed column count of the current cursor.
    /// </returns>
    inline size_t GetColumn() const
    {
      return m_column;
    };

    /// <summary>
    /// Retrieves the next character from the scanner source or <c>EOF</c> if
    /// the source is empty. Increments the line and column counts accordingly.
    /// </summary>
    /// <returns>
    /// Next character in the scanner source.
    /// </returns>
    wchar_t Next();

    /// <summary>
    /// Retrieves <c>max(length,remaining)</c> characters from the scanner
    /// source. Increments the line and column counts accordingly.
    /// </summary>
    /// <param name="length">
    /// Number of characters to attempt to retrieve.
    /// </param>
    /// <returns>
    /// String of the front <c>length</c> characters. The resulting string can
    /// have less than <c>length</c> characters if the source does not have that
    /// many.
    /// </returns>
    std::wstring Next(size_t length);

    /// <summary>
    /// Peeks <c>max(length,remaining)</c> characters from the scanner
    /// source.
    /// </summary>
    /// <param name="offset">
    /// Number of characters to skip when looking ahead.
    /// </param>
    /// <param name="length">
    /// Number of characters to attempt to retrieve.
    /// </param>
    /// <returns>
    /// String of the front <c>length</c> characters. The resulting string can
    /// have less than <c>length</c> characters if the source does not have that
    /// many.
    /// </returns>
    std::wstring LookAhead(int offset, int length) const;

    /// <summary>
    /// Peeks the next character from the scanner source or <c>EOF</c> if
    /// the source is empty.
    /// </summary>
    /// <param name="offset">
    /// Number of characters to skip when looking ahead.
    /// </param>
    /// <returns>
    /// <c>offset</c>th character in the scanner source.
    /// </returns>
    virtual wchar_t LookAhead(int offset) const = 0;

    /// <returns>
    /// Whether more characters are available from the scanner
    /// source.
    /// </returns>
    virtual bool Finished() = 0;

  private:
    virtual wchar_t NextInternal() = 0;

    size_t m_column;
    size_t m_line;
  };
} // namespace Vypr