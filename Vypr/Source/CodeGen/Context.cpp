#pragma once

#include "Vypr/CodeGen/Context.hpp"

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>

#include "Vypr/AST/SymbolTable.hpp"

namespace Vypr
{
  Context::Context(const std::string &name)
      : module(name, context), builder(context)
  {
    symbolTable.PushScope();
  }

  void Context::PrettyPrint() const
  {
    module.print(llvm::outs(), nullptr);
  }

  void Context::Verify() const
  {
    llvm::verifyModule(module, &llvm::errs());
  }

  void Context::GenerateObjectFile(const std::string &filename)
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
} // namespace Vypr