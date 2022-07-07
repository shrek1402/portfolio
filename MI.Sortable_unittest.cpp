
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Base/Ranges/MI.Algorithm.h"
#include "Container/MI.Sortable.h"

namespace mi::test {
TEST(Sortable, DefaultConstructor) {
  MI sortable<int, 3> value;

  EXPECT_TRUE(value.size() == 0);
  EXPECT_TRUE(value.empty());

  EXPECT_THAT(value, testing::ElementsAre());
}

TEST(Sortable, ArgsConstructor) {
  MI sortable<int, 3> sort   = {0, 1, 2};
  MI sortable<int, 3> unsort = {2, 0, 1};

  EXPECT_THAT(sort, testing::ElementsAre(0, 1, 2));
  EXPECT_THAT(unsort, testing::ElementsAre(0, 1, 2));

  EXPECT_TRUE(MI ranges::is_sorted(sort));
  EXPECT_TRUE(MI ranges::is_sorted(unsort));
}

TEST(Sortable, CopyConstructor) {
  MI sortable<int, 3> a = {2, 0, 1};
  MI sortable<int, 3> b = a;

  EXPECT_THAT(a, testing::ElementsAre(0, 1, 2));
  EXPECT_THAT(b, testing::ElementsAre(0, 1, 2));

  EXPECT_TRUE(MI ranges::is_sorted(a));
  EXPECT_TRUE(MI ranges::is_sorted(b));
}

TEST(Sortable, MoveConstructor) {
  MI sortable<int, 3> a = {2, 0, 1};
  MI sortable<int, 3> b = STD move(a);

  EXPECT_TRUE(a.empty());
  EXPECT_EQ(a.size(), 0);
  EXPECT_THAT(a, testing::ElementsAre());

  EXPECT_FALSE(b.empty());
  EXPECT_EQ(b.size(), 3);
  EXPECT_THAT(b, testing::ElementsAre(0, 1, 2));

  EXPECT_TRUE(MI ranges::is_sorted(a));
  EXPECT_TRUE(MI ranges::is_sorted(b));
}

TEST(Sortable, CopyOperator) {
  MI sortable<int, 3> a = {2, 0, 1};
  MI sortable<int, 3> b;

  b = a;

  ASSERT_THAT(a, testing::ElementsAre(0, 1, 2));
  ASSERT_THAT(b, testing::ElementsAre(0, 1, 2));

  EXPECT_TRUE(MI ranges::is_sorted(a));
  EXPECT_TRUE(MI ranges::is_sorted(b));
}

TEST(Sortable, MoveOperator) {
  MI sortable<int, 3> a = {2, 0, 1};
  MI sortable<int, 3> b;

  b = STD move(a);

  EXPECT_TRUE(a.empty());
  EXPECT_EQ(a.size(), 0);
  EXPECT_THAT(a, testing::ElementsAre());

  EXPECT_FALSE(b.empty());
  EXPECT_EQ(b.size(), 3);
  EXPECT_THAT(b, testing::ElementsAre(0, 1, 2));

  EXPECT_TRUE(MI ranges::is_sorted(a));
  EXPECT_TRUE(MI ranges::is_sorted(b));
}

TEST(Sortable, FillConstructor) {
  // MI sortable<int, 3> a(2);
  //
  // ASSERT_THAT(a, testing::ElementsAre(2, 2, 2));
}

TEST(Sortable, InitListConstructor) {
  MI sortable<int, 3> sort({2, 0, 1});

  ASSERT_THAT(sort, testing::ElementsAre(0, 1, 2));

  EXPECT_TRUE(MI ranges::is_sorted(sort));
}

TEST(Sortable, InitListOperator) {
  MI sortable<int, 3> sort = {2, 0, 1};

  ASSERT_THAT(sort, testing::ElementsAre(0, 1, 2));
  EXPECT_TRUE(MI ranges::is_sorted(sort));
}

TEST(Sortable, IteratorConstructor) {
  STD array<int, 3>   ar = {2, 0, 1};
  MI sortable<int, 3> sort(ar.begin(), ar.end());

  ASSERT_THAT(ar, testing::ElementsAre(2, 0, 1));
  ASSERT_THAT(sort, testing::ElementsAre(0, 1, 2));
  EXPECT_TRUE(MI ranges::is_sorted(sort));
}

TEST(Sortable, SequenceCopyConstructor) {
  int ar[3] = {2, 0, 1};

  MI sortable<int, 3> sort(ar);

  ASSERT_THAT(sort, testing::ElementsAre(0, 1, 2));
  EXPECT_TRUE(MI ranges::is_sorted(sort));
}

TEST(Sortable, SequenceMoveConstructor) {
  int ar[3] = {2, 0, 1};

  MI sortable<int, 3> sort(STD move(ar));

  ASSERT_THAT(sort, testing::ElementsAre(0, 1, 2));
  EXPECT_TRUE(MI ranges::is_sorted(sort));
}

TEST(Sortable, Swap) {
  MI sortable<int, 3> lhs({0, 1, 2});
  MI sortable<int, 3> rhs({3, 4, 5});

  lhs.swap(rhs);

  ASSERT_THAT(lhs, testing::ElementsAre(3, 4, 5));
  ASSERT_THAT(rhs, testing::ElementsAre(0, 1, 2));

  MI sortable<int, 3> lhs1({0, 1, 2});
  MI sortable<int, 3> rhs1({3, 4, 5});

  MI swap(lhs1, rhs1);

  ASSERT_THAT(lhs1, testing::ElementsAre(3, 4, 5));
  ASSERT_THAT(rhs1, testing::ElementsAre(0, 1, 2));

  EXPECT_TRUE(MI ranges::is_sorted(lhs));
  EXPECT_TRUE(MI ranges::is_sorted(rhs));
}

TEST(Sortable, Size1) {
  MI sortable1n s = {123};

  EXPECT_THAT(s, testing::ElementsAre(123));
  EXPECT_TRUE(MI ranges::is_sorted(s));
}

TEST(Sortable, Size2) {
  MI sortable2n s = {123, 2};

  EXPECT_THAT(s, testing::ElementsAre(2, 123));
  EXPECT_TRUE(MI ranges::is_sorted(s));
}

TEST(Sortable, Constexpr) {
  MI sortable2n           s1 = {2, 1};
  constexpr MI sortable2n s2 = {2, 1};
  constexpr MI sortable3n s3 = {2, 1, 0};
  constexpr MI sortable4n s4 = {2, 1, 0, 13};
  constexpr MI sortable5n s5 = {11, 2, 1, 0, 13};

  EXPECT_EQ(s1, s2);
}
}  // namespace mi::test
