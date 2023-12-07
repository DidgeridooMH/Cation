#pragma once

namespace Vypr
{
  enum class UnaryOp
  {
    Increment,
    Decrement,
    Negate,
    LogicalNot,
    Not,
    Deref,
    AddressOf,
    Sizeof
  };
} // namespace Vypr