#pragma once

namespace Vypr
{
  template <class... Ts> struct overloaded : Ts...
  {
    using Ts::operator()...;
  };

#ifdef __APPLE__
  template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
#endif
} // namespace Vypr