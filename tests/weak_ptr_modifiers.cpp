//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "test_helpers.h"
#include <catch.hpp>
#include <throwing/weak_ptr.hpp>

TEST_CASE("weak_ptr reset", "[weak_ptr][reset]") {
    auto p = std::make_shared<int>(42);
    throwing::weak_ptr<int> wp(p);
    REQUIRE(wp.use_count() == 1);
    REQUIRE(wp.expired() == false);
    REQUIRE(wp.lock() == p);
    wp.reset();
    REQUIRE(wp.lock() == nullptr);
}

TEST_CASE("weak_ptr swap", "[weak_ptr][swap]") {
    auto p1 = std::make_shared<int>(41);
    auto p2 = std::make_shared<int>(42);
    throwing::weak_ptr<int> wp1(p1);
    throwing::weak_ptr<int> wp2(p2);
    REQUIRE(wp1.lock() == p1);
    REQUIRE(wp2.lock() == p2);
    wp1.swap(wp2);
    REQUIRE(wp1.lock() == p2);
    REQUIRE(wp2.lock() == p1);
    wp2.swap(wp1);
    REQUIRE(wp1.lock() == p1);
    REQUIRE(wp2.lock() == p2);
}

TEST_CASE("weak_ptr std::swap", "[weak_ptr][swap]") {
    auto p1 = std::make_shared<int>(41);
    auto p2 = std::make_shared<int>(42);
    throwing::weak_ptr<int> wp1(p1);
    throwing::weak_ptr<int> wp2(p2);
    REQUIRE(wp1.lock() == p1);
    REQUIRE(wp2.lock() == p2);
    std::swap(wp1, wp2);
    REQUIRE(wp1.lock() == p2);
    REQUIRE(wp2.lock() == p1);
    std::swap(wp1, wp2);
    REQUIRE(wp1.lock() == p1);
    REQUIRE(wp2.lock() == p2);
}
