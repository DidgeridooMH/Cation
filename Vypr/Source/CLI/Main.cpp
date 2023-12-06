#include <iostream>
#include <string>

#include "Vypr/AST/Expression/ExpressionNode.hpp"
#include "Vypr/Lexer/CLangLexer.hpp"
#include "Vypr/Scanner/StringScanner.hpp"

int main(int, char **)
{
  Vypr::CLangLexer lexer(
      std::make_unique<Vypr::StringScanner>(L"4 + 3.0 - 2.0f / 1 && 1"));

  try
  {
    auto expression = Vypr::ExpressionNode::Parse(lexer);
    std::wcout << expression->PrettyPrint(0) << std::endl;
  }
  catch (Vypr::TypeException &e)
  {
    std::wcerr << e.what() << " [" << e.column << "," << e.line << "]"
               << std::endl;
  }

  return 0;
}
