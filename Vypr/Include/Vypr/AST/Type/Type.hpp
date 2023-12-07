#pragma once

namespace Vypr
{
  enum class MetaType
  {
    Storage,
    FunctionDesignator
  };

  class Type
  {
  public:
    Type(MetaType type) : m_type(type)
    {
    }

  private:
    MetaType m_type;
  };
} // namespace Vypr