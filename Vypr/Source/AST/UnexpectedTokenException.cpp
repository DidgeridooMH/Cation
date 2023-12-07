#include "Vypr/AST/UnexpectedTokenException.hpp"

namespace Vypr
{
  UnexpectedTokenException::UnexpectedTokenException(
      const std::string &expectedToken, size_t line, size_t column)
      : message("Expected " + expectedToken + " at line " +
                std::to_string(line) + " column " + std::to_string(column)),
        expectedToken(expectedToken), line(line), column(column)
  {
  }

  const char *UnexpectedTokenException::what() const noexcept
  {
    return message.c_str();
  }
} // namespace Vypr