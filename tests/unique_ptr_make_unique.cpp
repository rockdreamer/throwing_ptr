//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/unique_ptr.hpp>

namespace {
struct Foo {
    int n1;
    int n2;
    Foo(int a, int b) : n1(a), n2(b) {}
    Foo() : n1(42), n2(84) {}
};
} // namespace

TEST_CASE("make_unique struct with arguments", "[unique_ptr][make_unique]") {
    auto ptr = throwing::make_unique<Foo>(1, 2);
    REQUIRE(ptr->n1 == 1);
    REQUIRE(ptr->n2 == 2);
}

TEST_CASE("make_unique struct with no arguments", "[unique_ptr][make_unique]") {
    auto ptr = throwing::make_unique<Foo>();
    REQUIRE(ptr->n1 == 42);
    REQUIRE(ptr->n2 == 84);
}

TEST_CASE("make_unique base type with argument", "[unique_ptr][make_unique]") {
    auto ptr = throwing::make_unique<int>(42);
    REQUIRE(*ptr == 42);
}

TEST_CASE("make_unique base type no arguments", "[unique_ptr][make_unique]") {
    auto ptr = throwing::make_unique<int>();
    REQUIRE(ptr);
}

TEST_CASE("make_unique array of struct", "[unique_ptr][make_unique][array]") {
    auto ptr = throwing::make_unique<Foo[]>(10);
    REQUIRE(ptr[0].n1 == 42);
    REQUIRE(ptr[0].n2 == 84);
    REQUIRE(ptr[9].n1 == 42);
    REQUIRE(ptr[9].n2 == 84);
}

TEST_CASE("make_unique array of base type",
          "[unique_ptr][make_unique][array]") {
    auto ptr = throwing::make_unique<int[]>(10);
    REQUIRE(ptr);
}
