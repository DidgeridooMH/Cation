#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <string>

#include "Vypr/AST/SymbolTable.hpp"

namespace Vypr
{
  struct Context
  {
    Context(const std::string &name) : module(name, context), builder(context)
    {
      symbolTable.PushScope();
    }

    void PrettyPrint() const
    {
      module.print(llvm::outs(), nullptr);
    }

    void Verify() const
    {
      llvm::verifyModule(module, &llvm::errs());
    }

    void GenerateObjectFile(const std::string &filename)
    {
      auto targetTriple = llvm::sys::getDefaultTargetTriple();
      module.setTargetTriple(targetTriple);

      std::string error;
      auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

      std::string cpu = "generic";
      std::string features = "";

      llvm::TargetOptions opt;
      llvm::TargetMachine *targetMachine = target->createTargetMachine(
          targetTriple, cpu, features, opt, llvm::Reloc::PIC_);

      module.setDataLayout(targetMachine->createDataLayout());

      std::error_code ec;
      llvm::raw_fd_ostream dest(filename, ec, llvm::sys::fs::OF_None);

      llvm::legacy::PassManager pass;
      auto fileType = llvm::CodeGenFileType::ObjectFile;

      targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType);

      pass.run(module);
      dest.flush();
    }

    llvm::LLVMContext context;
    llvm::Module module;
    llvm::IRBuilder<> builder;
    SymbolTable<llvm::AllocaInst *> symbolTable;
  };
} // namespace Vypr