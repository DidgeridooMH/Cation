#pragma once

#include <string>

namespace Cation {
  /// <summary>
  /// Token emitted by the lexer. These tokens are defined in the C language
  /// grammar. Identifiers and constants contain the text they were parsed
  /// with.
  /// </summary>
  class CLangToken {
  public:
    enum TokenType {
      NoToken,
      Keyword,
      Identifier,
      Constant,
      StringLiteral,
      LeftBracket,
      RightBracket,
      LeftParenthesis,
      RightParenthesis,
      LeftDragon,
      RightDragon,
      Period,
      Arrow,
      Increment,
      Decrement,
      Star,
      Add,
      Subtract,
      Tilde,
      Exclamation,
      Divide,
      Modulo,
      ShiftLeft,
      ShiftRight,
      LessThan,
      GreaterThan,
      LessEqual,
      GreaterEqual,
      Equal,
      NotEqual,
      And,
      Or,
      Xor,
      LogicalAnd,
      LogicalOr,
      TernaryProposition,
      TernaryDecision,
      StatementDelimiter,
      Variadic,
      Assign,
      MultiplyAssign,
      DivideAssign,
      ModuloAssign,
      AddAssign,
      SubtractAssign,
      LeftShiftAssign,
      RightShiftAssign,
      AndAssign,
      XorAssign,
      OrAssign,
      Comma,
      Preprocessor,
      PreprocessorConcat
    };

    /// <summary>
    /// Creates a token.
    /// </summary>
    /// <param name="type">Type of keyword, identifier, constant or token.</param>
    /// <param name="content">Raw string used to create token.</param>
    /// <param name="line">1-indexed line of the file token was created from.</param>
    /// <param name="column">1-indexed column of the file token was created from.</param>
    CLangToken(TokenType type, std::wstring content = L"", size_t line = 0,
      size_t column = 0);

    /// <returns>The type of the token.</returns>
    inline TokenType GetType() const {
      return m_type;
    }

    /// <returns>Raw string used to create token.</returns>
    inline const std::wstring& GetContent() const {
      return m_content;
    }

    /// <returns>1-indexed line of the file token was created from.</returns>
    inline size_t GetLine() const {
      return m_line;
    }

    /// <returns>1-indexed column of the file token was created from.</returns>
    inline size_t GetColumn() const {
      return m_column;
    }

  private:
    TokenType m_type;
    std::wstring m_content;
    size_t m_line;
    size_t m_column;
  };
}
