#pragma once

#include <memory>

#include "Vypr/AST/Type/StorageType.hpp"

namespace Vypr
{
  enum class Real
  {
    Float,
    Double,
    LongDouble
  };

  class RealType : public StorageType
  {
  public:
    RealType(Real real, bool isConst, bool isLValue);

    Real real;
  };

} // namespace Vypr