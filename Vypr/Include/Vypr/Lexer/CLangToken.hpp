#pragma once

#include <string>
#include "Vypr/Lexer/CLangTokenType.hpp"

namespace Vypr
{
  /// <summary>
  /// Token emitted by the lexer. These tokens are defined in the C language
  /// grammar. Identifiers and constants contain the text they were parsed
  /// with.
  /// </summary>
  struct CLangToken
  {
    static constexpr size_t NoPosition = 0;

    /// <summary>Type of keyword, identifier, constant or token.</summary>
    CLangTokenType type = CLangTokenType::NoToken;

    /// <summary>Raw string used to create token.</summary>
    std::wstring content = L"";

    /// <summary>1-indexed line of the file token was created from.</summary>
    size_t line = NoPosition;

    /// <summary>1-indexed column of the file token was created from.</summary>
    size_t column = NoPosition;
  };
} // namespace Vypr
