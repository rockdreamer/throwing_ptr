//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/shared_ptr.hpp>

struct Good: throwing::enable_shared_from_this<Good>
{
    throwing::shared_ptr<Good> getptr() {
        return shared_from_this();
    }
};

TEST_CASE("enable_shared_from_this from make_shared",
          "[shared_ptr][enable_shared_from_this]") {
    throwing::shared_ptr<Good> gp1 = throwing::make_shared<Good>();
    throwing::shared_ptr<Good> gp2 = gp1->getptr();
    REQUIRE(gp2.use_count() == 2);
    REQUIRE(gp1.get() == gp2.get());
}

TEST_CASE("enable_shared_from_this from constructor",
          "[shared_ptr][enable_shared_from_this]") {
    throwing::shared_ptr<Good> gp1 = throwing::shared_ptr<Good>(new Good);
    throwing::shared_ptr<Good> gp2 = gp1->getptr();
    REQUIRE(gp2.use_count() == 2);
    REQUIRE(gp1.get() == gp2.get());
}
