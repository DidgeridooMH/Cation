#include "Vypr/Lexer/CLangLexer.hpp"

#include <gtest/gtest.h>
#include <string>

#include "Vypr/Lexer/CLangToken.hpp"
#include "Vypr/Scanner/StringScanner.hpp"

namespace CLangLexerTest
{
  TEST(GetToken, Empty)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L""));

    Vypr::CLangToken token = lexer.GetToken();

    EXPECT_EQ(token.type, Vypr::CLangTokenType::NoToken);
  }

  TEST(GetToken, LeadingWS)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"   ("));

    Vypr::CLangToken token = lexer.GetToken();

    EXPECT_EQ(token.type, Vypr::CLangTokenType::LeftParenthesis);
    EXPECT_EQ(token.line, 1ULL);
    EXPECT_EQ(token.column, 4ULL);
    EXPECT_EQ(token.content, L"(");
  }

#define TEST_GET_TOKEN(testName, name, testStr)                                \
  TEST(GetToken, testName##name)                                               \
  {                                                                            \
    Vypr::CLangLexer lexer(                                                    \
        std::make_unique<Vypr::StringScanner>(testStr L" aaa"));               \
    Vypr::CLangToken token = lexer.GetToken();                                 \
    EXPECT_EQ(token.type, Vypr::CLangTokenType::name);                         \
    EXPECT_EQ(token.line, 1ULL);                                               \
    EXPECT_EQ(token.column, 1ULL);                                             \
    EXPECT_EQ(token.content, std::wstring(testStr));                           \
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
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L".."));

    Vypr::CLangToken firstToken = lexer.GetToken();
    Vypr::CLangToken secondToken = lexer.GetToken();

    EXPECT_EQ(firstToken.type, Vypr::CLangTokenType::Period);
    EXPECT_EQ(firstToken.line, 1ULL);
    EXPECT_EQ(firstToken.column, 1ULL);
    EXPECT_EQ(firstToken.content, std::wstring(L"."));

    EXPECT_EQ(secondToken.type, Vypr::CLangTokenType::Period);
    EXPECT_EQ(secondToken.line, 1ULL);
    EXPECT_EQ(secondToken.column, 2ULL);
    EXPECT_EQ(secondToken.content, std::wstring(L"."));
  }

  TEST(GetToken, TwoToken)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"   (}"));

    Vypr::CLangToken firstToken = lexer.GetToken();
    Vypr::CLangToken secondToken = lexer.GetToken();

    EXPECT_EQ(firstToken.type, Vypr::CLangTokenType::LeftParenthesis);
    EXPECT_EQ(firstToken.line, 1ULL);
    EXPECT_EQ(firstToken.column, 4ULL);
    EXPECT_EQ(firstToken.content, std::wstring(L"("));

    EXPECT_EQ(secondToken.type, Vypr::CLangTokenType::RightDragon);
    EXPECT_EQ(secondToken.line, 1ULL);
    EXPECT_EQ(secondToken.column, 5ULL);
    EXPECT_EQ(secondToken.content, std::wstring(L"}"));
  }

  TEST(GetToken, TwoTokenNoSecond)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"   ("));

    Vypr::CLangToken firstToken = lexer.GetToken();
    Vypr::CLangToken secondToken = lexer.GetToken();

    EXPECT_EQ(firstToken.type, Vypr::CLangTokenType::LeftParenthesis);
    EXPECT_EQ(firstToken.line, 1ULL);
    EXPECT_EQ(firstToken.column, 4ULL);
    EXPECT_EQ(firstToken.content, std::wstring(L"("));

    EXPECT_EQ(secondToken.type, Vypr::CLangTokenType::NoToken);
    EXPECT_EQ(secondToken.line, 0ULL);
    EXPECT_EQ(secondToken.column, 0ULL);
    EXPECT_TRUE(secondToken.content.empty());
  }

  TEST(PeekToken, TwoToken)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"hi 123"));

    Vypr::CLangToken firstToken = lexer.PeekToken();
    Vypr::CLangToken secondToken = lexer.PeekToken();

    EXPECT_EQ(firstToken.type, secondToken.type);
    EXPECT_EQ(firstToken.line, secondToken.line);
    EXPECT_EQ(firstToken.column, secondToken.column);
    EXPECT_EQ(firstToken.content, secondToken.content);
  }

  TEST(PeekToken, PeekGetToken)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"hi 123"));

    Vypr::CLangToken firstToken = lexer.PeekToken();
    Vypr::CLangToken secondToken = lexer.GetToken();

    EXPECT_EQ(firstToken.type, secondToken.type);
    EXPECT_EQ(firstToken.line, secondToken.line);
    EXPECT_EQ(firstToken.column, secondToken.column);
    EXPECT_EQ(firstToken.content, secondToken.content);
  }

#define TEST_IDENT_GET_TOKEN(name, testStr)                                    \
  TEST(GetToken, Identifier##name)                                             \
  {                                                                            \
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(testStr));    \
    Vypr::CLangToken token = lexer.GetToken();                                 \
    EXPECT_EQ(token.type, Vypr::CLangTokenType::Identifier);                   \
    EXPECT_EQ(token.line, 1ULL);                                               \
    EXPECT_EQ(token.column, 1ULL);                                             \
    EXPECT_EQ(token.content, std::wstring(testStr));                           \
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

#define TEST_IDENT_GET_TOKEN_W_RESULT(name, testStr, result)                   \
  TEST(GetToken, Identifier##name)                                             \
  {                                                                            \
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(testStr));    \
    Vypr::CLangToken token = lexer.GetToken();                                 \
    EXPECT_EQ(token.type, Vypr::CLangTokenType::Identifier);                   \
    EXPECT_EQ(token.line, 1ULL);                                               \
    EXPECT_EQ(token.column, 1ULL);                                             \
    EXPECT_EQ(token.content, std::wstring(result));                            \
  }

  TEST_IDENT_GET_TOKEN_W_RESULT(Universal4, L"\\u2343abc", L"\u2343abc");
  TEST_IDENT_GET_TOKEN_W_RESULT(Universal8, L"\\U00029617abc",
                                L"\U00029617abc");
  TEST_IDENT_GET_TOKEN_W_RESULT(Universal8NoHigh, L"\\U00009617abc",
                                L"\U00009617abc");
  TEST_IDENT_GET_TOKEN_W_RESULT(Universal4Inside, L"hello\\u2343there",
                                L"hello\u2343there");
  TEST_IDENT_GET_TOKEN_W_RESULT(Universal8Inside, L"hello\\U00029617there",
                                L"hello\U00029617there");

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

#define TEST_GET_TOKEN_CONSTANT(name, testStr, resultStr, tokenType)           \
  TEST(GetToken, NumericConstant##name)                                        \
  {                                                                            \
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(testStr));    \
                                                                               \
    Vypr::CLangToken token = lexer.GetToken();                                 \
                                                                               \
    EXPECT_EQ(token.type, Vypr::CLangTokenType::tokenType);                    \
    EXPECT_EQ(token.content, resultStr);                                       \
    EXPECT_EQ(token.line, 1);                                                  \
    EXPECT_EQ(token.column, 1);                                                \
  }

#define TEST_GET_TOKEN_CONSTANT_INTEGER(name, testStr, resultStr)              \
  TEST_GET_TOKEN_CONSTANT(name, testStr, resultStr, IntegerConstant)

#define TEST_GET_TOKEN_CONSTANT_FLOAT(name, testStr, resultStr)                \
  TEST_GET_TOKEN_CONSTANT(name, testStr, resultStr, FloatConstant)

#define TEST_GET_TOKEN_THROW(name, testStr)                                    \
  TEST(GetToken, NumericConstant##name)                                        \
  {                                                                            \
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(testStr));    \
                                                                               \
    EXPECT_THROW((void)lexer.GetToken(), Vypr::ParsingException);              \
  }

  TEST_GET_TOKEN_CONSTANT_INTEGER(BinaryZero, L"0b0", L"0b0");
  TEST_GET_TOKEN_CONSTANT_INTEGER(BinaryOne, L"0b1", L"0b1");
  TEST_GET_TOKEN_CONSTANT_INTEGER(BinaryCommon, L"0b101", L"0b101");
  TEST_GET_TOKEN_CONSTANT_INTEGER(BinaryBigLetter, L"0B1", L"0b1");
  TEST_GET_TOKEN_CONSTANT_INTEGER(BinarySuffixU, L"0B1U", L"0b1u");
  TEST_GET_TOKEN_CONSTANT_INTEGER(BinarySuffixu, L"0B1u", L"0b1u");
  TEST_GET_TOKEN_CONSTANT_INTEGER(BinarySuffixL, L"0B1L", L"0b1l");
  TEST_GET_TOKEN_CONSTANT_INTEGER(BinarySuffixl, L"0B1l", L"0b1l");
  TEST_GET_TOKEN_CONSTANT_INTEGER(BinarySuffixUL, L"0B1ul", L"0b1ul");
  TEST_GET_TOKEN_CONSTANT_INTEGER(BinarySuffixLU, L"0B1lu", L"0b1lu");
  TEST_GET_TOKEN_CONSTANT_INTEGER(BinarySuffixLL, L"0B1ll", L"0b1ll");
  TEST_GET_TOKEN_CONSTANT_INTEGER(BinarySuffixULL, L"0B1ull", L"0b1ull");
  TEST_GET_TOKEN_CONSTANT_INTEGER(BinarySuffixLLU, L"0B1llu", L"0b1llu");
  TEST_GET_TOKEN_THROW(BinaryBadNum, L"0b21");
  TEST_GET_TOKEN_THROW(BinaryBadSuffix, L"0b1A");
  TEST_GET_TOKEN_THROW(BinarySuffixUU, L"0b1UU");
  TEST_GET_TOKEN_THROW(BinarySuffixLUL, L"0b2LUL");

  TEST_GET_TOKEN_CONSTANT_INTEGER(HexZero, L"0x0", L"0x0");
  TEST_GET_TOKEN_CONSTANT_INTEGER(HexOne, L"0x1", L"0x1");
  TEST_GET_TOKEN_CONSTANT_INTEGER(HexCommon, L"0xDED", L"0xded");
  TEST_GET_TOKEN_CONSTANT_INTEGER(HexBigLetter, L"0X1", L"0x1");
  TEST_GET_TOKEN_CONSTANT_INTEGER(HexSuffixU, L"0x1U", L"0x1u");
  TEST_GET_TOKEN_CONSTANT_INTEGER(HexSuffixu, L"0x1u", L"0x1u");
  TEST_GET_TOKEN_CONSTANT_INTEGER(HexSuffixL, L"0x1L", L"0x1l");
  TEST_GET_TOKEN_CONSTANT_INTEGER(HexSuffixl, L"0x1l", L"0x1l");
  TEST_GET_TOKEN_CONSTANT_INTEGER(HexSuffixUL, L"0x1ul", L"0x1ul");
  TEST_GET_TOKEN_CONSTANT_INTEGER(HexSuffixLU, L"0x1lu", L"0x1lu");
  TEST_GET_TOKEN_CONSTANT_INTEGER(HexSuffixLL, L"0x1ll", L"0x1ll");
  TEST_GET_TOKEN_CONSTANT_INTEGER(HexSuffixULL, L"0x1ull", L"0x1ull");
  TEST_GET_TOKEN_CONSTANT_INTEGER(HexSuffixLLU, L"0x1llu", L"0x1llu");
  TEST_GET_TOKEN_CONSTANT_FLOAT(HexFloatingPoint, L"0xDED.ADp3", L"0xded.adp3");
  TEST_GET_TOKEN_CONSTANT_FLOAT(HexFloatingPointNoBase, L"0x.ADp3", L"0x.adp3");
  TEST_GET_TOKEN_CONSTANT_FLOAT(HexFloatingPointNoFraction, L"0x1.p3",
                                L"0x1.p3");
  TEST_GET_TOKEN_CONSTANT_FLOAT(HexFloatingPointLongExponent, L"0x.ADp323",
                                L"0x.adp323");
  TEST_GET_TOKEN_CONSTANT_FLOAT(HexFloatingPointUppercaseExponent, L"0x.ADP323",
                                L"0x.adp323");
  TEST_GET_TOKEN_CONSTANT_FLOAT(HexFloatingPointZeroExponent, L"0xded.ADp0",
                                L"0xded.adp0");
  TEST_GET_TOKEN_CONSTANT_FLOAT(HexFloatingPointPositiveExponent,
                                L"0xded.ADp+323", L"0xded.adp+323");
  TEST_GET_TOKEN_CONSTANT_FLOAT(HexFloatingPointNegativeExponent,
                                L"0xded.ADp-323", L"0xded.adp-323");
  TEST_GET_TOKEN_CONSTANT_FLOAT(HexFloatingPointSuffixF, L"0xDED.ADp3F",
                                L"0xded.adp3f");
  TEST_GET_TOKEN_CONSTANT_FLOAT(HexFloatingPointSuffixf, L"0xDED.ADp3f",
                                L"0xded.adp3f");
  TEST_GET_TOKEN_CONSTANT_FLOAT(HexFloatingPointSuffixL, L"0xDED.ADp3L",
                                L"0xded.adp3l");
  TEST_GET_TOKEN_CONSTANT_FLOAT(HexFloatingPointSuffixl, L"0xDED.ADp3l",
                                L"0xded.adp3l");
  TEST_GET_TOKEN_THROW(HexBadSuffix, L"0x21Z");
  TEST_GET_TOKEN_THROW(HexSuffixUU, L"0xaEUU");
  TEST_GET_TOKEN_THROW(HexSuffixLUL, L"0xdelul");
  TEST_GET_TOKEN_THROW(HexDecimalOnly, L"0x3.");
  TEST_GET_TOKEN_THROW(HexNoBaseNoFraction, L"0x.p3");
  TEST_GET_TOKEN_THROW(HexNoExponent, L"0x12.12p");

  TEST_GET_TOKEN_CONSTANT_INTEGER(DecimalZero, L"0", L"0");
  TEST_GET_TOKEN_CONSTANT_INTEGER(DecimalOne, L"1", L"1");
  TEST_GET_TOKEN_CONSTANT_INTEGER(DecimalCommon, L"145", L"145");
  TEST_GET_TOKEN_CONSTANT_INTEGER(DecimalSuffixU, L"1U", L"1u");
  TEST_GET_TOKEN_CONSTANT_INTEGER(DecimalSuffixu, L"1u", L"1u");
  TEST_GET_TOKEN_CONSTANT_INTEGER(DecimalSuffixL, L"1L", L"1l");
  TEST_GET_TOKEN_CONSTANT_INTEGER(DecimalSuffixl, L"1l", L"1l");
  TEST_GET_TOKEN_CONSTANT_INTEGER(DecimalSuffixUL, L"1ul", L"1ul");
  TEST_GET_TOKEN_CONSTANT_INTEGER(DecimalSuffixLU, L"1lu", L"1lu");
  TEST_GET_TOKEN_CONSTANT_INTEGER(DecimalSuffixLL, L"1ll", L"1ll");
  TEST_GET_TOKEN_CONSTANT_INTEGER(DecimalSuffixULL, L"1ull", L"1ull");
  TEST_GET_TOKEN_CONSTANT_INTEGER(DecimalSuffixLLU, L"1llu", L"1llu");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalFloatingPointNoFraction, L"12e3",
                                L"12e3");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalFloatingPointZeroExponent, L"12e0",
                                L"12e0");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalFloatingPointPositiveExponent, L"12e+23",
                                L"12e+23");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalFloatingPointNegativeExponent, L"12e-23",
                                L"12e-23");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalZeroExponent, L"0e3", L"0e3");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalZeroFloat, L"0.0", L"0.0");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalZeroFloatNoFraction, L"0.", L"0.");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalFloatingPoint, L"1434.23e3",
                                L"1434.23e3");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalFloatingPointNoBase, L".12", L".12");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalFloatingPointNoExponent, L"12.0",
                                L"12.0");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalFloatingPointLongExponent, L"12.0e123",
                                L"12.0e123");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalFloatingPointUppercaseExponent,
                                L"12.0E123", L"12.0e123");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalFloatingPointSuffixF, L"23.3F",
                                L"23.3f");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalFloatingPointSuffixf, L"23.3f",
                                L"23.3f");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalFloatingPointSuffixL, L"23.3L",
                                L"23.3l");
  TEST_GET_TOKEN_CONSTANT_FLOAT(DecimalFloatingPointSuffixl, L"23.3l",
                                L"23.3l");
  TEST_GET_TOKEN_THROW(DecimalBadSuffix, L"12.0Z");
  TEST_GET_TOKEN_THROW(DecimalSuffixUU, L"12UU");
  TEST_GET_TOKEN_THROW(DecimalSuffixLUL, L"12lul");

#define TEST_GET_TOKEN_CHAR_CONSTANT(name, testStr, resultStr)                 \
  TEST(GetToken, CharacterConstant##name)                                      \
  {                                                                            \
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(testStr));    \
                                                                               \
    Vypr::CLangToken token = lexer.GetToken();                                 \
                                                                               \
    EXPECT_EQ(token.type, Vypr::CLangTokenType::CharacterConstant);            \
    EXPECT_EQ(token.content, resultStr);                                       \
    EXPECT_EQ(token.line, 1);                                                  \
    EXPECT_EQ(token.column, 1);                                                \
  }

  TEST_GET_TOKEN_CHAR_CONSTANT(RegularCharacter, L"'a'", L"a");
  TEST_GET_TOKEN_CHAR_CONSTANT(AlertCharacter, L"'\\a'", L"\a");
  TEST_GET_TOKEN_CHAR_CONSTANT(BackspaceCharacter, L"'\\b'", L"\b");
  TEST_GET_TOKEN_CHAR_CONSTANT(EscapeCharacter, L"'\\e'", L"\e");
  TEST_GET_TOKEN_CHAR_CONSTANT(FormfeedCharacter, L"'\\f'", L"\f");
  TEST_GET_TOKEN_CHAR_CONSTANT(NewlineCharacter, L"'\\n'", L"\n");
  TEST_GET_TOKEN_CHAR_CONSTANT(CarriageReturnCharacter, L"'\\r'", L"\r");
  TEST_GET_TOKEN_CHAR_CONSTANT(TabCharacter, L"'\\t'", L"\t");
  TEST_GET_TOKEN_CHAR_CONSTANT(VerticalTabCharacter, L"'\\v'", L"\v");
  TEST_GET_TOKEN_CHAR_CONSTANT(BackslashCharacter, L"'\\\\'", L"\\");
  TEST_GET_TOKEN_CHAR_CONSTANT(SingleQuoteCharacter, L"'\\''", L"\'");
  TEST_GET_TOKEN_CHAR_CONSTANT(DoubleQuoteCharacter, L"'\\\"'", L"\"");
  TEST_GET_TOKEN_CHAR_CONSTANT(QuestionMarkCharacter, L"'\\?'", L"\?");
  TEST_GET_TOKEN_CHAR_CONSTANT(OctalMarkCharacter, L"'\101'", L"A");
  TEST_GET_TOKEN_CHAR_CONSTANT(HexMarkCharacter, L"'\x65'", L"e");
  TEST_GET_TOKEN_CHAR_CONSTANT(Unicode16MarkCharacter, L"'\u0065'", L"e");
  TEST_GET_TOKEN_CHAR_CONSTANT(Unicode32MarkCharacter, L"'\U00000065'", L"e");
  TEST_GET_TOKEN_THROW(MissingTerminator, L"'a");
  TEST_GET_TOKEN_THROW(MissingContent, L"''");
  TEST_GET_TOKEN_THROW(TooMuchContent, L"'aa'");

#define TEST_GET_TOKEN_STRING_CONSTANT(name, testStr, resultStr)               \
  TEST(GetToken, StringLiteral##name)                                          \
  {                                                                            \
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(testStr));    \
                                                                               \
    Vypr::CLangToken token = lexer.GetToken();                                 \
                                                                               \
    EXPECT_EQ(token.type, Vypr::CLangTokenType::StringLiteral);                \
    EXPECT_EQ(token.content, resultStr);                                       \
    EXPECT_EQ(token.line, 1);                                                  \
    EXPECT_EQ(token.column, 1);                                                \
  }

  TEST_GET_TOKEN_STRING_CONSTANT(CommonString, L"\"helloworld\"",
                                 L"helloworld");
  TEST_GET_TOKEN_STRING_CONSTANT(NewLineString, L"\"hello\\nworld\"",
                                 L"hello\nworld");
  TEST_GET_TOKEN_STRING_CONSTANT(EmptyString, L"\"\"", L"");
  TEST_GET_TOKEN_THROW(MissingTerminatorStringLiteral, L"\"a");
  TEST_GET_TOKEN_THROW(NewlineInStringLiteral, L"\"a\n\"");

  TEST(GetToken, SingleLineComment)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"// Hello\nint"));

    Vypr::CLangToken token = lexer.GetToken();

    EXPECT_EQ(token.type, Vypr::CLangTokenType::IntegerType);
    EXPECT_EQ(token.column, 1);
    EXPECT_EQ(token.line, 2);
    EXPECT_EQ(token.content, L"int");
  }

  TEST(GetToken, MultiLineOneLineComment)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"/* Hello */int"));

    Vypr::CLangToken token = lexer.GetToken();

    EXPECT_EQ(token.type, Vypr::CLangTokenType::IntegerType);
    EXPECT_EQ(token.column, 12);
    EXPECT_EQ(token.line, 1);
    EXPECT_EQ(token.content, L"int");
  }

  TEST(GetToken, MultiLineSplitLineComment)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"/* Hello */\nint"));

    Vypr::CLangToken token = lexer.GetToken();

    EXPECT_EQ(token.type, Vypr::CLangTokenType::IntegerType);
    EXPECT_EQ(token.column, 1);
    EXPECT_EQ(token.line, 2);
    EXPECT_EQ(token.content, L"int");
  }

  TEST(GetToken, MultiLineManyLineComment)
  {
    Vypr::CLangLexer lexer(
        std::make_unique<Vypr::StringScanner>(L"/* \nHello \n*/\nint"));

    Vypr::CLangToken token = lexer.GetToken();

    EXPECT_EQ(token.type, Vypr::CLangTokenType::IntegerType);
    EXPECT_EQ(token.column, 1);
    EXPECT_EQ(token.line, 4);
    EXPECT_EQ(token.content, L"int");
  }

  TEST(GetToken, ManyTokens)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(
        L"hi 33.2f \nint i = 0;/* \nHello \n*/\nint"));

    std::vector<std::tuple<Vypr::CLangTokenType, size_t, size_t>> tokens;
    Vypr::CLangToken token = lexer.GetToken();
    while (token.type != Vypr::CLangTokenType::NoToken)
    {
      tokens.push_back({token.type, token.column, token.line});
      token = lexer.GetToken();
    }

    std::vector<std::tuple<Vypr::CLangTokenType, size_t, size_t>> groundTruth =
        {{Vypr::CLangTokenType::Identifier, 1, 1},
         {Vypr::CLangTokenType::FloatConstant, 4, 1},
         {Vypr::CLangTokenType::IntegerType, 1, 2},
         {Vypr::CLangTokenType::Identifier, 5, 2},
         {Vypr::CLangTokenType::Assign, 7, 2},
         {Vypr::CLangTokenType::IntegerConstant, 9, 2},
         {Vypr::CLangTokenType::StatementDelimiter, 10, 2},
         {Vypr::CLangTokenType::IntegerType, 1, 5}};

    ASSERT_EQ(tokens.size(), groundTruth.size());
    for (int i = 0; i < tokens.size(); i++)
    {
      auto &[type, col, line] = tokens[i];
      auto &[ttype, tcol, tline] = groundTruth[i];
      ASSERT_EQ(type, ttype);
    }
  }
} // namespace CLangLexerTest