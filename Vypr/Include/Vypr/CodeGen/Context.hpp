#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <memory>
#include <string>

#include "Vypr/AST/SymbolTable.hpp"

namespace Vypr
{
  struct Context
  {
    Context(const std::string &name);

    void PrettyPrint() const;

    void Verify() const;

    void GenerateObjectFile(const std::string &filename);

    llvm::LLVMContext context;
    llvm::Module module;
    llvm::IRBuilder<> builder;
    SymbolTable<llvm::AllocaInst *> symbolTable;
  };
} // namespace Vypr