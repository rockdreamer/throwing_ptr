//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/shared_ptr.hpp>

namespace {
struct Foo {
    int foo() const { return 42; }
};
} // namespace

TEST_CASE("shared_ptr to array from nullptr: get returns nullptr",
          "[shared_ptr][array][access]") {
    throwing::shared_ptr<int[10]> nothing;
    REQUIRE(nothing.get() == nullptr);

    throwing::shared_ptr<int[10]> nothing_nullptr(nullptr);
    REQUIRE(nothing.get() == nullptr);

    throwing::shared_ptr<int[10]> nothing_null(NULL);
    REQUIRE(nothing.get() == nullptr);
}

TEST_CASE("shared_ptr to array: get returns first element",
          "[shared_ptr][array][access]") {
    int *ptr = new int[10];
    throwing::shared_ptr<int[10]> t_ptr(ptr);
    REQUIRE(t_ptr.get() == ptr);
}

TEST_CASE("dereferencing null shared_ptr to array throws",
          "[shared_ptr][array][access]") {
    throwing::shared_ptr<Foo[100]> nothing;
    REQUIRE_THROWS_AS(nothing[0], throwing::base_null_ptr_exception&);
    REQUIRE_THROWS_AS(nothing[0], throwing::null_ptr_exception<Foo[100]>&);
}

TEST_CASE("shared_ptr to array: [0] returns first element",
          "[shared_ptr][array][access]") {
    int *ptr = new int[10];
    throwing::shared_ptr<int[10]> t_ptr(ptr);
    REQUIRE(&t_ptr[0] == ptr);
}
