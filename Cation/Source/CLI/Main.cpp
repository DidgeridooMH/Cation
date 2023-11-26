#include <iostream>
#include <sstream>
#include <string>
#include "../Lexer/CLangLexer.hpp"
#include "../Lexer/CLangToken.hpp"

int main(int, char**) {
  std::wstringstream source(L"(}hi)");
  Cation::CLangLexer lexer;

  Cation::CLangToken token = lexer.GetToken(source);
  while (token.GetType() != Cation::CLangToken::NoToken) {
    std::wcout << "[" << token.GetLine() << "," << token.GetColumn() << "] "
      << token.GetType() << " - " << token.GetContent() << "\n";

    token = lexer.GetToken(source);
  }
  std::wcout << std::endl;

  return 0;
}