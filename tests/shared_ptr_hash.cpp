//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <throwing/shared_ptr.hpp>

TEST(SharedPtr, Hash) {
    auto p = std::make_shared<int>(42);
    throwing::shared_ptr<int> tp = p;
    EXPECT_EQ(std::hash<std::shared_ptr<int>>()(p),
              std::hash<throwing::shared_ptr<int>>()(tp));
}
