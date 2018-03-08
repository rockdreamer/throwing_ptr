//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/unique_ptr.hpp>

namespace {
struct Foo {
    int bar = 42;
    int foo() const { return bar; }
};
} // namespace

TEST_CASE("unique_ptr dereference via * throws on nullptr",
          "[unique_ptr][dereference][nullptr]") {
    throwing::unique_ptr<int> nothing;
    REQUIRE_THROWS_AS((*nothing)++, throwing::base_null_ptr_exception);
    REQUIRE_THROWS_AS((*nothing)++, throwing::null_ptr_exception<int>);
}

TEST_CASE("unique_ptr dereference via -> throws on nullptr",
          "[unique_ptr][dereference][nullptr]") {
    throwing::unique_ptr<Foo> nothing;
    REQUIRE_THROWS_AS(nothing->foo(), throwing::base_null_ptr_exception);
    REQUIRE_THROWS_AS(nothing->foo(), throwing::null_ptr_exception<Foo>);
}

TEST_CASE("type specific unique_ptr exceptions are caught by base exception",
          "[unique_ptr][exception]") {
    throwing::unique_ptr<int> nothing;
    try {
        (*nothing)++;
    } catch (const throwing::null_ptr_exception<float> &) {
        FAIL();
    } catch (const throwing::base_null_ptr_exception &e) {
        REQUIRE(std::string(e.what()) == "Dereference of nullptr");
    }
}

TEST_CASE(
        "type specific unique_ptr exceptions are caught by using correct type",
        "[unique_ptr][exception]") {
    throwing::unique_ptr<int> nothing;
    try {
        (*nothing)++;
    } catch (const throwing::null_ptr_exception<float> &) {
        FAIL();
    } catch (const throwing::null_ptr_exception<int> &) {
    }
}

TEST_CASE("unique_ptr exceptions have non-empty what()",
          "[unique_ptr][exception]") {
    throwing::unique_ptr<int> nothing;
    try {
        (*nothing)++;
    } catch (const throwing::base_null_ptr_exception &e) {
        std::string what = e.what_type();
        REQUIRE_FALSE(what.empty());
    }
}
