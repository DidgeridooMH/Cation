#include <sstream>
#include <string>
#include "../Lexer/CLangLexer.hpp"

int main(int, char**) {
  std::wstringstream source(L"(hi)");
  Cation::CLangLexer lexer;
  lexer.GetToken(source);
  return 0;
}