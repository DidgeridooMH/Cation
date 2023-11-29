#include <iostream>
#include <sstream>
#include <string>

#include "Vypr/Lexer/CLangLexer.hpp"
#include "Vypr/Lexer/CLangToken.hpp"

int main(int, char**)
{
  std::wstringstream source(L"(}hi)");
  Vypr::CLangLexer lexer;

  Vypr::CLangToken token = lexer.GetToken(source);
  while (token.type != Vypr::CLangTokenType::NoToken)
  {
    std::wcout << "[" << token.line << "," << token.column << "] "
      << (int)token.type << " - " << token.content << "\n";

    token = lexer.GetToken(source);
  }
  std::wcout << std::endl;

  return 0;
}
