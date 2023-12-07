#pragma once

#include <memory>
#include <vector>

#include "Vypr/AST/Type/StorageType.hpp"

namespace
{
  class FunctionDesignator
  {
  public:
    FunctionDesignator(std::unique_ptr<StorageType> &&returnType,
                       std::vector<std::unique_ptr<StorageType>> &&arguments,
                       bool isVariadic)
        : m_returnType(std::move(returnType)),
          m_arguments(std::move(arguments)), m_isVariadic(isVariadic)
    {
    }

  private:
    std::unique_ptr<StorageType> m_returnType;
    std::vector<std::unique_ptr<StorageType>> m_arguments;
    bool m_isVariadic;
  };
} // namespace