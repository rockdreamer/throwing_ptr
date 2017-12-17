//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <throwing/shared_ptr.hpp>

namespace {
struct Foo {
    int foo() const { return 42; }
};
} // namespace

TEST(Get, NullPtr) {
    throwing::shared_ptr<int[10]> nothing;
    EXPECT_EQ(nullptr, nothing.get());

    throwing::shared_ptr<int[10]> nothing_nullptr(nullptr);
    EXPECT_EQ(nullptr, nothing.get());

    throwing::shared_ptr<int[10]> nothing_null(NULL);
    EXPECT_EQ(nullptr, nothing.get());
}

TEST(Get, Get) {
    int *ptr = new int[10];
    throwing::shared_ptr<int[10]> t_ptr(ptr);
    EXPECT_EQ(ptr, t_ptr.get());
}

TEST(SquareBrackets, NullPtr) {
    throwing::shared_ptr<Foo[100]> nothing;
    EXPECT_THROW(nothing[0], throwing::base_null_ptr_exception);
    EXPECT_THROW(nothing[0], throwing::null_ptr_exception<Foo[100]>);
}

TEST(SquareBrackets, Works) {
    int *ptr = new int[10];
    throwing::shared_ptr<int[10]> t_ptr(ptr);
    EXPECT_EQ(ptr, &t_ptr[0]);
}
