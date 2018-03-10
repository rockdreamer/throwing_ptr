//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
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

TEST_CASE("assignment from throwing::shared_ptr", "[shared_ptr][assignment]") {
    A *ptr1 = new A;
    throwing::shared_ptr<A> t_ptr1(ptr1);
    throwing::shared_ptr<A> t_ptr2;
    t_ptr2 = t_ptr1;
    REQUIRE(t_ptr2.get() == t_ptr1.get());
    t_ptr1.reset();
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() == ptr1);
}

TEST_CASE("assignment from std::shared_ptr", "[shared_ptr][assignment]") {
    A *ptr1 = new A;
    std::shared_ptr<A> t_ptr1(ptr1);
    throwing::shared_ptr<A> t_ptr2;
    t_ptr2 = t_ptr1;
    REQUIRE(t_ptr2.get() == t_ptr1.get());
    t_ptr1.reset();
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() == ptr1);
}

TEST_CASE("assignment from throwing::shared_ptr to derived class",
          "[shared_ptr][assignment]") {
    throwing::shared_ptr<B> t_ptr1(new B);
    throwing::shared_ptr<A> t_ptr2;
    t_ptr2 = t_ptr1;
    REQUIRE(t_ptr2.get() == t_ptr1.get());
}

TEST_CASE("assignment from std::shared_ptr to derived class",
          "[shared_ptr][assignment]") {
    std::shared_ptr<B> t_ptr1(new B);
    throwing::shared_ptr<A> t_ptr2;
    t_ptr2 = t_ptr1;
    REQUIRE(t_ptr2.get() == t_ptr1.get());
}

TEST_CASE("move assignment from throwing::shared_ptr to derived class",
          "[shared_ptr][assignment]") {
    B *ptr = new B;
    throwing::shared_ptr<B> t_ptr1(ptr);
    throwing::shared_ptr<A> t_ptr2;
    t_ptr2 = std::move(t_ptr1);
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() == ptr);
}

TEST_CASE("move assignment from std::shared_ptr to derived class",
          "[shared_ptr][assignment]") {
    B *ptr = new B;
    std::shared_ptr<B> t_ptr1(ptr);
    throwing::shared_ptr<A> t_ptr2;
    t_ptr2 = std::move(t_ptr1);
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() == ptr);
}
