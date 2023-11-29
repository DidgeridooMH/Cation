#pragma once

#include <string>
#include <unordered_map>

#include "Vypr/Lexer/CLangTokenType.hpp"

namespace Vypr
{
  /// <summary>
  /// Helper for mapping strings to their token types.
  /// </summary>
  using CLangTokenMap = std::unordered_map<std::wstring, CLangTokenType>;

  /// <summary>
  /// Map keyword token types using their string values.
  /// </summary>
  extern const CLangTokenMap KeywordMap;

  /// <summary>
  /// Map punctuation token types using their string values.
  /// </summary>
  extern const CLangTokenMap PunctuatorMap;
} // namespace Vypr