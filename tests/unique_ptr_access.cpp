//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/unique_ptr.hpp>

TEST_CASE("unique_ptr to nullptr get returns nullptr",
          "[unique_ptr][nullptr]") {
    throwing::unique_ptr<int> nothing;
    REQUIRE(nothing.get() == nullptr);

    throwing::unique_ptr<int> nothing2(nullptr);
    REQUIRE(nothing2.get() == nullptr);
}
