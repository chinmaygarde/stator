// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include "flutter/testing/testing.h"
#include "impeller/stator/epoxy/driver.h"
#include "impeller/stator/epoxy/sema.h"

namespace epoxy {
namespace testing {

TEST(SemaTest, Empty) {
  Driver driver;
  auto driver_result = driver.Parse(R"~()~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kSuccess);
}

TEST(SemaTest, DuplicateFunctionsCauseSemaError) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      function foo() -> void
      function foo(int32_t a, int32_t b) -> void*
    }
  )~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kError);
}

TEST(SemaTest, NonPointerVoidMemberIsError) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      struct Foo {
        void a;
      }
    }
  )~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kError);
}

TEST(SemaTest, EnumCannotHaveDuplicateMembers) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      enum Foo {
        SomeMember,
        SomeMember,
      }
    }
  )~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kError);
}

TEST(SemaTest, StructsAndEnumNamesCannotCollide) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      enum Foo {

      }
      struct Foo {

      }
    }
  )~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kError);
}

TEST(SemaTest, EnumNamesInFunctionArgsMustBeKnown) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      function Foo(AbsentEnum val) -> void
    }
  )~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kError);
}

TEST(SemaTest, EnumNamesInStructsMustBeKnown) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      struct Foo {
        AbsentEnum val;
      }
    }
  )~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kError);
}

TEST(SemaTest, StructPointersInStructsMustBeKnown) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      struct Foo {
        AbsentStrct* val;
      }
    }
  )~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kError);
}

TEST(SemaTest, StructNamesInFunctionArgsMustBeKnown) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      function Foo(AbsentStruct* val) -> void
    }
  )~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kError);
}

TEST(SemaTest, CannotReturnPointerToUnknownStruct) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      enum AbsentStruct {

      }
      function Foo() -> AbsentStruct*
    }
  )~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kError);
}

TEST(SemaTest, CannotReturnUnknownEnum) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      struct AbsentEnum {

      }
      function Foo() -> AbsentEnum
    }
  )~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kError);
}

}  // namespace testing
}  // namespace epoxy
