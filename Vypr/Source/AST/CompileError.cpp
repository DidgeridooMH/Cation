#include "Vypr/AST/CompileError.hpp"

#include <codecvt>
#include <locale>
#include <unordered_map>

namespace Vypr
{
  CompileError::CompileError(CompileErrorId id, size_t line, size_t column,
                             std::wstring content)
      : id(id), line(line), column(column),
        description(GetCompileErrorMessage(id))
  {
    constexpr size_t ZeroPadLength = 4;

    if (!content.empty())
    {
      description += L" at " + content;
    }

    std::wstring message = L"[" + std::to_wstring(line) + L", " +
                           std::to_wstring(column) + L"] CE";

    std::wstring errorId = std::to_wstring(static_cast<int>(id));
    message += std::wstring(ZeroPadLength - errorId.length(), L'0') + errorId;

    message += L": " + description;

    m_message =
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(
            message);
  }

  std::wstring CompileError::GetCompileErrorMessage(CompileErrorId id)
  {
    switch (id)
    {
    case CompileErrorId::ExpectedExpression:
      return L"Expected expression";
    case CompileErrorId::ExpectedConstant:
      return L"Expected constant";
    case CompileErrorId::ExpectedIdentifier:
      return L"Expected identifier";
    case CompileErrorId::ExpectedGroupEnd:
      return L"Expected group end";
    case CompileErrorId::UndefinedSymbol:
      return L"Undefined symbol";
    case CompileErrorId::InvalidOperands:
      return L"Invalid operands";
    }

    return L"Unknown error";
  }

  const char *CompileError::what() const noexcept
  {
    return m_message.c_str();
  }
} // namespace Vypr