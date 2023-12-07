#include <exception>
#include <string>

namespace Vypr
{
  struct TypeException : public std::exception
  {
    TypeException(std::string message, size_t column, size_t line)
        : message(message), column(column), line(line)
    {
    }

    const char *what() const noexcept override
    {
      return message.c_str();
    }

    std::string message;
    size_t column;
    size_t line;
  };
} // namespace Vypr