//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/unique_ptr.hpp>

TEST_CASE("unique_ptr swap swaps pointers", "[unique_ptr][swap]") {
    int *ptr1 = new int;
    throwing::unique_ptr<int> t_ptr1(ptr1);
    int *ptr2 = new int;
    throwing::unique_ptr<int> t_ptr2(ptr2);
    REQUIRE(t_ptr1.get() == ptr1);
    REQUIRE(t_ptr2.get() == ptr2);
    t_ptr1.swap(t_ptr2);
    REQUIRE(t_ptr1.get() == ptr2);
    REQUIRE(t_ptr2.get() == ptr1);
    std::swap(t_ptr1, t_ptr2);
    REQUIRE(t_ptr1.get() == ptr1);
    REQUIRE(t_ptr2.get() == ptr2);
}

TEST_CASE("unique_ptr swap null pointers", "[unique_ptr][swap][nullptr]") {
    throwing::unique_ptr<int> t_ptr1(new int);
    throwing::unique_ptr<int> t_ptr2;
    REQUIRE(t_ptr1.get() != nullptr);
    REQUIRE(t_ptr2.get() == nullptr);
    t_ptr1.swap(t_ptr2);
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() != nullptr);
    std::swap(t_ptr1, t_ptr2);
    REQUIRE(t_ptr1.get() != nullptr);
    REQUIRE(t_ptr2.get() == nullptr);
}

TEST_CASE("unique_ptr to array swap swaps pointers",
          "[unique_ptr][array][swap]") {
    int *ptr1 = new int;
    throwing::unique_ptr<int> t_ptr1(ptr1);
    int *ptr2 = new int;
    throwing::unique_ptr<int> t_ptr2(ptr2);
    REQUIRE(t_ptr1.get() == ptr1);
    REQUIRE(t_ptr2.get() == ptr2);
    t_ptr1.swap(t_ptr2);
    REQUIRE(t_ptr1.get() == ptr2);
    REQUIRE(t_ptr2.get() == ptr1);
    std::swap(t_ptr1, t_ptr2);
    REQUIRE(t_ptr1.get() == ptr1);
    REQUIRE(t_ptr2.get() == ptr2);
}

TEST_CASE("unique_ptr to array swap null pointers",
          "[unique_ptr][array][swap][nullptr]") {
    throwing::unique_ptr<int[]> t_ptr1(new int[10]);
    throwing::unique_ptr<int[]> t_ptr2;
    REQUIRE(t_ptr1.get() != nullptr);
    REQUIRE(t_ptr2.get() == nullptr);
    t_ptr1.swap(t_ptr2);
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() != nullptr);
    std::swap(t_ptr1, t_ptr2);
    REQUIRE(t_ptr1.get() != nullptr);
    REQUIRE(t_ptr2.get() == nullptr);
}
