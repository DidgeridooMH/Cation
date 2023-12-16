#include "Vypr/AST/Expression/VariableNode.hpp"

#include <gtest/gtest.h>

#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/PointerType.hpp"
#include "Vypr/AST/Type/RealType.hpp"
#include "Vypr/Lexer/CLangLexer.hpp"
#include "Vypr/Scanner/StringScanner.hpp"

namespace VariableNodeTest
{
  TEST(Parse, InvalidVariableName)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"34"));
    Vypr::TypeTable typeTable;
    ASSERT_THROW(Vypr::VariableNode::Parse(lexer, typeTable),
                 Vypr::CompileError);
  }

  TEST(Parse, UndefinedVariable)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"alpha"));
    Vypr::TypeTable typeTable;
    ASSERT_THROW(Vypr::VariableNode::Parse(lexer, typeTable),
                 Vypr::CompileError);
  }

  TEST(Parse, IntegralVariable)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"alpha"));
    Vypr::TypeTable typeTable;
    typeTable.AddSymbol(L"alpha", std::make_shared<Vypr::IntegralType>(
                                      Vypr::Integral::Int, false, false, true));

    std::unique_ptr<Vypr::VariableNode> variable =
        Vypr::VariableNode::Parse(lexer, typeTable);

    ASSERT_EQ(variable->column, 1);
    ASSERT_EQ(variable->line, 1);
    ASSERT_EQ(variable->type->GetType(), Vypr::StorageMetaType::Integral);
    ASSERT_EQ(
        dynamic_cast<Vypr::IntegralType *>(variable->type.get())->integral,
        Vypr::Integral::Int);
    ASSERT_EQ(variable->type->isLValue, true);
    ASSERT_EQ(variable->type->isConst, false);
  }

  TEST(Parse, RealVariable)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"alpha"));
    Vypr::TypeTable typeTable;
    typeTable.AddSymbol(L"alpha", std::make_shared<Vypr::RealType>(
                                      Vypr::Real::Float, false, true));

    std::unique_ptr<Vypr::VariableNode> variable =
        Vypr::VariableNode::Parse(lexer, typeTable);

    ASSERT_EQ(variable->column, 1);
    ASSERT_EQ(variable->line, 1);
    ASSERT_EQ(variable->type->GetType(), Vypr::StorageMetaType::Real);
    ASSERT_EQ(dynamic_cast<Vypr::RealType *>(variable->type.get())->real,
              Vypr::Real::Float);
    ASSERT_EQ(variable->type->isLValue, true);
    ASSERT_EQ(variable->type->isConst, false);
  }

  TEST(Parse, PointerVariable)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"beta"));
    Vypr::TypeTable typeTable;
    std::unique_ptr<Vypr::StorageType> storage =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Bool, false, false,
                                             true);
    typeTable.AddSymbol(
        L"beta", std::make_shared<Vypr::PointerType>(storage, false, true));

    std::unique_ptr<Vypr::VariableNode> variable =
        Vypr::VariableNode::Parse(lexer, typeTable);

    ASSERT_EQ(variable->column, 1);
    ASSERT_EQ(variable->line, 1);
    ASSERT_EQ(variable->type->GetType(), Vypr::StorageMetaType::Pointer);
    ASSERT_EQ(variable->type->isLValue, true);
    ASSERT_EQ(variable->type->isConst, false);
  }
} // namespace VariableNodeTest