//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/shared_ptr.hpp>

TEST_CASE("hash operator of shared_ptr", "[shared_ptr][hash]") {
    auto p = std::make_shared<int>(42);
    throwing::shared_ptr<int> tp = p;
    REQUIRE(std::hash<throwing::shared_ptr<int>>()(tp) ==
            std::hash<std::shared_ptr<int>>()(p));
}
