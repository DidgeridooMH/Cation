#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

#include "Vypr/AST/Expression/ExpressionNode.hpp"
#include "Vypr/Lexer/CLangLexer.hpp"
#include "Vypr/Scanner/StringScanner.hpp"

int main(int, char **)
{
  Vypr::CLangLexer lexer(
      std::make_unique<Vypr::StringScanner>(L"3 && 1 + 23 * 2 | 2 % 2"));

  auto expression = Vypr::ExpressionNode::Parse(lexer);
  std::wstring result = expression->PrettyPrint(0);

  std::wcout << result << std::endl;

  return 0;
}
