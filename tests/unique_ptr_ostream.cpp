//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <sstream>
#include <throwing/unique_ptr.hpp>

TEST_CASE("unique_ptr operator<< prints stored ptr", "[unique_ptr][streams]") {
    auto ptr = throwing::make_unique<int>();
    std::stringstream ss_ptr;
    ss_ptr << ptr.get();
    std::stringstream ss_tptr;
    ss_tptr << ptr;
    REQUIRE(ss_tptr.str() == ss_ptr.str());
}

TEST_CASE("unique_ptr to array operator<< prints stored ptr",
          "[unique_ptr][array][streams]") {
    auto ptr = throwing::make_unique<int[]>(10);
    std::stringstream ss_ptr;
    ss_ptr << ptr.get();
    std::stringstream ss_tptr;
    ss_tptr << ptr;
    REQUIRE(ss_tptr.str() == ss_ptr.str());
}

TEST_CASE("unique_ptr operator<< prints nullptr",
          "[unique_ptr][streams][nullptr]") {
    throwing::unique_ptr<int> ptr;
    std::stringstream ss_ptr;
    ss_ptr << ptr.get();
    std::stringstream ss_tptr;
    ss_tptr << ptr;
    REQUIRE(ss_tptr.str() == ss_ptr.str());
}
