//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <throwing/shared_ptr.hpp>

struct Foo {
    int n1;
    int n2;
    Foo(int a, int b) : n1(a), n2(b) {}
    Foo() : n1(0), n2(0) {}
};

TEST(MakeShared, StructWithArgs) {
    auto ptr = throwing::make_shared<Foo>(1, 2);
    EXPECT_EQ(1, ptr->n1);
    EXPECT_EQ(2, ptr->n2);
}

TEST(MakeShared, StructNoArgs) {
    auto ptr = throwing::make_shared<Foo>();
    EXPECT_EQ(0, ptr->n1);
    EXPECT_EQ(0, ptr->n2);
}

TEST(MakeShared, BaseType) {
    auto ptr = throwing::make_shared<int>(42);
    EXPECT_EQ(42, *ptr);
}

TEST(MakeShared, BaseTypeNoArg) {
    auto ptr = throwing::make_shared<int>();
    EXPECT_TRUE(ptr);
}

TEST(AllocateShared, StructWithArgs) {
    std::allocator<Foo> allocator;
    auto ptr = throwing::allocate_shared<Foo>(allocator, 1, 2);
    EXPECT_EQ(1, ptr->n1);
    EXPECT_EQ(2, ptr->n2);
}

TEST(AllocateShared, StructNoArgs) {
    std::allocator<Foo> allocator;
    auto ptr = throwing::allocate_shared<Foo>(allocator);
    EXPECT_EQ(0, ptr->n1);
    EXPECT_EQ(0, ptr->n2);
}

TEST(AllocateShared, BaseType) {
    std::allocator<int> allocator;
    auto ptr = throwing::allocate_shared<int>(allocator, 42);
    EXPECT_EQ(42, *ptr);
}

TEST(AllocateShared, BaseTypeNoArg) {
    std::allocator<int> allocator;
    auto ptr = throwing::allocate_shared<int>(allocator);
    EXPECT_TRUE(ptr);
}
