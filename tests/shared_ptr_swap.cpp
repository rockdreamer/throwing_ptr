//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <throwing/shared_ptr.hpp>

TEST(Swap, SwapPtrs) {
    int *ptr1 = new int;
    throwing::shared_ptr<int> t_ptr1(ptr1);
    int *ptr2 = new int;
    throwing::shared_ptr<int> t_ptr2(ptr2);
    EXPECT_EQ(ptr1, t_ptr1.get());
    EXPECT_EQ(ptr2, t_ptr2.get());
    t_ptr1.swap(t_ptr2);
    EXPECT_EQ(ptr2, t_ptr1.get());
    EXPECT_EQ(ptr1, t_ptr2.get());
    std::swap(t_ptr1, t_ptr2);
    EXPECT_EQ(ptr1, t_ptr1.get());
    EXPECT_EQ(ptr2, t_ptr2.get());
}

TEST(Swap, SwapNullPtrs) {
    throwing::shared_ptr<int> t_ptr1;
    throwing::shared_ptr<int> t_ptr2;
    EXPECT_EQ(nullptr, t_ptr1.get());
    EXPECT_EQ(nullptr, t_ptr2.get());
    t_ptr1.swap(t_ptr2);
    EXPECT_EQ(nullptr, t_ptr1.get());
    EXPECT_EQ(nullptr, t_ptr2.get());
    std::swap(t_ptr1, t_ptr2);
    EXPECT_EQ(nullptr, t_ptr1.get());
    EXPECT_EQ(nullptr, t_ptr2.get());
}
