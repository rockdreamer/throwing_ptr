//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <throwing/unique_ptr.hpp>

namespace {
struct Foo {
    int foo() const { return 42; }
};
} // namespace

TEST(UniquePtrToArray, NullPtr) {
	throwing::unique_ptr<int[]> nothing;
	EXPECT_EQ(nullptr, nothing.get());

	throwing::unique_ptr<int[]> nothing_nullptr(nullptr);
	EXPECT_EQ(nullptr, nothing.get());

	throwing::unique_ptr<int[]> nothing_null((int*)NULL);
    EXPECT_EQ(nullptr, nothing.get());
}

TEST(UniquePtrToArray, Get) {
    int *ptr = new int[10];
    throwing::unique_ptr<int[]> t_ptr(ptr);
    EXPECT_EQ(ptr, t_ptr.get());
}

TEST(UniquePtrToArray, SquareBracketsToNullPtr) {
    throwing::unique_ptr<Foo[]> nothing;
    EXPECT_THROW(nothing[0], throwing::base_null_ptr_exception);
    EXPECT_THROW(nothing[0], throwing::null_ptr_exception<Foo>);
}

TEST(UniquePtrToArray, SquareBracketsWorks) {
    int *ptr = new int[10];
    throwing::unique_ptr<int[]> t_ptr(ptr);
    EXPECT_EQ(ptr, &t_ptr[0]);
}
