#include "Vypr/AST/Type/RealType.hpp"

namespace Vypr
{
  RealType::RealType(Real real, bool isConst, bool isLValue)
      : StorageType(StorageMetaType::Real, isConst, isLValue), real(real)
  {
  }
} // namespace Vypr