#pragma once

#include <string>

#include "Vypr/Lexer/CLangTokenType.hpp"

namespace Vypr
{
  /// @brief Token emitted by the lexer. These tokens are defined in the C
  /// language grammar.
  ///
  /// Identifiers and constants contain the text they were parsed
  /// with.
  struct CLangToken
  {
    /// @brief No position is defined for the line or column;
    static constexpr size_t NoPosition = 0;

    /// @brief Type of keyword, identifier, constant or token.
    CLangTokenType type = CLangTokenType::NoToken;

    /// @brief Raw string used to create token.
    std::wstring content = L"";

    /// @brief 1-indexed line of the file token was created from.
    size_t line = NoPosition;

    /// @brief 1-indexed column of the file token was created from.
    size_t column = NoPosition;
  };
} // namespace Vypr
