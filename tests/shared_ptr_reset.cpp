//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <throwing/shared_ptr.hpp>

namespace {
class A {
public:
    A(bool &deleted) : deleted(deleted) {}
    virtual ~A() { deleted = true; }
    A &operator=(const A &) { return *this; }
    bool &deleted;
};

class B : public A {
    int dummy_b;

public:
    B(bool &deleted) : A(deleted) {}
    B &operator=(const B &) { return *this; }
    int dummy() { return dummy_b; }
};
} // namespace

TEST(Reset, Reset) {
    bool deleted = false;
    A *ptr1 = new A(deleted);
    throwing::shared_ptr<A> t_ptr1(ptr1);
    EXPECT_EQ(ptr1, t_ptr1.get());
    EXPECT_FALSE(deleted);
    t_ptr1.reset();
    EXPECT_EQ(nullptr, t_ptr1.get());
    EXPECT_TRUE(deleted);
}

TEST(Reset, ResetWithPtr) {
    bool deleted1 = false;
    A *ptr1 = new A(deleted1);
    bool deleted2 = false;
    B *ptr2 = new B(deleted2);

    throwing::shared_ptr<A> t_ptr1(ptr1);
    EXPECT_EQ(ptr1, t_ptr1.get());
    EXPECT_FALSE(deleted1);
    t_ptr1.reset(ptr2);
    EXPECT_EQ(ptr2, t_ptr1.get());
    EXPECT_TRUE(deleted1);
}

TEST(Reset, ResetWithPtrAndDeleter) {
    bool deleted1 = false;
    A *ptr1 = new A(deleted1);
    bool deleted2 = false;
    B *ptr2 = new B(deleted2);

    bool lamda_called = false;
    auto deleter = [&lamda_called](A *p) {
        delete p;
        lamda_called = true;
    };

    throwing::shared_ptr<A> t_ptr1(ptr1);
    EXPECT_EQ(ptr1, t_ptr1.get());
    EXPECT_FALSE(deleted1);

    t_ptr1.reset(ptr2, deleter);
    EXPECT_FALSE(lamda_called);
    EXPECT_TRUE(deleted1);
    EXPECT_EQ(ptr2, t_ptr1.get());

    t_ptr1.reset();
    EXPECT_TRUE(deleted2);
    EXPECT_TRUE(lamda_called);
    EXPECT_EQ(nullptr, t_ptr1.get());
}

TEST(Reset, ResetWithPtrAndDeleterAndAllocator) {
    bool deleted1 = false;
    A *ptr1 = new A(deleted1);
    bool deleted2 = false;
    B *ptr2 = new B(deleted2);

    bool lamda_called = false;
    auto deleter = [&lamda_called](A *p) {
        delete p;
        lamda_called = true;
    };

    throwing::shared_ptr<A> t_ptr1(ptr1);
    EXPECT_EQ(ptr1, t_ptr1.get());
    EXPECT_FALSE(deleted1);

    std::allocator<void *> allocator;
    t_ptr1.reset(ptr2, deleter, allocator);
    EXPECT_FALSE(lamda_called);
    EXPECT_TRUE(deleted1);
    EXPECT_EQ(ptr2, t_ptr1.get());

    t_ptr1.reset();
    EXPECT_TRUE(deleted2);
    EXPECT_TRUE(lamda_called);
    EXPECT_EQ(nullptr, t_ptr1.get());
}
