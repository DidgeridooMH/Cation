#pragma once

#include <exception>
#include <string>

namespace Vypr
{
  class UnexpectedTokenException : std::exception
  {
  public:
    UnexpectedTokenException(const std::string &expectedToken, size_t line,
                             size_t column);
    const char *what() const noexcept override;

    std::string expectedToken;
    size_t line;
    size_t column;

  private:
    std::string message;
  };
} // namespace Vypr