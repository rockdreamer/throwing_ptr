//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <throwing/shared_ptr.hpp>

namespace {
class A {
    int dummy_a;

public:
    int dummy() { return dummy_a; }
};

class B : public A {
    int dummy_b;

public:
    int dummy() { return dummy_b; }
};

} // namespace

TEST(Assignment, FromThrowingSharedPtr) {
    A *ptr1 = new A;
    throwing::shared_ptr<A> t_ptr1(ptr1);
    throwing::shared_ptr<A> t_ptr2;
    t_ptr2 = t_ptr1;
    EXPECT_EQ(t_ptr1.get(), t_ptr2.get());
    t_ptr1.reset();
    EXPECT_EQ(nullptr, t_ptr1.get());
    EXPECT_EQ(ptr1, t_ptr2.get());
}

TEST(Assignment, FromStdSharedPtr) {
    A *ptr1 = new A;
    std::shared_ptr<A> t_ptr1(ptr1);
    throwing::shared_ptr<A> t_ptr2;
    t_ptr2 = t_ptr1;
    EXPECT_EQ(t_ptr1.get(), t_ptr2.get());
    t_ptr1.reset();
    EXPECT_EQ(nullptr, t_ptr1.get());
    EXPECT_EQ(ptr1, t_ptr2.get());
}

TEST(Assignment, FromThrowingPtrToDerived) {
    throwing::shared_ptr<B> t_ptr1(new B);
    throwing::shared_ptr<A> t_ptr2;
    t_ptr2 = t_ptr1;
    EXPECT_EQ(t_ptr1.get(), t_ptr2.get());
}

TEST(Assignment, FromSharedPtrToDerived) {
    std::shared_ptr<B> t_ptr1(new B);
    throwing::shared_ptr<A> t_ptr2;
    t_ptr2 = t_ptr1;
    EXPECT_EQ(t_ptr1.get(), t_ptr2.get());
}

TEST(Assignment, MoveThrowingPtrToDerived) {
    B *ptr = new B;
    throwing::shared_ptr<B> t_ptr1(ptr);
    throwing::shared_ptr<A> t_ptr2;
    t_ptr2 = std::move(t_ptr1);
    EXPECT_EQ(nullptr, t_ptr1.get());
    EXPECT_EQ(ptr, t_ptr2.get());
}

TEST(Assignment, MoveSharedPtrToDerived) {
    B *ptr = new B;
    std::shared_ptr<B> t_ptr1(ptr);
    throwing::shared_ptr<A> t_ptr2;
    t_ptr2 = std::move(t_ptr1);
    EXPECT_EQ(nullptr, t_ptr1.get());
    EXPECT_EQ(ptr, t_ptr2.get());
}

TEST(Assignment, MoveDerivedStdUniquePtr) {
    B *ptr1 = new B;
    std::unique_ptr<B> u_ptr1(ptr1);
    throwing::shared_ptr<A> t_ptr2;
    t_ptr2 = std::move(u_ptr1);
    EXPECT_EQ(nullptr, u_ptr1.get());
    EXPECT_EQ(ptr1, t_ptr2.get());
}
