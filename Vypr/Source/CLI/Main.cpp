#include <iostream>
#include <llvm/Support/TargetSelect.h>
#include <string>

#include "Vypr/AST/Expression/ExpressionNode.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/TypeException.hpp"
#include "Vypr/AST/UnexpectedTokenException.hpp"
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

  // Temp
  llvm::Function *function = llvm::Function::Create(
      llvm::FunctionType::get(llvm::Type::getInt1Ty(context->context), false),
      llvm::Function::ExternalLinkage, "TestFunction", context->module);

  llvm::BasicBlock *block =
      llvm::BasicBlock::Create(context->context, "entry", function);
  context->builder.SetInsertPoint(block);

  // Temp

  Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"var == abc"));

  try
  {
    Vypr::TypeTable typeTable;
    typeTable.PushScope();
    typeTable.AddSymbol(L"var", std::make_shared<Vypr::IntegralType>(
                                    Vypr::Integral::Bool, false, false, true));
    typeTable.AddSymbol(L"abc", std::make_shared<Vypr::IntegralType>(
                                    Vypr::Integral::Int, false, false, true));

    llvm::AllocaInst *variable = context->builder.CreateAlloca(
        context->builder.getInt1Ty(), nullptr, "var");
    context->builder.CreateStore(context->builder.getInt1(true), variable);
    context->symbolTable.AddSymbol(L"var", variable);

    llvm::AllocaInst *abc = context->builder.CreateAlloca(
        context->builder.getInt32Ty(), nullptr, "abc");
    context->builder.CreateStore(context->builder.getInt32(32), abc);
    context->symbolTable.AddSymbol(L"abc", abc);

    auto expression = Vypr::ExpressionNode::Parse(lexer, typeTable);
    std::wcout << expression->PrettyPrint(0) << std::endl;

    llvm::Value *ret = expression->GenerateCode(*context);

    // Temp
    context->builder.CreateRet(ret);
    // Temp
  }
  catch (Vypr::TypeException &e)
  {
    std::wcerr << L"TypeException: " << e.what() << " [" << e.column << ","
               << e.line << "]" << std::endl;
  }
  catch (Vypr::UnexpectedTokenException &e)
  {
    std::wcerr << L"UnexpectedTokenException: " << e.what() << " [" << e.column
               << "," << e.line << "]" << std::endl;
  }

  context->Verify();
  context->PrettyPrint();
  context->GenerateObjectFile("module.o");

  return 0;
}
