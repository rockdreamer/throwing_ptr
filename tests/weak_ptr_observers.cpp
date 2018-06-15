//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "test_helpers.h"
#include <catch.hpp>
#include <throwing/shared_ptr.hpp>

TEST_CASE("weak_ptr use_count and expired", "[weak_ptr][use_count]") {
    throwing::weak_ptr<int> wp;
    REQUIRE(wp.use_count() == 0);
    REQUIRE(wp.expired() == true);

    wp = std::make_shared<int>(42);
    REQUIRE(wp.use_count() == 0);
    REQUIRE(wp.expired() == true);

    auto p1 = std::make_shared<int>(42);
    wp = p1;
    REQUIRE(wp.use_count() == 1);
    REQUIRE(wp.expired() == false);

    auto p2 = p1;
    REQUIRE(wp.use_count() == 2);
    REQUIRE(wp.expired() == false);

    p1.reset();
    REQUIRE(wp.use_count() == 1);
    REQUIRE(wp.expired() == false);

    p2.reset();
    REQUIRE(wp.use_count() == 0);
    REQUIRE(wp.expired() == true);
}

TEST_CASE("weak_ptr lock creates throwing_ptr", "[weak_ptr][lock]") {
    throwing::shared_ptr<TestBaseClass> p;
    throwing::weak_ptr<TestBaseClass> wp;

    REQUIRE(wp.lock() == nullptr);
    REQUIRE_THROWS_AS(wp.lock()->dummy(), throwing::base_null_ptr_exception&);

    p = throwing::make_shared<TestBaseClass>();
    wp = p;
    wp.lock()->dummy(); // no exception

    p.reset();
    REQUIRE_THROWS_AS(wp.lock()->dummy(), throwing::base_null_ptr_exception&);
}

TEST_CASE("weak_ptr owner_before behaves as std::weak_ptr",
          "[weak_ptr][owner_before]") {
    auto sp1 = std::make_shared<MemoryPositionHelper>(1, 2);
    std::shared_ptr<int> sp2(sp1, &sp1->m1);
    std::shared_ptr<int> sp3(sp1, &sp1->m2);
    std::weak_ptr<int> sw2(sp2);
    std::weak_ptr<int> sw3(sp3);

    auto tsp1 = throwing::make_shared<MemoryPositionHelper>(1, 2);
    throwing::shared_ptr<int> tsp2(tsp1, &tsp1->m1);
    throwing::shared_ptr<int> tsp3(tsp1, &tsp1->m2);
    throwing::weak_ptr<int> tw2(tsp2);
    throwing::weak_ptr<int> tw3(tsp3);

    REQUIRE(sw2.owner_before(sw3) == tw2.owner_before(tw3));
    REQUIRE(sw3.owner_before(sw2) == tw3.owner_before(tw2));
}
