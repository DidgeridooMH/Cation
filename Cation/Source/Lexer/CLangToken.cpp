#include <string>
#include "CLangToken.hpp"

namespace Cation {
  CLangToken::CLangToken(TokenType type, std::wstring content, size_t line,
    size_t column)
    : m_type(type),
    m_content(content),
    m_line(line),
    m_column(column) {}
}
