//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <functional>
#include <throwing/shared_ptr.hpp>

TEST_CASE("comparison operators between valid throwing::shared_ptr",
          "[shared_ptr][comparison]") {
    int *ptr1 = new int;
    int *ptr2 = new int;
    auto t_ptr1 = throwing::shared_ptr<int>(ptr1);
    auto t_ptr1_1 = t_ptr1;
    auto t_ptr2 = throwing::shared_ptr<int>(ptr2);

    REQUIRE_FALSE(t_ptr1 == t_ptr2);
    REQUIRE_FALSE(t_ptr1_1 == t_ptr2);
    REQUIRE(t_ptr1 == t_ptr1);
    REQUIRE(t_ptr1 == t_ptr1_1);

    REQUIRE(t_ptr1 != t_ptr2);
    REQUIRE(t_ptr1_1 != t_ptr2);
    REQUIRE_FALSE(t_ptr1 != t_ptr1);
    REQUIRE_FALSE(t_ptr1 != t_ptr1_1);

    REQUIRE((t_ptr1 < t_ptr2) == (ptr1 < ptr2));
    REQUIRE((t_ptr1_1 < t_ptr2) == (ptr1 < ptr2));
    REQUIRE_FALSE(t_ptr1 < t_ptr1);
    REQUIRE_FALSE(t_ptr1 < t_ptr1_1);

    REQUIRE((t_ptr1 > t_ptr2) == (ptr1 > ptr2));
    REQUIRE((t_ptr1_1 > t_ptr2) == (ptr1 > ptr2));
    REQUIRE_FALSE(t_ptr1 > t_ptr1);
    REQUIRE_FALSE(t_ptr1 > t_ptr1_1);

    REQUIRE((t_ptr1 <= t_ptr2) == (ptr1 <= ptr2));
    REQUIRE((t_ptr1_1 <= t_ptr2) == (ptr1 <= ptr2));
    REQUIRE((t_ptr1 <= t_ptr1) == (ptr1 <= ptr1));
    REQUIRE((t_ptr1 <= t_ptr1_1) == (ptr1 <= ptr1));

    REQUIRE((t_ptr1 >= t_ptr2) == (ptr1 >= ptr2));
    REQUIRE((t_ptr1_1 >= t_ptr2) == (ptr1 >= ptr2));
    REQUIRE((t_ptr1 >= t_ptr1) == (ptr1 >= ptr1));
    REQUIRE((t_ptr1 >= t_ptr1_1) == (ptr1 >= ptr1));
}

TEST_CASE("comparison operators with null throwing::shared_ptr",
          "[shared_ptr][comparison]") {
    int *ptr = new int;
    auto t_ptr = throwing::shared_ptr<int>(ptr);
    auto empty_t_ptr = throwing::shared_ptr<int>();

    REQUIRE(empty_t_ptr == nullptr);
    REQUIRE_FALSE(t_ptr == nullptr);
    REQUIRE(nullptr == empty_t_ptr);
    REQUIRE_FALSE(nullptr == t_ptr);

    REQUIRE_FALSE(empty_t_ptr != nullptr);
    REQUIRE(t_ptr != nullptr);
    REQUIRE_FALSE(nullptr != empty_t_ptr);
    REQUIRE(nullptr != t_ptr);

    REQUIRE((t_ptr < nullptr) == std::less<int *>()(ptr, nullptr));
    REQUIRE((empty_t_ptr < nullptr) == std::less<int *>()(nullptr, nullptr));
    REQUIRE((nullptr < t_ptr) == std::less<int *>()(nullptr, ptr));
    REQUIRE((nullptr < empty_t_ptr) == std::less<int *>()(nullptr, nullptr));

    REQUIRE((t_ptr > nullptr) == std::less<int *>()(nullptr, ptr));
    REQUIRE((empty_t_ptr > nullptr) == std::less<int *>()(nullptr, nullptr));
    REQUIRE((nullptr > t_ptr) == std::less<int *>()(ptr, nullptr));
    REQUIRE((nullptr > empty_t_ptr) == std::less<int *>()(nullptr, nullptr));

    REQUIRE((t_ptr <= nullptr) == !std::less<int *>()(nullptr, ptr));
    REQUIRE((empty_t_ptr <= nullptr) == !std::less<int *>()(nullptr, nullptr));
    REQUIRE((nullptr <= t_ptr) == !std::less<int *>()(ptr, nullptr));
    REQUIRE((nullptr <= empty_t_ptr) == !std::less<int *>()(nullptr, nullptr));

    REQUIRE((t_ptr >= nullptr) == !std::less<int *>()(ptr, nullptr));
    REQUIRE((empty_t_ptr >= nullptr) == !std::less<int *>()(nullptr, nullptr));
    REQUIRE((nullptr >= t_ptr) == !std::less<int *>()(nullptr, ptr));
    REQUIRE((nullptr >= empty_t_ptr) == !std::less<int *>()(nullptr, nullptr));
}
