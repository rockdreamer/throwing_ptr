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

struct A_D {
    void operator()(A *p) const { delete p; }
};

struct Contained {
    // some data that we want to point to
    int dummy_;
    int dummy() { return dummy_; }
};

struct Container {
    Contained things;
};
} // namespace

TEST_CASE("shared_ptr constructor from pointer", "[shared_ptr][constructor]") {
    A *ptr1 = new A;
    throwing::shared_ptr<A> t_ptr1(ptr1);
    REQUIRE(t_ptr1.get() == ptr1);
}

TEST_CASE("shared_ptr constructor from pointer and deleter",
          "[shared_ptr][constructor]") {
    A *ptr1 = new A;
    throwing::shared_ptr<A> t_ptr1(ptr1, A_D());
    REQUIRE(t_ptr1.get() == ptr1);
    REQUIRE(throwing::get_deleter<A_D>(t_ptr1) != nullptr);
}

TEST_CASE("shared_ptr constructor from pointer and lambda deleter",
          "Construction") {
    A *ptr1 = new A;
    bool lamda_called = false;
    {
        throwing::shared_ptr<A> t_ptr1(ptr1, [&lamda_called](A *p) {
            delete p;
            lamda_called = true;
        });
        REQUIRE(t_ptr1.get() == ptr1);
        REQUIRE_FALSE(lamda_called);
    }
    REQUIRE(lamda_called);
}

TEST_CASE("shared_ptr constructor from pointer to derived class",
          "[shared_ptr][constructor]") {
    B *ptr1 = new B;
    throwing::shared_ptr<A> t_ptr1(ptr1);
    REQUIRE(t_ptr1.get() == ptr1);
}

TEST_CASE("shared_ptr constructor from pointer to base type",
          "[shared_ptr][constructor]") {
    int *ptr1 = new int;
    throwing::shared_ptr<int> t_ptr1(ptr1);
    REQUIRE(t_ptr1.get() == ptr1);
}

TEST_CASE("shared_ptr constructor from nullptr and deleter",
          "[shared_ptr][constructor]") {
    bool lamda_called = false;
    {
        throwing::shared_ptr<A> t_ptr1(nullptr, [&lamda_called](A *p) {
            delete p;
            lamda_called = true;
        });
        REQUIRE(t_ptr1.get() == nullptr);
        REQUIRE_FALSE(lamda_called);
    }
    REQUIRE(lamda_called);
}

TEST_CASE("shared_ptr constructor from nullptr, lambda deleter and allocator",
          "[shared_ptr][constructor]") {
    A *ptr1 = new A;
    std::allocator<void *> allocator;
    bool lamda_called = false;
    {
        throwing::shared_ptr<A> t_ptr1(ptr1,
                                       [&lamda_called](A *p) {
                                           delete p;
                                           lamda_called = true;
                                       },
                                       allocator);
        REQUIRE(t_ptr1.get() == ptr1);
        REQUIRE_FALSE(lamda_called);
    }
    REQUIRE(lamda_called);
}

TEST_CASE("shared_ptr aliasing constructor", "[shared_ptr][constructor]") {
    Container *ptr1 = new Container;
    bool lamda_called = false;
    {
        throwing::shared_ptr<Container> t_ptr1(ptr1,
                                               [&lamda_called](Container *p) {
                                                   delete p;
                                                   lamda_called = true;
                                               });
        REQUIRE(t_ptr1.get() == ptr1);
        REQUIRE_FALSE(lamda_called);
        auto via_aliasing =
                throwing::shared_ptr<Contained>(t_ptr1, &ptr1->things);
        REQUIRE_FALSE(lamda_called);
        t_ptr1.reset();
        REQUIRE_FALSE(lamda_called);
        via_aliasing.reset();
        REQUIRE(lamda_called);
    }
}

TEST_CASE("shared_ptr copy constructor", "[shared_ptr][constructor]") {
    A *ptr1 = new A;
    auto t_ptr1 = throwing::shared_ptr<A>(ptr1);
    throwing::shared_ptr<A> t_ptr2(t_ptr1);
    REQUIRE(t_ptr2.get() == t_ptr1.get());
    t_ptr1.reset();
    REQUIRE(t_ptr2.get() == ptr1);
}

TEST_CASE("shared_ptr copy constructor from derived", "Construction") {
    B *ptr1 = new B;
    auto t_ptr1 = throwing::shared_ptr<B>(ptr1);
    throwing::shared_ptr<A> t_ptr2 = t_ptr1;
    REQUIRE(t_ptr2.get() == t_ptr1.get());
    t_ptr1.reset();
    REQUIRE(t_ptr2.get() == ptr1);
}

TEST_CASE("shared_ptr move constructor", "[shared_ptr][constructor]") {
    A *ptr1 = new A;
    auto t_ptr1 = throwing::shared_ptr<A>(ptr1);
    throwing::shared_ptr<A> t_ptr2(std::move(t_ptr1));
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() == ptr1);
}

TEST_CASE("shared_ptr move constructor from derived",
          "[shared_ptr][constructor]") {
    B *ptr1 = new B;
    auto t_ptr1 = throwing::shared_ptr<B>(ptr1);
    throwing::shared_ptr<A> t_ptr2(std::move(t_ptr1));
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() == ptr1);
}

TEST_CASE("shared_ptr constructor from std::shared_ptr",
          "[shared_ptr][constructor]") {
    A *ptr1 = new A;
    auto t_ptr1 = std::shared_ptr<A>(ptr1);
    throwing::shared_ptr<A> t_ptr2(t_ptr1);
    REQUIRE(t_ptr2.get() == t_ptr1.get());
    t_ptr1.reset();
    REQUIRE(t_ptr2.get() == ptr1);
}

TEST_CASE("shared_ptr constructor from std::shared_ptr to derived",
          "[shared_ptr][constructor]") {
    B *ptr1 = new B;
    auto t_ptr1 = std::shared_ptr<B>(ptr1);
    throwing::shared_ptr<A> t_ptr2(t_ptr1);
    REQUIRE(t_ptr2.get() == t_ptr1.get());
    t_ptr1.reset();
    REQUIRE(t_ptr2.get() == ptr1);
}

TEST_CASE("shared_ptr move constructor from std::shared_ptr",
          "[shared_ptr][constructor]") {
    A *ptr1 = new A;
    auto t_ptr1 = std::shared_ptr<A>(ptr1);
    throwing::shared_ptr<A> t_ptr2(std::move(t_ptr1));
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() == ptr1);
}

TEST_CASE("shared_ptr move constructor from std::shared_ptr to derived",
          "[shared_ptr][constructor]") {
    B *ptr1 = new B;
    auto t_ptr1 = std::shared_ptr<B>(ptr1);
    throwing::shared_ptr<A> t_ptr2(std::move(t_ptr1));
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() == ptr1);
}

TEST_CASE("shared_ptr constructor from derived std::weak_ptr",
          "[shared_ptr][constructor]") {
    B *ptr1 = new B;
    auto t_ptr1 = std::shared_ptr<B>(ptr1);
    std::weak_ptr<B> weak = t_ptr1;
    throwing::shared_ptr<A> t_ptr2(weak);
    REQUIRE(t_ptr2.get() == t_ptr1.get());
    t_ptr1.reset();
    REQUIRE(t_ptr2.get() == ptr1);
}

TEST_CASE("shared_ptr move constructor from std::unique_ptr",
          "[shared_ptr][constructor]") {
    B *ptr1 = new B;
    auto u_ptr1 = std::unique_ptr<B>(ptr1);
    throwing::shared_ptr<A> t_ptr2(std::move(u_ptr1));
    REQUIRE(u_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() == ptr1);
}
