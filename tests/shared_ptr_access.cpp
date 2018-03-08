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

TEST_CASE("shared_ptr get returns correctly with nullptr",
          "[shared_ptr][access][nullptr]") {
    throwing::shared_ptr<int> nothing;
    REQUIRE(nothing.get() == nullptr);

    throwing::shared_ptr<int> nothing_nullptr(nullptr);
    REQUIRE(nothing.get() == nullptr);

    throwing::shared_ptr<int> nothing_null(NULL);
    REQUIRE(nothing.get() == nullptr);
}

TEST_CASE("shared_ptr get returns correct address", "[shared_ptr][access]") {
    int *ptr = new int;
    throwing::shared_ptr<int> t_ptr(ptr);
    REQUIRE(t_ptr.get() == ptr);
}

TEST_CASE("shared_ptr dereference via * throws on nullptr",
          "[shared_ptr][dereference][nullptr]") {
    throwing::shared_ptr<int> nothing;
    REQUIRE_THROWS_AS((*nothing)++, throwing::base_null_ptr_exception);
    REQUIRE_THROWS_AS((*nothing)++, throwing::null_ptr_exception<int>);
}

TEST_CASE("shared_ptr dereference via -> throws on nullptr",
          "[shared_ptr][dereference][nullptr]") {
    throwing::shared_ptr<Foo> nothing;
    REQUIRE_THROWS_AS(nothing->foo(), throwing::base_null_ptr_exception);
    REQUIRE_THROWS_AS(nothing->foo(), throwing::null_ptr_exception<Foo>);
}

TEST_CASE("type specific shared_ptr exceptions are caught by base exception",
          "[shared_ptr][exception]") {
    throwing::shared_ptr<int> nothing;
    try {
        (*nothing)++;
    } catch (const throwing::null_ptr_exception<float> &) {
        FAIL();
    } catch (const throwing::base_null_ptr_exception &e) {
        REQUIRE(std::string(e.what()) == "Dereference of nullptr");
    }
}

TEST_CASE(
        "type specific shared_ptr exceptions are caught by using correct type",
        "[shared_ptr][exception]") {
    throwing::shared_ptr<int> nothing;
    try {
        (*nothing)++;
    } catch (const throwing::null_ptr_exception<float> &) {
        FAIL();
    } catch (const throwing::null_ptr_exception<int> &) {
    }
}

TEST_CASE("shared_ptr exceptions have non-empty what()",
          "[shared_ptr][exception]") {
    throwing::shared_ptr<int> nothing;
    try {
        (*nothing)++;
    } catch (const throwing::base_null_ptr_exception &e) {
        std::string what = e.what_type();
        REQUIRE_FALSE(what.empty());
    }
}

TEST_CASE("shared_ptr use count works", "[shared_ptr][use count]") {
    Foo *foo = new Foo;
    throwing::shared_ptr<Foo> ptr;
    REQUIRE(ptr.use_count() == 0l);
    ptr.reset(foo);
    REQUIRE(ptr.use_count() == 1l);
    auto ptr2 = ptr;
    REQUIRE(ptr.use_count() == 2l);
    REQUIRE(ptr2.use_count() == 2l);
    ptr.reset();
    REQUIRE(ptr.use_count() == 0l);
    REQUIRE(ptr2.use_count() == 1l);
    ptr2.reset();
    REQUIRE(ptr.use_count() == 0l);
    REQUIRE(ptr2.use_count() == 0l);
}

TEST_CASE("operator bool works", "[shared_ptr]") {
    Foo *foo = new Foo;
    throwing::shared_ptr<Foo> ptr;
    REQUIRE_FALSE(ptr);
    ptr.reset(foo);
    REQUIRE(ptr);
    ptr.reset();
    REQUIRE_FALSE(ptr);
}
