#include <iosfwd>
#include <string>
#include "CppUnitTest.h"
#include "Lexer/CLangLexer.hpp"
#include "Lexer/CLangToken.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CLangTest {
  TEST_CLASS(CLangLexerTest) {
public:
  TEST_METHOD(GetTokenEmpty) {
    Cation::CLangLexer lexer;
    std::wstringstream testStream(L"");

    Cation::CLangToken token = lexer.GetToken(testStream);

    Assert::IsTrue(token.GetType() == Cation::CLangToken::NoToken);
  }

  TEST_METHOD(GetTokenLeadingWS) {
    Cation::CLangLexer lexer;
    std::wstringstream testStream(L"   (");

    Cation::CLangToken token = lexer.GetToken(testStream);
    std::wstring remaining;
    testStream >> remaining;

    Assert::IsTrue(token.GetType() == Cation::CLangToken::LeftParenthesis);
    Assert::AreEqual(token.GetLine(), 1ULL);
    Assert::AreEqual(token.GetColumn(), 4ULL);
    Assert::AreEqual(token.GetContent(), std::wstring(L"("));
    Assert::IsTrue(remaining.empty());
  }

#define TEST_PUNC_GET_TOKEN(name, testStr) \
  TEST_METHOD(GetToken ## name) { \
    Cation::CLangLexer lexer; \
    std::wstringstream testStream(testStr L"aaa"); \
    Cation::CLangToken token = lexer.GetToken(testStream); \
    std::wstring remaining; \
    testStream >> remaining; \
    Assert::IsTrue(token.GetType() == Cation::CLangToken:: ## name); \
    Assert::AreEqual(token.GetLine(), 1ULL); \
    Assert::AreEqual(token.GetColumn(), 1ULL); \
    Assert::AreEqual(token.GetContent(), std::wstring(testStr)); \
    Assert::AreEqual(remaining, std::wstring(L"aaa")); \
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

  TEST_METHOD(GetTokenPartialVariadic) {
    Cation::CLangLexer lexer;
    std::wstringstream testStream(L"..");

    Cation::CLangToken firstToken = lexer.GetToken(testStream);
    Cation::CLangToken secondToken = lexer.GetToken(testStream);
    std::wstring remaining;
    testStream >> remaining;

    Assert::IsTrue(firstToken.GetType() == Cation::CLangToken::Period);
    Assert::AreEqual(firstToken.GetLine(), 1ULL);
    Assert::AreEqual(firstToken.GetColumn(), 1ULL);
    Assert::AreEqual(firstToken.GetContent(), std::wstring(L"."));

    Assert::IsTrue(secondToken.GetType() == Cation::CLangToken::Period);
    Assert::AreEqual(secondToken.GetLine(), 1ULL);
    Assert::AreEqual(secondToken.GetColumn(), 2ULL);
    Assert::AreEqual(secondToken.GetContent(), std::wstring(L"."));

    Assert::IsTrue(remaining.empty());
  }

  TEST_METHOD(GetTokenTwoToken) {
    Cation::CLangLexer lexer;
    std::wstringstream testStream(L"   (}");

    Cation::CLangToken firstToken = lexer.GetToken(testStream);
    Cation::CLangToken secondToken = lexer.GetToken(testStream);
    std::wstring remaining;
    testStream >> remaining;

    Assert::IsTrue(firstToken.GetType() == Cation::CLangToken::LeftParenthesis);
    Assert::AreEqual(firstToken.GetLine(), 1ULL);
    Assert::AreEqual(firstToken.GetColumn(), 4ULL);
    Assert::AreEqual(firstToken.GetContent(), std::wstring(L"("));

    Assert::IsTrue(secondToken.GetType() == Cation::CLangToken::RightDragon);
    Assert::AreEqual(secondToken.GetLine(), 1ULL);
    Assert::AreEqual(secondToken.GetColumn(), 5ULL);
    Assert::AreEqual(secondToken.GetContent(), std::wstring(L"}"));

    Assert::IsTrue(remaining.empty());
  }
  };
}