//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/unique_ptr.hpp>

TEST_CASE("unique_ptr to nullptr get returns nullptr",
          "[unique_ptr][nullptr]") {
    throwing::unique_ptr<int> nothing;
    REQUIRE(nothing.get() == nullptr);
    REQUIRE(!nothing);

    throwing::unique_ptr<int> nothing2(nullptr);
    REQUIRE(nothing2.get() == nullptr);
    REQUIRE(!nothing2);
}

TEST_CASE("unique_ptr: operator bool", "[unique_ptr][bool]") {
    throwing::unique_ptr<int> nothing;
    REQUIRE(!nothing);

    throwing::unique_ptr<int> something(new int);
    REQUIRE(something);
}
