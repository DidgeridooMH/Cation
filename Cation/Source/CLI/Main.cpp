#include <iostream>
#include <sstream>
#include <string>
#include "../Lexer/CLangLexer.hpp"
#include "../Lexer/CLangToken.hpp"

int main(int, char**)
{
  std::wstringstream source(L"(}hi)");
  Cation::CLangLexer lexer;

  Cation::CLangToken token = lexer.GetToken(source);
  while (token.type != Cation::CLangTokenType::NoToken)
  {
    std::wcout << "[" << token.line << "," << token.column << "] "
      << (int)token.type << " - " << token.content << "\n";

    token = lexer.GetToken(source);
  }
  std::wcout << std::endl;

  return 0;
}