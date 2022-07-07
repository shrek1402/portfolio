#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Base/Test/MI.GTestUtil.h"
#include "MI.StaticVector.h"

#ifndef MI
  #define MI ::mi::
#endif

namespace mi::test {

class value_ndc {
  public:
    explicit value_ndc(int a)
        : aa(a) {
    }

    ~value_ndc() {
    }

    bool operator==(const value_ndc& v) const {
      return aa == v.aa;
    }

    bool operator<(const value_ndc& v) const {
      return aa < v.aa;
    }

  private:
    value_ndc(const value_ndc&) {
    }

    value_ndc& operator=(const value_ndc&) {
      return *this;
    }

    int aa;
};

class value_nd {
  public:
    explicit value_nd(int a)
        : aa(a) {
    }

    ~value_nd() {
    }

    bool operator==(const value_nd& v) const {
      return aa == v.aa;
    }

    bool operator<(const value_nd& v) const {
      return aa < v.aa;
    }

  private:
    int aa;
};

class value_nc {
  public:
    explicit value_nc(int a = 0)
        : aa(a) {
    }

    ~value_nc() {
    }

    value_nc& operator=(int a) {
      aa = a;
      return *this;
    }

    bool operator==(const value_nc& v) const {
      return aa == v.aa;
    }

    bool operator<(const value_nc& v) const {
      return aa < v.aa;
    }

  private:
    value_nc(const value_nc&) {
    }

    value_nc& operator=(const value_nc&) {
      return *this;
    }

    int aa;
};

class counting_value {
  public:
    explicit counting_value(int a = 0, int b = 0)
        : aa(a),
          bb(b) {
      ++c();
    }

    counting_value(const counting_value& v)
        : aa(v.aa),
          bb(v.bb) {
      ++c();
    }

    counting_value(counting_value&& p) noexcept
        : aa(p.aa),
          bb(p.bb) {
      p.aa = 0;
      p.bb = 0;
      ++c();
    }  // Move constructor

    counting_value& operator=(counting_value&& p) noexcept {
      aa   = p.aa;
      p.aa = 0;
      bb   = p.bb;
      p.bb = 0;
      return *this;
    }  // Move assignment

    counting_value& operator=(const counting_value& p) {
      aa = p.aa;
      bb = p.bb;
      return *this;
    }  // Copy assignment

    counting_value& operator=(int a) {
      aa = a;
      return *this;
    }  // Copy assignment

    ~counting_value() {
      --c();
    }

    bool operator==(const counting_value& v) const {
      return aa == v.aa && bb == v.bb;
    }

    bool operator<(const counting_value& v) const {
      return aa < v.aa || (aa == v.aa && bb < v.bb);
    }

    static size_t count() {
      return c();
    }

  private:
    static size_t& c() {
      static size_t co = 0;
      return co;
    }

    int aa, bb;
};

TEST(StaticVector, DefaultConstructor) {
  MI static_vector<int, 1> a;
  MI static_vector<int, 3> b;
}

TEST(StaticVector, CopyConstructor) {
  MI static_vector<int, 3> a = {1, 2, 3};
  MI static_vector<int, 3> b(a);

  EXPECT_THAT(a, testing::ElementsAre(1, 2, 3));
  EXPECT_THAT(b, testing::ElementsAre(1, 2, 3));
}

TEST(StaticVector, MoveConstructor) {
  MI static_vector<int, 3> a = {1, 2, 3};
  MI static_vector<int, 3> b(STD move(a));

  EXPECT_TRUE(a.empty());
  EXPECT_THAT(b, testing::ElementsAre(1, 2, 3));
}

TEST(StaticVector, CopyOperator) {
  MI static_vector<int, 3> a = {1, 2, 3};
  MI static_vector<int, 3> b;

  b = a;

  EXPECT_THAT(a, testing::ElementsAre(1, 2, 3));
  EXPECT_THAT(b, testing::ElementsAre(1, 2, 3));
}

TEST(StaticVector, MoveOperator) {
  MI static_vector<int, 3> a = {1, 2, 3};
  MI static_vector<int, 3> b;

  b = STD move(a);

  EXPECT_TRUE(a.empty());
  EXPECT_THAT(b, testing::ElementsAre(1, 2, 3));
}

TEST(StaticVector, InitListConstructor) {
  MI static_vector<int, 2> a(STD initializer_list{1, 2});

  EXPECT_THAT(a, testing::ElementsAre(1, 2));
}

TEST(StaticVector, InitListOperator) {
  MI static_vector<int, 2> a = STD initializer_list{1, 2};

  EXPECT_THAT(a, testing::ElementsAre(1, 2));
}

TEST(StaticVector, IteratorConstructor) {
  STD initializer_list     init_list = {1, 2};
  MI static_vector<int, 2> a(init_list.begin(), init_list.end());

  EXPECT_THAT(a, testing::ElementsAre(1, 2));
}

TEST(StaticVector, Size) {
  MI static_vector<int, 3> m_0;
  MI static_vector<int, 3> m_2 = {
    {0, 1}
  };

  GTEST_ASSERT_EQ(m_0.size(), 0);
  GTEST_ASSERT_EQ(m_2.size(), 2);
}

TEST(StaticVector, MaxSize) {
  MI static_vector<int, 3> m;

  GTEST_ASSERT_EQ(m.max_size(), 3);
  GTEST_ASSERT_EQ((MI static_vector<int, 3>::max_size()), 3);
}

TEST(StaticVector, Empty) {
  MI static_vector<int, 3> m_a;

  MI static_vector<int, 3> m_b = {
    {1, 2, 3}
  };

  GTEST_ASSERT_TRUE(m_a.empty());
  GTEST_ASSERT_TRUE(!m_b.empty());
}

TEST(StaticVector, Full) {
  MI static_vector<int, 3> m_a;

  MI static_vector<int, 3> m_b = {
    {1, 2, 3}
  };

  GTEST_ASSERT_TRUE(!m_a.full());
  GTEST_ASSERT_TRUE(m_b.full());
}

TEST(StaticVector, Fill) {
  MI static_vector<int, 3> m;
  m.fill(666);

  GTEST_ASSERT_EQ(m[0], 666);
  GTEST_ASSERT_EQ(m[1], 666);
  GTEST_ASSERT_EQ(m[2], 666);
}

TEST(StaticVector, Swap) {
  const MI static_vector<int, 3> m_a_const = {1, 2, 3};
  const MI static_vector<int, 3> m_b_const = {4, 5, 6};
  MI static_vector<int, 3>       m_a       = m_a_const;
  MI static_vector<int, 3>       m_b       = m_b_const;

  m_a.swap(m_b);

  GTEST_ASSERT_EQ(m_a, m_b_const);
  GTEST_ASSERT_EQ(m_b, m_a_const);
}

TEST(StaticVector, Iterators) {
  constexpr MI static_vector<int, 3> m = {1, 2, 3};
  constexpr MI static_vector<int, 3> m_empty;

  static_assert(m_empty.begin() == m_empty.end(), "");

  static_assert(m.begin() != m.end(), "");
}

TEST(StaticVector, At) {
  constexpr MI static_vector<int, 3> m = {1, 2, 3};

  GTEST_ASSERT_EQ(m.at(0), 1);
  GTEST_ASSERT_EQ(m.at(1), 2);
  GTEST_ASSERT_EQ(m.at(2), 3);
  MI_EXPECT_CHECK_DEATH((MI_DISABLE_4834)m.at(3));
}

}  // namespace mi::test
