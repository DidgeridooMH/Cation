#include <iostream>
#include <string>

#include "Vypr/AST/Expression/ExpressionNode.hpp"
#include "Vypr/AST/Type/TypeException.hpp"
#include "Vypr/AST/UnexpectedTokenException.hpp"
#include "Vypr/Lexer/CLangLexer.hpp"
#include "Vypr/Scanner/StringScanner.hpp"

int main(int, char **)
{
  Vypr::CLangLexer lexer(
      std::make_unique<Vypr::StringScanner>(L"(2 + 2 * (2 + 4)) == 3"));

  try
  {
    auto expression = Vypr::ExpressionNode::Parse(lexer);
    std::wcout << expression->PrettyPrint(0) << std::endl;
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

  return 0;
}
