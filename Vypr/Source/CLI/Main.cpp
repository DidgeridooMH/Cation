#include <iostream>
#include <string>

#include "Vypr/Lexer/CLangLexer.hpp"
#include "Vypr/Lexer/CLangToken.hpp"
#include "Vypr/Scanner/StringScanner.hpp"

int main(int, char **)
{
  std::unique_ptr<Vypr::Scanner> source =
      std::make_unique<Vypr::StringScanner>(L"(}hi)");
  Vypr::CLangLexer lexer;

  Vypr::CLangToken token = lexer.GetToken(*source);
  while (token.type != Vypr::CLangTokenType::NoToken)
  {
    std::wcout << "[" << token.line << "," << token.column << "] "
               << (int)token.type << " - " << token.content << "\n";

    token = lexer.GetToken(*source);
  }
  std::wcout << std::endl;

  return 0;
}
