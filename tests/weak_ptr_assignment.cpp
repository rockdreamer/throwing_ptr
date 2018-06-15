//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "test_helpers.h"
#include <catch.hpp>
#include <throwing/shared_ptr.hpp>

TEST_CASE("weak_ptr assignment from std::shared_ptr",
          "[weak_ptr][assignment]") {
    auto p = std::make_shared<int>(42);
    throwing::weak_ptr<int> wp;
    REQUIRE(wp.lock() == nullptr);
    wp = p;
    REQUIRE(wp.lock() == p);
}

TEST_CASE("weak_ptr assignment from std::shared_ptr to derived class",
          "[weak_ptr][assignment]") {
    auto p = std::make_shared<TestDerivedClass>();
    throwing::weak_ptr<TestBaseClass> wp;
    REQUIRE(wp.lock() == nullptr);
    wp = p;
    REQUIRE(wp.lock() == p);
}

TEST_CASE("weak_ptr assignment from throwing::shared_ptr",
          "[weak_ptr][assignment]") {
    auto p = throwing::make_shared<int>(42);
    throwing::weak_ptr<int> wp;
    REQUIRE(wp.lock() == nullptr);
    wp = p;
    REQUIRE(wp.lock() == p);
}

TEST_CASE("weak_ptr assignment from throwing::shared_ptr to derived class",
          "[weak_ptr][assignment]") {
    auto p = throwing::make_shared<TestDerivedClass>();
    throwing::weak_ptr<TestBaseClass> wp;
    REQUIRE(wp.lock() == nullptr);
    wp = p;
    REQUIRE(wp.lock() == p);
}

TEST_CASE("weak_ptr assignment from weak_ptr", "[weak_ptr][assignment]") {
    auto p = throwing::make_shared<int>(42);
    throwing::weak_ptr<int> wp(p);
    throwing::weak_ptr<int> wp2;
    REQUIRE(wp2.lock() == nullptr);
    wp2 = wp;
    REQUIRE(wp2.lock() == p);
}

TEST_CASE("weak_ptr assignment from derived class weak_ptr",
          "[weak_ptr][assignment]") {
    auto p = throwing::make_shared<TestDerivedClass>();
    throwing::weak_ptr<TestDerivedClass> wp(p);
    throwing::weak_ptr<TestBaseClass> wp2;
    REQUIRE(wp2.lock() == nullptr);
    wp2 = wp;
    REQUIRE(wp2.lock() == p);
}

TEST_CASE("weak_ptr move assignment", "[weak_ptr][assignment]") {
    auto p = throwing::make_shared<int>(42);
    throwing::weak_ptr<int> wp(p);
    throwing::weak_ptr<int> wp2;
    REQUIRE(wp2.lock() == nullptr);
    wp2 = std::move(wp);
    REQUIRE(wp2.lock() == p);
}

TEST_CASE("weak_ptr move assignment from derived class weak_ptr",
          "[weak_ptr][assignment]") {
    auto p = throwing::make_shared<TestDerivedClass>();
    throwing::weak_ptr<TestDerivedClass> wp(p);
    throwing::weak_ptr<TestBaseClass> wp2;
    REQUIRE(wp2.lock() == nullptr);
    wp2 = std::move(wp);
    REQUIRE(wp2.lock() == p);
}
