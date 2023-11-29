#pragma once

#include "Vypr/Scanner/Scanner.hpp"

namespace Vypr
{
  class StringScanner : public Scanner
  {
  public:
    /// <summary>
    /// Constructs a scanner from the source string. The string is copied to an
    /// internal buffer.
    /// </summary>
    /// <param name="source">
    /// String to retrieve characters from.
    /// </param>
    StringScanner(std::wstring source = L"");

    wchar_t LookAhead(int stepSize) const override;
    bool Finished() override;

  protected:
    wchar_t NextInternal() override;

  private:
    std::wstring m_buffer;
    size_t m_index;
  };
} // namespace Vypr