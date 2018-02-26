//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/shared_ptr.hpp>

namespace {
class A {
public:
    A(bool &deleted) : deleted_(deleted) {}
    virtual ~A() { deleted_ = true; }
    A &operator=(const A &) { return *this; }
    bool &deleted_;
};

class B : public A {
    int dummy_b;

public:
    B(bool &deleted) : A(deleted) {}
    B &operator=(const B &) { return *this; }
    int dummy() { return dummy_b; }
};
} // namespace

TEST_CASE("shared_ptr reset with no arguments clears", "[shared_ptr][reset]") {
    bool deleted = false;
    A *ptr1 = new A(deleted);
    throwing::shared_ptr<A> t_ptr1(ptr1);
    REQUIRE(t_ptr1.get() == ptr1);
    REQUIRE_FALSE(deleted);
    t_ptr1.reset();
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(deleted);
}

TEST_CASE("shared_ptr reset with pointer", "[shared_ptr][reset]") {
    bool deleted1 = false;
    A *ptr1 = new A(deleted1);
    bool deleted2 = false;
    B *ptr2 = new B(deleted2);

    throwing::shared_ptr<A> t_ptr1(ptr1);
    REQUIRE(t_ptr1.get() == ptr1);
    REQUIRE_FALSE(deleted1);
    t_ptr1.reset(ptr2);
    REQUIRE(t_ptr1.get() == ptr2);
    REQUIRE(deleted1);
}

TEST_CASE("shared_ptr reset with pointer and deleter", "[shared_ptr][reset]") {
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
    REQUIRE(t_ptr1.get() == ptr1);
    REQUIRE_FALSE(deleted1);

    t_ptr1.reset(ptr2, deleter);
    REQUIRE_FALSE(lamda_called);
    REQUIRE(deleted1);
    REQUIRE(t_ptr1.get() == ptr2);

    t_ptr1.reset();
    REQUIRE(deleted2);
    REQUIRE(lamda_called);
    REQUIRE(t_ptr1.get() == nullptr);
}

TEST_CASE("shared_ptr reset with pointer, deleter and allocator",
          "[shared_ptr][reset]") {
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
    REQUIRE(t_ptr1.get() == ptr1);
    REQUIRE_FALSE(deleted1);

    std::allocator<void *> allocator;
    t_ptr1.reset(ptr2, deleter, allocator);
    REQUIRE_FALSE(lamda_called);
    REQUIRE(deleted1);
    REQUIRE(t_ptr1.get() == ptr2);

    t_ptr1.reset();
    REQUIRE(deleted2);
    REQUIRE(lamda_called);
    REQUIRE(t_ptr1.get() == nullptr);
}
