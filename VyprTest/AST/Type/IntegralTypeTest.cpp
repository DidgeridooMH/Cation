#include "Vypr/AST/Type/IntegralType.hpp"

#include <gtest/gtest.h>

namespace IntegralTypeTest
{
  TEST(Construct, NotConstNotLValue)
  {
    std::unique_ptr<Vypr::IntegralType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Bool, false, false,
                                             false);

    ASSERT_EQ(type->integral, Vypr::Integral::Bool);
    ASSERT_EQ(type->isConst, false);
    ASSERT_EQ(type->isLValue, false);
  }

  TEST(Construct, ConstNotLValue)
  {
    std::unique_ptr<Vypr::IntegralType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Bool, false, true,
                                             false);

    ASSERT_EQ(type->integral, Vypr::Integral::Bool);
    ASSERT_EQ(type->isConst, true);
    ASSERT_EQ(type->isLValue, false);
  }

  TEST(Construct, NotConstLValue)
  {
    std::unique_ptr<Vypr::IntegralType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Bool, false, false,
                                             true);

    ASSERT_EQ(type->integral, Vypr::Integral::Bool);
    ASSERT_EQ(type->isConst, false);
    ASSERT_EQ(type->isLValue, true);
  }

  TEST(Construct, ConstLValue)
  {
    std::unique_ptr<Vypr::IntegralType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Bool, false, false,
                                             true);

    ASSERT_EQ(type->integral, Vypr::Integral::Bool);
    ASSERT_EQ(type->isConst, false);
    ASSERT_EQ(type->isLValue, true);
  }

  TEST(Construct, OtherIntegralType)
  {
    std::unique_ptr<Vypr::IntegralType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);

    ASSERT_EQ(type->integral, Vypr::Integral::Long);
    ASSERT_EQ(type->isConst, false);
    ASSERT_EQ(type->isLValue, true);
  }

  TEST(Clone, NotConstNotLValue)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);

    std::unique_ptr<Vypr::StorageType> other = type->Clone();

    ASSERT_NE(type.get(), other.get());
    ASSERT_EQ(dynamic_cast<Vypr::IntegralType *>(type.get())->integral,
              dynamic_cast<Vypr::IntegralType *>(other.get())->integral);
    ASSERT_EQ(type->isConst, other->isConst);
    ASSERT_FALSE(other->isLValue);
  }

  TEST(CheckArithmetic, Void)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::StorageType>();

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::Add, other.get());

    ASSERT_EQ(result, nullptr);
  }

  TEST(CheckArithmetic, IntegralLowerPriority)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Byte, false, false,
                                             true);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::Add, other.get());
    Vypr::IntegralType *resultIntegral =
        dynamic_cast<Vypr::IntegralType *>(result.get());

    ASSERT_EQ(resultIntegral->integral, Vypr::Integral::Long);
    ASSERT_FALSE(resultIntegral->isConst);
    ASSERT_FALSE(resultIntegral->isLValue);
    ASSERT_FALSE(resultIntegral->isUnsigned);
  }

  TEST(CheckArithmetic, IntegralHigherPriority)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Byte, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::Add, other.get());
    Vypr::IntegralType *resultIntegral =
        dynamic_cast<Vypr::IntegralType *>(result.get());

    ASSERT_EQ(resultIntegral->integral, Vypr::Integral::Long);
    ASSERT_FALSE(resultIntegral->isConst);
    ASSERT_FALSE(resultIntegral->isLValue);
    ASSERT_FALSE(resultIntegral->isUnsigned);
  }

  TEST(CheckArithmetic, IntegralEqualPriority)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int, false, false,
                                             true);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::Add, other.get());
    Vypr::IntegralType *resultIntegral =
        dynamic_cast<Vypr::IntegralType *>(result.get());

    ASSERT_EQ(resultIntegral->integral, Vypr::Integral::Int);
    ASSERT_FALSE(resultIntegral->isConst);
    ASSERT_FALSE(resultIntegral->isLValue);
    ASSERT_FALSE(resultIntegral->isUnsigned);
  }

  TEST(CheckArithmetic, IntegralLowerPriorityUnsigned)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Byte, true, false,
                                             true);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::Add, other.get());
    Vypr::IntegralType *resultIntegral =
        dynamic_cast<Vypr::IntegralType *>(result.get());

    ASSERT_EQ(resultIntegral->integral, Vypr::Integral::Long);
    ASSERT_FALSE(resultIntegral->isConst);
    ASSERT_FALSE(resultIntegral->isLValue);
    ASSERT_TRUE(resultIntegral->isUnsigned);
  }

  TEST(CheckArithmetic, IntegralHigherPriorityUnsigned)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, true, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Byte, false, false,
                                             true);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::Add, other.get());
    Vypr::IntegralType *resultIntegral =
        dynamic_cast<Vypr::IntegralType *>(result.get());

    ASSERT_EQ(resultIntegral->integral, Vypr::Integral::Long);
    ASSERT_FALSE(resultIntegral->isConst);
    ASSERT_FALSE(resultIntegral->isLValue);
    ASSERT_TRUE(resultIntegral->isUnsigned);
  }

  TEST(CheckBitwise, Real)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::StorageType>(Vypr::StorageMetaType::Real, false,
                                            false);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::ShiftLeft, other.get());

    ASSERT_EQ(result, nullptr);
  }

  TEST(CheckBitwise, Pointer)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::StorageType>(Vypr::StorageMetaType::Pointer,
                                            false, false);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::ShiftLeft, other.get());

    ASSERT_EQ(result, nullptr);
  }

  TEST(CheckBitwise, Array)
  {
    std::unique_ptr<Vypr::StorageType> type =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Long, false, false,
                                             true);
    std::unique_ptr<Vypr::StorageType> other =
        std::make_unique<Vypr::StorageType>(Vypr::StorageMetaType::Array, false,
                                            false);

    std::unique_ptr<Vypr::StorageType> result =
        type->Check(Vypr::BinaryOp::ShiftLeft, other.get());

    ASSERT_EQ(result, nullptr);
  }
} // namespace IntegralTypeTest