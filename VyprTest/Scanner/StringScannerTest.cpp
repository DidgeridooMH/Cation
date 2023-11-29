#include "Vypr/Scanner/StringScanner.hpp"

#include <gtest/gtest.h>

namespace StringScannerTest
{
  TEST(Next, Empty)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>();

    wchar_t result = scanner->Next();

    EXPECT_EQ(result, std::char_traits<wchar_t>::eof());
    EXPECT_EQ(scanner->GetLine(), 1);
    EXPECT_EQ(scanner->GetColumn(), 1);
  }

  TEST(Next, OneCharacter)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>(L"a");

    wchar_t result = scanner->Next();

    EXPECT_EQ(result, L'a');
    EXPECT_EQ(scanner->GetLine(), 1);
    EXPECT_EQ(scanner->GetColumn(), 2);
  }

  TEST(Next, MultipleCharacters)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>(L"abcde");

    wchar_t result1 = scanner->Next();
    wchar_t result2 = scanner->Next();
    wchar_t result3 = scanner->Next();

    EXPECT_EQ(result1, L'a');
    EXPECT_EQ(result2, L'b');
    EXPECT_EQ(result3, L'c');
    EXPECT_EQ(scanner->GetLine(), 1);
    EXPECT_EQ(scanner->GetColumn(), 4);
  }

  TEST(Next, ToEnd)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>(L"a");

    wchar_t result = scanner->Next();
    wchar_t eofResult = scanner->Next();

    EXPECT_EQ(result, L'a');
    EXPECT_EQ(eofResult, std::char_traits<wchar_t>::eof());
    EXPECT_EQ(scanner->GetLine(), 1);
    EXPECT_EQ(scanner->GetColumn(), 2);
  }

  TEST(Next, NextLine)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>(L"a\na");

    wchar_t result1 = scanner->Next();
    wchar_t result2 = scanner->Next();

    EXPECT_EQ(result1, L'a');
    EXPECT_EQ(result2, L'\n');
    EXPECT_EQ(scanner->GetLine(), 2);
    EXPECT_EQ(scanner->GetColumn(), 1);
  }

  TEST(LookAhead, Empty)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>();

    wchar_t result = scanner->LookAhead(0);

    EXPECT_EQ(result, std::char_traits<wchar_t>::eof());
    EXPECT_EQ(scanner->GetLine(), 1);
    EXPECT_EQ(scanner->GetColumn(), 1);
  }

  TEST(LookAhead, OneCharacter)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>(L"a");

    wchar_t result = scanner->LookAhead(0);

    EXPECT_EQ(result, L'a');
    EXPECT_EQ(scanner->GetLine(), 1);
    EXPECT_EQ(scanner->GetColumn(), 1);
  }

  TEST(LookAhead, MultipleCharacters)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>(L"abcde");

    wchar_t result = scanner->LookAhead(2);

    EXPECT_EQ(result, L'c');
    EXPECT_EQ(scanner->GetLine(), 1);
    EXPECT_EQ(scanner->GetColumn(), 1);
  }

  TEST(LookAhead, ToEnd)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>(L"a");

    wchar_t result = scanner->LookAhead(5);

    EXPECT_EQ(result, std::char_traits<wchar_t>::eof());
    EXPECT_EQ(scanner->GetLine(), 1);
    EXPECT_EQ(scanner->GetColumn(), 1);
  }

  TEST(Finished, Empty)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>();
    EXPECT_TRUE(scanner->Finished());
  }

  TEST(Finished, ManyCharactersFalse)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>(L"abc");
    EXPECT_TRUE(!scanner->Finished());
  }

  TEST(Finished, ManyCharactersTrue)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>(L"abc");
    scanner->Next();
    scanner->Next();
    scanner->Next();
    EXPECT_TRUE(scanner->Finished());
  }

  TEST(NextWhile, Empty)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>();

    std::wstring result = scanner->NextWhile([](wchar_t c) { return true; });

    EXPECT_TRUE(result.empty());
    EXPECT_EQ(scanner->GetLine(), 1);
    EXPECT_EQ(scanner->GetColumn(), 1);
  }

  TEST(NextWhile, MultipleCharacters)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>(L"aaab");

    std::wstring result =
        scanner->NextWhile([](wchar_t c) { return c == 'a'; });

    EXPECT_EQ(result, L"aaa");
    EXPECT_EQ(scanner->GetLine(), 1);
    EXPECT_EQ(scanner->GetColumn(), 4);
  }

  TEST(NextWhile, ToEnd)
  {
    std::unique_ptr<Vypr::Scanner> scanner =
        std::make_unique<Vypr::StringScanner>(L"aa");

    std::wstring result =
        scanner->NextWhile([](wchar_t c) { return c == 'a'; });

    EXPECT_EQ(result, L"aa");
    EXPECT_EQ(scanner->GetLine(), 1);
    EXPECT_EQ(scanner->GetColumn(), 3);
  }
} // namespace StringScannerTest