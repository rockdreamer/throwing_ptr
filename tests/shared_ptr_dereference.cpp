//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <throwing/shared_ptr.hpp>

TEST(Dereference, Default) {
    throwing::shared_ptr<int> nothing;
    EXPECT_THROW(nothing.get(), std::runtime_error);
}
