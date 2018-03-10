//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/unique_ptr.hpp>

namespace {
class A {
    int dummy_a;

public:
    int dummy() { return dummy_a; }
};

struct DeleterA {
    DeleterA() : called(false) {}
    void operator()(A *p) {
        called = true;
        delete p;
    }
    bool called;
};
} // namespace

TEST_CASE("move assignment from throwing::unique_ptr to array",
          "[unique_ptr][assignment][array]") {
    A *ptr1 = new A[10];
    throwing::unique_ptr<A[]> t_ptr1(ptr1);
    throwing::unique_ptr<A[]> t_ptr2;
    t_ptr2 = std::move(t_ptr1);
    REQUIRE(t_ptr2.get() == ptr1);
    REQUIRE(t_ptr1.get() == nullptr);
}

TEST_CASE("move assignment from std::unique_ptr to array",
          "[unique_ptr][assignment][array]") {
    A *ptr1 = new A[10];
    std::unique_ptr<A[]> t_ptr1(ptr1);
    throwing::unique_ptr<A[]> t_ptr2;
    t_ptr2 = std::move(t_ptr1);
    REQUIRE(t_ptr2.get() == ptr1);
    REQUIRE(t_ptr1.get() == nullptr);
}

TEST_CASE("assignment from nullptr to array",
          "[unique_ptr][assignment][nullptr][array]") {
    throwing::unique_ptr<A[], DeleterA> t_ptr(new A[10]);
    REQUIRE(t_ptr.get());
    t_ptr = nullptr;
    REQUIRE_FALSE(t_ptr.get());
    REQUIRE(t_ptr.get_deleter().called);
}
