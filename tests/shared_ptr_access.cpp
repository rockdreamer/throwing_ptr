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
    throwing::shared_ptr<int> nothing;
    EXPECT_EQ(nullptr, nothing.get());

    throwing::shared_ptr<int> nothing_nullptr(nullptr);
    EXPECT_EQ(nullptr, nothing.get());

    throwing::shared_ptr<int> nothing_null(NULL);
    EXPECT_EQ(nullptr, nothing.get());
}

TEST(Get, Get) {
    int *ptr = new int;
    throwing::shared_ptr<int> t_ptr(ptr);
    EXPECT_EQ(ptr, t_ptr.get());
}

TEST(Star, NullPtr) {
    throwing::shared_ptr<int> nothing;
    EXPECT_THROW((*nothing)++, throwing::base_null_ptr_exception);
    EXPECT_THROW((*nothing)++, throwing::null_ptr_exception<int>);
}

TEST(Dereference, NullPtr) {
    throwing::shared_ptr<Foo> nothing;
    EXPECT_THROW(nothing->foo(), throwing::base_null_ptr_exception);
    EXPECT_THROW(nothing->foo(), throwing::null_ptr_exception<Foo>);
}

TEST(NullPtrException, NullPtrCatchWithBase) {
    throwing::shared_ptr<int> nothing;
    try {
        (*nothing)++;
    } catch (const throwing::null_ptr_exception<float> &) {
        FAIL();
    } catch (const throwing::base_null_ptr_exception &e) {
        EXPECT_STREQ("Dereference of nullptr", e.what());
    }
}

TEST(NullPtrException, NullPtrCatchWithCorrectType) {
    throwing::shared_ptr<int> nothing;
    try {
        (*nothing)++;
    } catch (const throwing::null_ptr_exception<float> &) {
        FAIL();
    } catch (const throwing::null_ptr_exception<int> &) {
    }
}

TEST(NullPtrException, NullPtrCatchWhatType) {
    throwing::shared_ptr<int> nothing;
    try {
        (*nothing)++;
    } catch (const throwing::base_null_ptr_exception &e) {
        std::string what = e.what_type();
        EXPECT_FALSE(what.empty());
    }
}

TEST(UseCount, UseCount) {
    Foo *foo = new Foo;
    throwing::shared_ptr<Foo> ptr;
    EXPECT_EQ(0l, ptr.use_count());
    ptr.reset(foo);
    EXPECT_EQ(1l, ptr.use_count());
    auto ptr2 = ptr;
    EXPECT_EQ(2l, ptr.use_count());
    EXPECT_EQ(2l, ptr2.use_count());
    ptr.reset();
    EXPECT_EQ(0l, ptr.use_count());
    EXPECT_EQ(1l, ptr2.use_count());
    ptr2.reset();
    EXPECT_EQ(0l, ptr.use_count());
    EXPECT_EQ(0l, ptr2.use_count());
}

TEST(OperatorBool, OperatorBool) {
    Foo *foo = new Foo;
    throwing::shared_ptr<Foo> ptr;
    EXPECT_FALSE(ptr);
    ptr.reset(foo);
    EXPECT_TRUE(ptr);
    ptr.reset();
    EXPECT_FALSE(ptr);
}
