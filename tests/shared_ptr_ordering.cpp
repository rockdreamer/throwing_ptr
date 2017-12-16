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
};

TEST(Ordering, OwnerBefore) {
    Foo *ptr = new Foo(1, 2);
    throwing::shared_ptr<Foo> p1(ptr);
    throwing::shared_ptr<int> p2(p1, &p1->n1);
    throwing::shared_ptr<int> p3(p1, &p1->n2);
    // EXPECT_TRUE(p2 < p3);
    // EXPECT_FALSE(p3 < p2);
    EXPECT_FALSE(p2.owner_before(p3));
    EXPECT_FALSE(p3.owner_before(p2));
}
