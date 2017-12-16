//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <sstream>
#include <throwing/shared_ptr.hpp>

TEST(Stream, Operator) {
    auto ptr = throwing::make_shared<int>();
    std::stringstream ss_ptr;
    ss_ptr << ptr.get();
    std::stringstream ss_tptr;
    ss_tptr << ptr;
    EXPECT_EQ(ss_ptr.str(), ss_tptr.str());
}

TEST(Stream, Null) {
    throwing::shared_ptr<int> ptr;
    std::stringstream ss_ptr;
    ss_ptr << ptr.get();
    std::stringstream ss_tptr;
    ss_tptr << ptr;
    EXPECT_EQ(ss_ptr.str(), ss_tptr.str());
}
