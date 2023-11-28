#include <gtest/gtest.h>
#include <iosfwd>
#include <string>
#include "Lexer/CLangLexer.hpp"
#include "Lexer/CLangToken.hpp"

namespace CLangLexerTest
{
  TEST(GetToken, Empty)
  {
    Cation::CLangLexer lexer;
    std::wstringstream testStream(L"");

    Cation::CLangToken token = lexer.GetToken(testStream);

    EXPECT_EQ(token.type, Cation::CLangTokenType::NoToken);
  }

  TEST(GetToken, LeadingWS)
  {
    Cation::CLangLexer lexer;
    std::wstringstream testStream(L"   (");

    Cation::CLangToken token = lexer.GetToken(testStream);
    std::wstring remaining;
    testStream >> remaining;

    EXPECT_EQ(token.type, Cation::CLangTokenType::LeftParenthesis);
    EXPECT_EQ(token.line, 1ULL);
    EXPECT_EQ(token.column, 4ULL);
    EXPECT_EQ(token.content, L"(");
    EXPECT_TRUE(remaining.empty());
  }

#define TEST_GET_TOKEN(testName, name, testStr) \
    TEST(GetToken, testName ## name) { \
      Cation::CLangLexer lexer; \
      std::wstringstream testStream(testStr L" aaa"); \
      Cation::CLangToken token = lexer.GetToken(testStream); \
      std::wstring remaining; \
      testStream >> remaining; \
      EXPECT_EQ(token.type, Cation::CLangTokenType:: ## name); \
      EXPECT_EQ(token.line, 1ULL); \
      EXPECT_EQ(token.column, 1ULL); \
      EXPECT_EQ(token.content, std::wstring(testStr)); \
      EXPECT_EQ(remaining, L"aaa"); \
    }

  TEST_GET_TOKEN(Operator, LeftBracket, L"[");
  TEST_GET_TOKEN(Operator, RightBracket, L"]");
  TEST_GET_TOKEN(Operator, LeftParenthesis, L"(");
  TEST_GET_TOKEN(Operator, RightParenthesis, L")");
  TEST_GET_TOKEN(Operator, LeftDragon, L"{");
  TEST_GET_TOKEN(Operator, RightDragon, L"}");
  TEST_GET_TOKEN(Operator, Period, L".");
  TEST_GET_TOKEN(Operator, Increment, L"++");
  TEST_GET_TOKEN(Operator, Decrement, L"--");
  TEST_GET_TOKEN(Operator, Star, L"*");
  TEST_GET_TOKEN(Operator, Add, L"+");
  TEST_GET_TOKEN(Operator, Subtract, L"-");
  TEST_GET_TOKEN(Operator, Tilde, L"~");
  TEST_GET_TOKEN(Operator, Exclamation, L"!");
  TEST_GET_TOKEN(Operator, Divide, L"/");
  TEST_GET_TOKEN(Operator, Modulo, L"%");
  TEST_GET_TOKEN(Operator, ShiftLeft, L"<<");
  TEST_GET_TOKEN(Operator, ShiftRight, L">>");
  TEST_GET_TOKEN(Operator, LessThan, L"<");
  TEST_GET_TOKEN(Operator, GreaterThan, L">");
  TEST_GET_TOKEN(Operator, LessEqual, L"<=");
  TEST_GET_TOKEN(Operator, GreaterEqual, L">=");
  TEST_GET_TOKEN(Operator, Equal, L"==");
  TEST_GET_TOKEN(Operator, NotEqual, L"!=");
  TEST_GET_TOKEN(Operator, And, L"&");
  TEST_GET_TOKEN(Operator, Or, L"|");
  TEST_GET_TOKEN(Operator, Xor, L"^");
  TEST_GET_TOKEN(Operator, LogicalAnd, L"&&");
  TEST_GET_TOKEN(Operator, LogicalOr, L"||");
  TEST_GET_TOKEN(Operator, TernaryProposition, L"?");
  TEST_GET_TOKEN(Operator, TernaryDecision, L":");
  TEST_GET_TOKEN(Operator, StatementDelimiter, L";");
  TEST_GET_TOKEN(Operator, Variadic, L"...");
  TEST_GET_TOKEN(Operator, Assign, L"=");
  TEST_GET_TOKEN(Operator, MultiplyAssign, L"*=");
  TEST_GET_TOKEN(Operator, DivideAssign, L"/=");
  TEST_GET_TOKEN(Operator, ModuloAssign, L"%=");
  TEST_GET_TOKEN(Operator, AddAssign, L"+=");
  TEST_GET_TOKEN(Operator, SubtractAssign, L"-=");
  TEST_GET_TOKEN(Operator, LeftShiftAssign, L"<<=");
  TEST_GET_TOKEN(Operator, RightShiftAssign, L">>=");
  TEST_GET_TOKEN(Operator, AndAssign, L"&=");
  TEST_GET_TOKEN(Operator, XorAssign, L"^=");
  TEST_GET_TOKEN(Operator, OrAssign, L"|=");
  TEST_GET_TOKEN(Operator, Comma, L",");
  TEST_GET_TOKEN(Operator, Preprocessor, L"#");
  TEST_GET_TOKEN(Operator, PreprocessorConcat, L"##");

  TEST(GetToken, PartialVariadic)
  {
    Cation::CLangLexer lexer;
    std::wstringstream testStream(L"..");

    Cation::CLangToken firstToken = lexer.GetToken(testStream);
    Cation::CLangToken secondToken = lexer.GetToken(testStream);
    std::wstring remaining;
    testStream >> remaining;

    EXPECT_EQ(firstToken.type, Cation::CLangTokenType::Period);
    EXPECT_EQ(firstToken.line, 1ULL);
    EXPECT_EQ(firstToken.column, 1ULL);
    EXPECT_EQ(firstToken.content, std::wstring(L"."));

    EXPECT_EQ(secondToken.type, Cation::CLangTokenType::Period);
    EXPECT_EQ(secondToken.line, 1ULL);
    EXPECT_EQ(secondToken.column, 2ULL);
    EXPECT_EQ(secondToken.content, std::wstring(L"."));

    EXPECT_TRUE(remaining.empty());
  }

  TEST(GetToken, TwoToken)
  {
    Cation::CLangLexer lexer;
    std::wstringstream testStream(L"   (}");

    Cation::CLangToken firstToken = lexer.GetToken(testStream);
    Cation::CLangToken secondToken = lexer.GetToken(testStream);
    std::wstring remaining;
    testStream >> remaining;

    EXPECT_EQ(firstToken.type, Cation::CLangTokenType::LeftParenthesis);
    EXPECT_EQ(firstToken.line, 1ULL);
    EXPECT_EQ(firstToken.column, 4ULL);
    EXPECT_EQ(firstToken.content, std::wstring(L"("));

    EXPECT_EQ(secondToken.type, Cation::CLangTokenType::RightDragon);
    EXPECT_EQ(secondToken.line, 1ULL);
    EXPECT_EQ(secondToken.column, 5ULL);
    EXPECT_EQ(secondToken.content, std::wstring(L"}"));

    EXPECT_TRUE(remaining.empty());
  }

  TEST(GetToken, TwoTokenNoSecond)
  {
    Cation::CLangLexer lexer;
    std::wstringstream testStream(L"   (");

    Cation::CLangToken firstToken = lexer.GetToken(testStream);
    Cation::CLangToken secondToken = lexer.GetToken(testStream);
    std::wstring remaining;
    testStream >> remaining;

    EXPECT_EQ(firstToken.type, Cation::CLangTokenType::LeftParenthesis);
    EXPECT_EQ(firstToken.line, 1ULL);
    EXPECT_EQ(firstToken.column, 4ULL);
    EXPECT_EQ(firstToken.content, std::wstring(L"("));

    EXPECT_EQ(secondToken.type, Cation::CLangTokenType::NoToken);
    EXPECT_EQ(secondToken.line, 0ULL);
    EXPECT_EQ(secondToken.column, 0ULL);
    EXPECT_TRUE(secondToken.content.empty());

    EXPECT_TRUE(remaining.empty());
  }

#define TEST_IDENT_GET_TOKEN(name, testStr) \
    TEST(GetToken, Identifier ## name) { \
      Cation::CLangLexer lexer; \
      std::wstringstream testStream(testStr); \
      Cation::CLangToken token = lexer.GetToken(testStream); \
      std::wstring remaining; \
      testStream >> remaining; \
      EXPECT_EQ(token.type, Cation::CLangTokenType::Identifier); \
      EXPECT_EQ(token.line, 1ULL); \
      EXPECT_EQ(token.column, 1ULL); \
      EXPECT_EQ(token.content, std::wstring(testStr)); \
      EXPECT_TRUE(remaining.empty());  \
    }

  TEST_IDENT_GET_TOKEN(OnlyLowercase, L"alexander");
  TEST_IDENT_GET_TOKEN(OnlyUppercase, L"ALEXANDER");
  TEST_IDENT_GET_TOKEN(CamelCase, L"gramBell");
  TEST_IDENT_GET_TOKEN(SnakeCase, L"gram_bell");
  TEST_IDENT_GET_TOKEN(PascalCase, L"VisualStudio");
  TEST_IDENT_GET_TOKEN(LeadingUnderscore, L"_unusedVariable");
  TEST_IDENT_GET_TOKEN(WithDigits, L"commander1234");
  TEST_IDENT_GET_TOKEN(SingleLetter, L"i");
  TEST_IDENT_GET_TOKEN(Unicode, L"◕‿◕");

#define TEST_IDENT_GET_TOKEN_W_RESULT(name, testStr, result) \
    TEST(GetToken, Identifier ## name) { \
      Cation::CLangLexer lexer; \
      std::wstringstream testStream(testStr); \
      Cation::CLangToken token = lexer.GetToken(testStream); \
      std::wstring remaining; \
      testStream >> remaining; \
      EXPECT_EQ(token.type, Cation::CLangTokenType::Identifier); \
      EXPECT_EQ(token.line, 1ULL); \
      EXPECT_EQ(token.column, 1ULL); \
      EXPECT_EQ(token.content, std::wstring(result)); \
      EXPECT_TRUE(remaining.empty()); \
    }

  TEST_IDENT_GET_TOKEN_W_RESULT(Universal4, L"\\u2343abc", L"\u2343abc");
  TEST_IDENT_GET_TOKEN_W_RESULT(Universal8, L"\\U00029617abc", L"\U00029617abc");
  TEST_IDENT_GET_TOKEN_W_RESULT(Universal8NoHigh, L"\\U00009617abc", L"\U00009617abc");
  TEST_IDENT_GET_TOKEN_W_RESULT(Universal4Inside, L"hello\\u2343there", L"hello\u2343there");
  TEST_IDENT_GET_TOKEN_W_RESULT(Universal8Inside, L"hello\\U00029617there", L"hello\U00029617there");

  TEST_GET_TOKEN(Keyword, Auto, L"auto");
  TEST_GET_TOKEN(Keyword, Break, L"break");
  TEST_GET_TOKEN(Keyword, Case, L"case");
  TEST_GET_TOKEN(Keyword, CharType, L"char");
  TEST_GET_TOKEN(Keyword, DoubleType, L"double");
  TEST_GET_TOKEN(Keyword, IntegerType, L"int");
  TEST_GET_TOKEN(Keyword, FloatType, L"float");
  TEST_GET_TOKEN(Keyword, LongType, L"long");
  TEST_GET_TOKEN(Keyword, ShortType, L"short");
  TEST_GET_TOKEN(Keyword, Const, L"const");
  TEST_GET_TOKEN(Keyword, Continue, L"continue");
  TEST_GET_TOKEN(Keyword, Default, L"default");
  TEST_GET_TOKEN(Keyword, Do, L"do");
  TEST_GET_TOKEN(Keyword, Else, L"else");
  TEST_GET_TOKEN(Keyword, Enumeration, L"enum");
  TEST_GET_TOKEN(Keyword, Extern, L"extern");
  TEST_GET_TOKEN(Keyword, For, L"for");
  TEST_GET_TOKEN(Keyword, Goto, L"goto");
  TEST_GET_TOKEN(Keyword, If, L"if");
  TEST_GET_TOKEN(Keyword, Inline, L"inline");
  TEST_GET_TOKEN(Keyword, Register, L"register");
  TEST_GET_TOKEN(Keyword, Restrict, L"restrict");
  TEST_GET_TOKEN(Keyword, Return, L"return");
  TEST_GET_TOKEN(Keyword, Signed, L"signed");
  TEST_GET_TOKEN(Keyword, Sizeof, L"sizeof");
  TEST_GET_TOKEN(Keyword, Static, L"static");
  TEST_GET_TOKEN(Keyword, Struct, L"struct");
  TEST_GET_TOKEN(Keyword, Switch, L"switch");
  TEST_GET_TOKEN(Keyword, Typedef, L"typedef");
  TEST_GET_TOKEN(Keyword, Union, L"union");
  TEST_GET_TOKEN(Keyword, Unsigned, L"unsigned");
  TEST_GET_TOKEN(Keyword, Void, L"void");
  TEST_GET_TOKEN(Keyword, Volatile, L"volatile");
  TEST_GET_TOKEN(Keyword, While, L"while");
  TEST_GET_TOKEN(Keyword, AlignAs, L"_Alignas");
  TEST_GET_TOKEN(Keyword, AlignOf, L"_Alignof");
  TEST_GET_TOKEN(Keyword, Atomic, L"_Atomic");
  TEST_GET_TOKEN(Keyword, Boolean, L"_Bool");
  TEST_GET_TOKEN(Keyword, Complex, L"_Complex");
  TEST_GET_TOKEN(Keyword, Generic, L"_Generic");
  TEST_GET_TOKEN(Keyword, Imaginary, L"_Imaginary");
  TEST_GET_TOKEN(Keyword, NoReturn, L"_Noreturn");
  TEST_GET_TOKEN(Keyword, StaticAssert, L"_Static_assert");
  TEST_GET_TOKEN(Keyword, ThreadLocal, L"_Thread_local");
}