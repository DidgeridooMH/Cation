#pragma once

#include <exception>
#include <string>

namespace Vypr
{
  enum class CompileErrorId
  {
    ExpectedExpression,
    ExpectedConstant,
    ExpectedIdentifier,
    ExpectedGroupEnd,
    UndefinedSymbol,
    InvalidOperands,
    ConstantTooLarge
  };

  class CompileError : std::exception
  {
  public:
    CompileError(CompileErrorId id, size_t line, size_t column,
                 std::wstring content = L"");

    const char *what() const noexcept override;

    CompileErrorId id;
    size_t line;
    size_t column;
    std::wstring description;

  private:
    static std::wstring GetCompileErrorMessage(CompileErrorId);

    std::string m_message;
  };
} // namespace Vypr