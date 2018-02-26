//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/shared_ptr.hpp>

struct Foo {
    int n1;
    int n2;
    Foo(int a, int b) : n1(a), n2(b) {}
    Foo() : n1(0), n2(0) {}
};

TEST_CASE("make_shared struct with arguments", "[shared_ptr][make_shared]") {
    auto ptr = throwing::make_shared<Foo>(1, 2);
    REQUIRE(ptr->n1 == 1);
    REQUIRE(ptr->n2 == 2);
}

TEST_CASE("make_shared struct with no arguments", "[shared_ptr][make_shared]") {
    auto ptr = throwing::make_shared<Foo>();
    REQUIRE(ptr->n1 == 0);
    REQUIRE(ptr->n2 == 0);
}

TEST_CASE("make_shared base type with argument", "[shared_ptr][make_shared]") {
    auto ptr = throwing::make_shared<int>(42);
    REQUIRE(*ptr == 42);
}

TEST_CASE("make_shared base type no arguments", "[shared_ptr][make_shared]") {
    auto ptr = throwing::make_shared<int>();
    REQUIRE(ptr);
}

TEST_CASE("allocate_shared with arguments", "[shared_ptr][allocate_shared]") {
    std::allocator<Foo> allocator;
    auto ptr = throwing::allocate_shared<Foo>(allocator, 1, 2);
    REQUIRE(ptr->n1 == 1);
    REQUIRE(ptr->n2 == 2);
}

TEST_CASE("allocate_shared with no arguments",
          "[shared_ptr][allocate_shared]") {
    std::allocator<Foo> allocator;
    auto ptr = throwing::allocate_shared<Foo>(allocator);
    REQUIRE(ptr->n1 == 0);
    REQUIRE(ptr->n2 == 0);
}

TEST_CASE("allocate_shared base type with arguments",
          "[shared_ptr][allocate_shared]") {
    std::allocator<int> allocator;
    auto ptr = throwing::allocate_shared<int>(allocator, 42);
    REQUIRE(*ptr == 42);
}

TEST_CASE("allocate_shared base type with no arguments",
          "[shared_ptr][allocate_shared]") {
    std::allocator<int> allocator;
    auto ptr = throwing::allocate_shared<int>(allocator);
    REQUIRE(ptr);
}
