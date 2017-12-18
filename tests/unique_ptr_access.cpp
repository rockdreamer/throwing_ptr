//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <throwing/unique_ptr.hpp>

TEST(Get, NullPtr) {
    throwing::unique_ptr<int> nothing;
    EXPECT_EQ(nullptr, nothing.get());
}

TEST(Get, NullPtrArray) {
    throwing::unique_ptr<int[]> nothing;
    EXPECT_EQ(nullptr, nothing.get());
}
