#include <iostream>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <string>

#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Expression/ExpressionNode.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/CodeGen/Context.hpp"
#include "Vypr/Lexer/CLangLexer.hpp"
#include "Vypr/Scanner/StringScanner.hpp"

int main(int, char **)
{
  auto context = std::make_unique<Vypr::Context>("module");

  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  Vypr::CLangLexer lexer(
      std::make_unique<Vypr::StringScanner>(L"1.0f + (3 & 1)"));

  try
  {
    Vypr::TypeTable typeTable;
    typeTable.AddSymbol(L"var", std::make_shared<Vypr::IntegralType>(
                                    Vypr::Integral::Int, false, false, true));

    auto expression = Vypr::ExpressionNode::Parse(lexer, typeTable);
    std::wcout << expression->PrettyPrint(0) << std::endl;

    // Temp
    llvm::Function *function = llvm::Function::Create(
        llvm::FunctionType::get(expression->type->GetIRType(*context), false),
        llvm::Function::ExternalLinkage, "TestFunction", context->module);
    llvm::BasicBlock *block =
        llvm::BasicBlock::Create(context->context, "entry", function);
    context->builder.SetInsertPoint(block);

    llvm::GlobalVariable *variable = new llvm::GlobalVariable(
        context->module, context->builder.getInt32Ty(), false,
        llvm::GlobalValue::InternalLinkage, context->builder.getInt32(42));
    context->builder.CreateStore(context->builder.getInt32(42), variable);
    context->symbolTable.AddSymbol(L"var", variable);
    // Temp

    llvm::Value *ret = expression->GenerateCode(*context);

    // Temp
    context->builder.CreateRet(ret);
    // Temp

    if (!llvm::verifyFunction(*function, &llvm::errs()) && context->Verify())
    {
      context->PrettyPrint();
      context->GenerateObjectFile("module.o");
    }
  }
  catch (Vypr::CompileError &e)
  {
    std::wcerr << e.what() << std::endl;
  }

  return 0;
}
