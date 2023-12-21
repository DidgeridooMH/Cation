#pragma once

#include <string>
#include <unordered_map>

#include "Vypr/Lexer/CLangTokenType.hpp"

namespace Vypr
{
  /// @brief Helper for mapping strings to their token types.
  using CLangTokenMap = std::unordered_map<std::wstring, CLangTokenType>;

  /// @brief Map keyword token types using their string values.
  extern const CLangTokenMap KeywordMap;

  /// @brief Map punctuation token types using their string values.
  extern const CLangTokenMap PunctuatorMap;
} // namespace Vypr