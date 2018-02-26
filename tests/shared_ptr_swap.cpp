//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/shared_ptr.hpp>

TEST_CASE("shared_ptr swap swaps pointers", "[shared_ptr][swap]") {
    int *ptr1 = new int;
    throwing::shared_ptr<int> t_ptr1(ptr1);
    int *ptr2 = new int;
    throwing::shared_ptr<int> t_ptr2(ptr2);
    REQUIRE(t_ptr1.get() == ptr1);
    REQUIRE(t_ptr2.get() == ptr2);
    t_ptr1.swap(t_ptr2);
    REQUIRE(t_ptr1.get() == ptr2);
    REQUIRE(t_ptr2.get() == ptr1);
    std::swap(t_ptr1, t_ptr2);
    REQUIRE(t_ptr1.get() == ptr1);
    REQUIRE(t_ptr2.get() == ptr2);
}

TEST_CASE("shared_ptr swap null pointers", "[shared_ptr][swap][nullptr]") {
    throwing::shared_ptr<int> t_ptr1;
    throwing::shared_ptr<int> t_ptr2;
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() == nullptr);
    t_ptr1.swap(t_ptr2);
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() == nullptr);
    std::swap(t_ptr1, t_ptr2);
    REQUIRE(t_ptr1.get() == nullptr);
    REQUIRE(t_ptr2.get() == nullptr);
}
