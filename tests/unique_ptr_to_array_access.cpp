//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/unique_ptr.hpp>

namespace {
struct Foo {
    int foo() const { return 42; }
};
} // namespace

TEST_CASE("unique_ptr to array: get on null returns nullptr",
          "[unique_ptr][array][nullptr][access]") {
    throwing::unique_ptr<int[]> nothing;
    REQUIRE(nothing.get() == nullptr);
    REQUIRE(!nothing);

    throwing::unique_ptr<int[]> nothing_nullptr(nullptr);
    REQUIRE(nothing.get() == nullptr);
    REQUIRE(!nothing);
}

TEST_CASE("unique_ptr to array: get returns first element",
          "[unique_ptr][array][access]") {
    int *ptr = new int[10];
    throwing::unique_ptr<int[]> t_ptr(ptr);
    REQUIRE(t_ptr.get() == ptr);
}

TEST_CASE("unique_ptr to array: dereferencing nullptr throws",
          "[unique_ptr][array][access][nullptr]") {
    throwing::unique_ptr<Foo[]> nothing;
    REQUIRE_THROWS_AS(nothing[0], throwing::base_null_ptr_exception&);
    REQUIRE_THROWS_AS(nothing[0], throwing::null_ptr_exception<Foo>&);
}

TEST_CASE("unique_ptr to array: [0] returns first element",
          "[unique_ptr][array][access]") {
    int *ptr = new int[10];
    throwing::unique_ptr<int[]> t_ptr(ptr);
    REQUIRE(&t_ptr[0] == ptr);
}

TEST_CASE("unique_ptr to array: operator bool", "[unique_ptr][array][bool]") {
    throwing::unique_ptr<int[]> nothing;
    REQUIRE(!nothing);

    throwing::unique_ptr<int[]> something(new int[10]);
    REQUIRE(something);
}
