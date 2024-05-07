#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Vypr/Lexer/CLangTokenType.hpp"

namespace Vypr
{
  /// @brief Helper for mapping strings to their token types.
  using CLangTokenMap = std::unordered_map<std::wstring, CLangTokenType>;

  /// @brief Helper for checking token membership.
  using CLangTokenSet = std::unordered_set<CLangTokenType>;

  /// @brief Map keyword token types using their string values.
  extern const CLangTokenMap KeywordMap;

  /// @brief Map punctuation token types using their string values.
  extern const CLangTokenMap PunctuatorMap;

  /// @brief A set of tokens that can prefix a type or are a type.
  extern const CLangTokenSet TypePrefixSet;
} // namespace Vypr
