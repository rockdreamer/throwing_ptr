//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <sstream>
#include <throwing/shared_ptr.hpp>

TEST_CASE("operator<< prints stored ptr", "[shared_ptr][streams]") {
    auto ptr = throwing::make_shared<int>();
    std::stringstream ss_ptr;
    ss_ptr << ptr.get();
    std::stringstream ss_tptr;
    ss_tptr << ptr;
    REQUIRE(ss_tptr.str() == ss_ptr.str());
}

TEST_CASE("operator<< prints nullptr", "[shared_ptr][streams]") {
    throwing::shared_ptr<int> ptr;
    std::stringstream ss_ptr;
    ss_ptr << ptr.get();
    std::stringstream ss_tptr;
    ss_tptr << ptr;
    REQUIRE(ss_tptr.str() == ss_ptr.str());
}
