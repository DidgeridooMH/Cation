set(LIBVYPR_SOURCE
  "Source/AST/UnknownTokenException.cpp"
  "Source/AST/Expression/ValueType.cpp"
  "Source/AST/Expression/ConstantNode.cpp"
  "Source/AST/Expression/ExpressionNode.cpp"
  "Source/AST/Expression/UnaryOpNode.cpp"
  "Source/AST/Expression/BinaryOpNode.cpp"
  "Source/AST/Expression/PostfixOpNode.cpp"
  "Source/Lexer/CLangKeywords.cpp"
  "Source/Lexer/CLangLexer.cpp"
  "Source/Lexer/CLangPunctuators.cpp"
  "Source/Scanner/Scanner.cpp"
  "Source/Scanner/StringScanner.cpp"
)

set(LIBVYPR_HEADER
  "Include/Vypr/AST/UnknownTokenException.hpp"
  "Include/Vypr/AST/Expression/ValueType.hpp"
  "Include/Vypr/AST/Expression/ConstantNode.hpp"
  "Include/Vypr/AST/Expression/ExpressionNode.hpp"
  "Include/Vypr/AST/Expression/UnaryOpNode.hpp"
  "Include/Vypr/AST/Expression/BinaryOpNode.hpp"
  "Include/Vypr/AST/Expression/PostfixOpNode.hpp"
  "Include/Vypr/Lexer/CLangLexer.hpp"
  "Include/Vypr/Lexer/CLangToken.hpp"
  "Include/Vypr/Lexer/CLangTokenMap.hpp"
  "Include/Vypr/Lexer/CLangTokenType.hpp"
  "Include/Vypr/Scanner/Scanner.hpp"
  "Include/Vypr/Scanner/StringScanner.hpp"
)

set(VYPR_SOURCE
  Source/CLI/Main.cpp
)

add_library(libvypr ${LIBVYPR_SOURCE} ${LIBVYPR_HEADER})
target_include_directories(libvypr PUBLIC Include/)

add_executable(vypr ${VYPR_SOURCE})
target_link_libraries(vypr PRIVATE libvypr)
