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

    EXPECT_EQ(token.GetType(), Cation::CLangToken::NoToken);
  }

  TEST(GetToken, LeadingWS)
  {
    Cation::CLangLexer lexer;
    std::wstringstream testStream(L"   (");

    Cation::CLangToken token = lexer.GetToken(testStream);
    std::wstring remaining;
    testStream >> remaining;

    EXPECT_EQ(token.GetType(), Cation::CLangToken::LeftParenthesis);
    EXPECT_EQ(token.GetLine(), 1ULL);
    EXPECT_EQ(token.GetColumn(), 4ULL);
    EXPECT_EQ(token.GetContent(), L"(");
    EXPECT_TRUE(remaining.empty());
  }

#define TEST_PUNC_GET_TOKEN(name, testStr) \
    TEST(GetToken, name) { \
      Cation::CLangLexer lexer; \
      std::wstringstream testStream(testStr L"aaa"); \
      Cation::CLangToken token = lexer.GetToken(testStream); \
      std::wstring remaining; \
      testStream >> remaining; \
      EXPECT_EQ(token.GetType(), Cation::CLangToken:: ## name); \
      EXPECT_EQ(token.GetLine(), 1ULL); \
      EXPECT_EQ(token.GetColumn(), 1ULL); \
      EXPECT_EQ(token.GetContent(), std::wstring(testStr)); \
      EXPECT_EQ(remaining, L"aaa"); \
    }

  TEST_PUNC_GET_TOKEN(LeftBracket, L"[");
  TEST_PUNC_GET_TOKEN(RightBracket, L"]");
  TEST_PUNC_GET_TOKEN(LeftParenthesis, L"(");
  TEST_PUNC_GET_TOKEN(RightParenthesis, L")");
  TEST_PUNC_GET_TOKEN(LeftDragon, L"{");
  TEST_PUNC_GET_TOKEN(RightDragon, L"}");
  TEST_PUNC_GET_TOKEN(Period, L".");
  TEST_PUNC_GET_TOKEN(Increment, L"++");
  TEST_PUNC_GET_TOKEN(Decrement, L"--");
  TEST_PUNC_GET_TOKEN(Star, L"*");
  TEST_PUNC_GET_TOKEN(Add, L"+");
  TEST_PUNC_GET_TOKEN(Subtract, L"-");
  TEST_PUNC_GET_TOKEN(Tilde, L"~");
  TEST_PUNC_GET_TOKEN(Exclamation, L"!");
  TEST_PUNC_GET_TOKEN(Divide, L"/");
  TEST_PUNC_GET_TOKEN(Modulo, L"%");
  TEST_PUNC_GET_TOKEN(ShiftLeft, L"<<");
  TEST_PUNC_GET_TOKEN(ShiftRight, L">>");
  TEST_PUNC_GET_TOKEN(LessThan, L"<");
  TEST_PUNC_GET_TOKEN(GreaterThan, L">");
  TEST_PUNC_GET_TOKEN(LessEqual, L"<=");
  TEST_PUNC_GET_TOKEN(GreaterEqual, L">=");
  TEST_PUNC_GET_TOKEN(Equal, L"==");
  TEST_PUNC_GET_TOKEN(NotEqual, L"!=");
  TEST_PUNC_GET_TOKEN(And, L"&");
  TEST_PUNC_GET_TOKEN(Or, L"|");
  TEST_PUNC_GET_TOKEN(Xor, L"^");
  TEST_PUNC_GET_TOKEN(LogicalAnd, L"&&");
  TEST_PUNC_GET_TOKEN(LogicalOr, L"||");
  TEST_PUNC_GET_TOKEN(TernaryProposition, L"?");
  TEST_PUNC_GET_TOKEN(TernaryDecision, L":");
  TEST_PUNC_GET_TOKEN(StatementDelimiter, L";");
  TEST_PUNC_GET_TOKEN(Variadic, L"...");
  TEST_PUNC_GET_TOKEN(Assign, L"=");
  TEST_PUNC_GET_TOKEN(MultiplyAssign, L"*=");
  TEST_PUNC_GET_TOKEN(DivideAssign, L"/=");
  TEST_PUNC_GET_TOKEN(ModuloAssign, L"%=");
  TEST_PUNC_GET_TOKEN(AddAssign, L"+=");
  TEST_PUNC_GET_TOKEN(SubtractAssign, L"-=");
  TEST_PUNC_GET_TOKEN(LeftShiftAssign, L"<<=");
  TEST_PUNC_GET_TOKEN(RightShiftAssign, L">>=");
  TEST_PUNC_GET_TOKEN(AndAssign, L"&=");
  TEST_PUNC_GET_TOKEN(XorAssign, L"^=");
  TEST_PUNC_GET_TOKEN(OrAssign, L"|=");
  TEST_PUNC_GET_TOKEN(Comma, L",");
  TEST_PUNC_GET_TOKEN(Preprocessor, L"#");
  TEST_PUNC_GET_TOKEN(PreprocessorConcat, L"##");

  TEST(GetToken, PartialVariadic)
  {
    Cation::CLangLexer lexer;
    std::wstringstream testStream(L"..");

    Cation::CLangToken firstToken = lexer.GetToken(testStream);
    Cation::CLangToken secondToken = lexer.GetToken(testStream);
    std::wstring remaining;
    testStream >> remaining;

    EXPECT_EQ(firstToken.GetType(), Cation::CLangToken::Period);
    EXPECT_EQ(firstToken.GetLine(), 1ULL);
    EXPECT_EQ(firstToken.GetColumn(), 1ULL);
    EXPECT_EQ(firstToken.GetContent(), std::wstring(L"."));

    EXPECT_EQ(secondToken.GetType(), Cation::CLangToken::Period);
    EXPECT_EQ(secondToken.GetLine(), 1ULL);
    EXPECT_EQ(secondToken.GetColumn(), 2ULL);
    EXPECT_EQ(secondToken.GetContent(), std::wstring(L"."));

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

    EXPECT_EQ(firstToken.GetType(), Cation::CLangToken::LeftParenthesis);
    EXPECT_EQ(firstToken.GetLine(), 1ULL);
    EXPECT_EQ(firstToken.GetColumn(), 4ULL);
    EXPECT_EQ(firstToken.GetContent(), std::wstring(L"("));

    EXPECT_EQ(secondToken.GetType(), Cation::CLangToken::RightDragon);
    EXPECT_EQ(secondToken.GetLine(), 1ULL);
    EXPECT_EQ(secondToken.GetColumn(), 5ULL);
    EXPECT_EQ(secondToken.GetContent(), std::wstring(L"}"));

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

    EXPECT_EQ(firstToken.GetType(), Cation::CLangToken::LeftParenthesis);
    EXPECT_EQ(firstToken.GetLine(), 1ULL);
    EXPECT_EQ(firstToken.GetColumn(), 4ULL);
    EXPECT_EQ(firstToken.GetContent(), std::wstring(L"("));

    EXPECT_EQ(secondToken.GetType(), Cation::CLangToken::NoToken);
    EXPECT_EQ(secondToken.GetLine(), 0ULL);
    EXPECT_EQ(secondToken.GetColumn(), 0ULL);
    EXPECT_TRUE(secondToken.GetContent().empty());

    EXPECT_TRUE(remaining.empty());
  }

#define TEST_IDENT_GET_TOKEN(name, testStr) \
    TEST(GetTokenIdentifier, name) { \
      Cation::CLangLexer lexer; \
      std::wstringstream testStream(testStr); \
      Cation::CLangToken token = lexer.GetToken(testStream); \
      std::wstring remaining; \
      testStream >> remaining; \
      EXPECT_EQ(token.GetType(), Cation::CLangToken::Identifier); \
      EXPECT_EQ(token.GetLine(), 1ULL); \
      EXPECT_EQ(token.GetColumn(), 1ULL); \
      EXPECT_EQ(token.GetContent(), std::wstring(testStr)); \
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
    TEST(GetTokenIdentifier, name) { \
      Cation::CLangLexer lexer; \
      std::wstringstream testStream(testStr); \
      Cation::CLangToken token = lexer.GetToken(testStream); \
      std::wstring remaining; \
      testStream >> remaining; \
      EXPECT_EQ(token.GetType(), Cation::CLangToken::Identifier); \
      EXPECT_EQ(token.GetLine(), 1ULL); \
      EXPECT_EQ(token.GetColumn(), 1ULL); \
      EXPECT_EQ(token.GetContent(), std::wstring(result)); \
      EXPECT_TRUE(remaining.empty()); \
    }

  TEST_IDENT_GET_TOKEN_W_RESULT(Universal4, L"\\u2343abc", L"\u2343abc");
  TEST_IDENT_GET_TOKEN_W_RESULT(Universal8, L"\\U00029617abc", L"\U00029617abc");
  TEST_IDENT_GET_TOKEN_W_RESULT(Universal8NoHigh, L"\\U00009617abc", L"\U00009617abc");
  TEST_IDENT_GET_TOKEN_W_RESULT(Universal4Inside, L"hello\\u2343there", L"hello\u2343there");
  TEST_IDENT_GET_TOKEN_W_RESULT(Universal8Inside, L"hello\\U00029617there", L"hello\U00029617there");
}