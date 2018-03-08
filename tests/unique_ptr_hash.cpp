//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/unique_ptr.hpp>

TEST_CASE("hash operator of unique_ptr", "[unique_ptr][hash]") {
    int *p = new int;
    auto tp = throwing::unique_ptr<int>(p);
    REQUIRE(std::hash<throwing::unique_ptr<int>>()(tp) ==
            std::hash<int *>()(p));
}
