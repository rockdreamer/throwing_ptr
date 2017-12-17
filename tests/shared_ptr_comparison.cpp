//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <functional>
#include <gtest/gtest.h>
#include <throwing/shared_ptr.hpp>

TEST(Comparison, TwoPointers) {
    int *ptr1 = new int;
    int *ptr2 = new int;
    auto t_ptr1 = throwing::shared_ptr<int>(ptr1);
    auto t_ptr1_1 = t_ptr1;
    auto t_ptr2 = throwing::shared_ptr<int>(ptr2);

    EXPECT_FALSE(t_ptr1 == t_ptr2);
    EXPECT_FALSE(t_ptr1_1 == t_ptr2);
    EXPECT_TRUE(t_ptr1 == t_ptr1);
    EXPECT_TRUE(t_ptr1 == t_ptr1_1);

    EXPECT_TRUE(t_ptr1 != t_ptr2);
    EXPECT_TRUE(t_ptr1_1 != t_ptr2);
    EXPECT_FALSE(t_ptr1 != t_ptr1);
    EXPECT_FALSE(t_ptr1 != t_ptr1_1);

    EXPECT_EQ(ptr1 < ptr2, t_ptr1 < t_ptr2);
    EXPECT_EQ(ptr1 < ptr2, t_ptr1_1 < t_ptr2);
    EXPECT_FALSE(t_ptr1 < t_ptr1);
    EXPECT_FALSE(t_ptr1 < t_ptr1_1);

    EXPECT_EQ(ptr1 > ptr2, t_ptr1 > t_ptr2);
    EXPECT_EQ(ptr1 > ptr2, t_ptr1_1 > t_ptr2);
    EXPECT_FALSE(t_ptr1 > t_ptr1);
    EXPECT_FALSE(t_ptr1 > t_ptr1_1);

    EXPECT_EQ(ptr1 <= ptr2, t_ptr1 <= t_ptr2);
    EXPECT_EQ(ptr1 <= ptr2, t_ptr1_1 <= t_ptr2);
    EXPECT_EQ(ptr1 <= ptr1, t_ptr1 <= t_ptr1);
    EXPECT_EQ(ptr1 <= ptr1, t_ptr1 <= t_ptr1_1);

    EXPECT_EQ(ptr1 >= ptr2, t_ptr1 >= t_ptr2);
    EXPECT_EQ(ptr1 >= ptr2, t_ptr1_1 >= t_ptr2);
    EXPECT_EQ(ptr1 >= ptr1, t_ptr1 >= t_ptr1);
    EXPECT_EQ(ptr1 >= ptr1, t_ptr1 >= t_ptr1_1);
}

TEST(Comparison, WithNullPtr) {
    int *ptr = new int;
    auto t_ptr = throwing::shared_ptr<int>(ptr);
    auto empty_t_ptr = throwing::shared_ptr<int>();

    EXPECT_TRUE(empty_t_ptr == nullptr);
    EXPECT_FALSE(t_ptr == nullptr);
    EXPECT_TRUE(nullptr == empty_t_ptr);
    EXPECT_FALSE(nullptr == t_ptr);

    EXPECT_FALSE(empty_t_ptr != nullptr);
    EXPECT_TRUE(t_ptr != nullptr);
    EXPECT_FALSE(nullptr != empty_t_ptr);
    EXPECT_TRUE(nullptr != t_ptr);

    EXPECT_EQ(std::less<int *>()(ptr, nullptr), t_ptr < nullptr);
    EXPECT_EQ(std::less<int *>()(nullptr, nullptr), empty_t_ptr < nullptr);
    EXPECT_EQ(std::less<int *>()(nullptr, ptr), nullptr < t_ptr);
    EXPECT_EQ(std::less<int *>()(nullptr, nullptr), nullptr < empty_t_ptr);

    EXPECT_EQ(std::less<int *>()(nullptr, ptr), t_ptr > nullptr);
    EXPECT_EQ(std::less<int *>()(nullptr, nullptr), empty_t_ptr > nullptr);
    EXPECT_EQ(std::less<int *>()(ptr, nullptr), nullptr > t_ptr);
    EXPECT_EQ(std::less<int *>()(nullptr, nullptr), nullptr > empty_t_ptr);

    EXPECT_EQ(!std::less<int *>()(nullptr, ptr), t_ptr <= nullptr);
    EXPECT_EQ(!std::less<int *>()(nullptr, nullptr), empty_t_ptr <= nullptr);
    EXPECT_EQ(!std::less<int *>()(ptr, nullptr), nullptr <= t_ptr);
    EXPECT_EQ(!std::less<int *>()(nullptr, nullptr), nullptr <= empty_t_ptr);

    EXPECT_EQ(!std::less<int *>()(ptr, nullptr), t_ptr >= nullptr);
    EXPECT_EQ(!std::less<int *>()(nullptr, nullptr), empty_t_ptr >= nullptr);
    EXPECT_EQ(!std::less<int *>()(nullptr, ptr), nullptr >= t_ptr);
    EXPECT_EQ(!std::less<int *>()(nullptr, nullptr), nullptr >= empty_t_ptr);
}
