set(LIBVYPR_SOURCE
  Source/Lexer/CLangKeywords.cpp
  Source/Lexer/CLangLexer.cpp
  Source/Lexer/CLangPunctuators.cpp
)

set(LIBVYPR_HEADER
  Include/Vypr/Lexer/CLangLexer.hpp
  Include/Vypr/Lexer/CLangToken.hpp
  Include/Vypr/Lexer/CLangTokenMap.hpp
  Include/Vypr/Lexer/CLangTokenType.hpp
)

set(VYPR_SOURCE
  Source/CLI/Main.cpp
)

add_library(libvypr ${LIBVYPR_SOURCE} ${LIBVYPR_HEADER})
target_include_directories(libvypr PUBLIC Include/)

add_executable(vypr ${VYPR_SOURCE})
target_link_libraries(vypr PRIVATE libvypr)
