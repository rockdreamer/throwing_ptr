//          Copyright Claudio Bantaloukas 2017.
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

class B : public A {
    int dummy_b;

public:
    int dummy() { return dummy_b; }
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

TEST_CASE("move assignment from throwing::unique_ptr",
          "[unique_ptr][assignment]") {
    A *ptr1 = new A;
    throwing::unique_ptr<A> t_ptr1(ptr1);
    throwing::unique_ptr<A> t_ptr2;
    t_ptr2 = std::move(t_ptr1);
    REQUIRE(t_ptr2.get() == ptr1);
    REQUIRE(t_ptr1.get() == nullptr);
}

TEST_CASE("move assignment from throwing::unique_ptr to derived class",
          "[unique_ptr][assignment]") {
    B *ptr1 = new B;
    throwing::unique_ptr<B> t_ptr1(ptr1);
    throwing::unique_ptr<A> t_ptr2;
    t_ptr2 = std::move(t_ptr1);
    REQUIRE(t_ptr2.get() == ptr1);
    REQUIRE(t_ptr1.get() == nullptr);
}

TEST_CASE("assignment from nullptr", "[unique_ptr][assignment][nullptr]") {
    DeleterA d;
    throwing::unique_ptr<A, DeleterA &> t_ptr(new A, d);
    REQUIRE(t_ptr.get());
    t_ptr = nullptr;
    REQUIRE_FALSE(t_ptr.get());
    REQUIRE(d.called);
}
