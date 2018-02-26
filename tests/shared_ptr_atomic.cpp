//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/shared_ptr.hpp>

TEST_CASE("atomic_is_lock_free std::shared_ptr compatibility",
          "[shared_ptr][atomic]") {
    const auto std_p = std::make_shared<int>(42);
    const auto p = throwing::make_shared<int>(42);
    REQUIRE(atomic_is_lock_free(&p) == atomic_is_lock_free(&std_p));
}

TEST_CASE("atomic_load works in the same thread", "[shared_ptr][atomic]") {
    const auto p = throwing::make_shared<int>(42);
    auto p2 = atomic_load(&p);
    REQUIRE(*p2 == 42);
}

TEST_CASE("atomic_load_explicit works in the same thread",
          "[shared_ptr][atomic]") {
    const auto p = throwing::make_shared<int>(42);
    auto p2 = atomic_load_explicit(&p, std::memory_order_seq_cst);
    REQUIRE(*p2 == 42);
}

TEST_CASE("atomic_store works in the same thread", "[shared_ptr][atomic]") {
    const auto p1 = throwing::make_shared<int>(1);
    auto p2 = throwing::make_shared<int>(2);

    atomic_store(&p2, p1);
    REQUIRE(*p2 == 1);
}

TEST_CASE("atomic_store_explicit works in the same thread",
          "[shared_ptr][atomic]") {
    const auto p1 = throwing::make_shared<int>(1);
    auto p2 = throwing::make_shared<int>(2);

    atomic_store_explicit(&p2, p1, std::memory_order_seq_cst);
    REQUIRE(*p2 == 1);
}

TEST_CASE("atomic_exchange works in the same thread", "[shared_ptr][atomic]") {
    const auto p1 = throwing::make_shared<int>(1);
    auto p2 = throwing::make_shared<int>(2);
    auto p3 = atomic_exchange(&p2, p1);
    REQUIRE(*p2 == 1);
    REQUIRE(*p3 == 2);
}

TEST_CASE("atomic_exchange_explicit works in the same thread",
          "[shared_ptr][atomic]") {
    const auto p1 = throwing::make_shared<int>(1);
    auto p2 = throwing::make_shared<int>(2);
    auto p3 = atomic_exchange_explicit(&p2, p1, std::memory_order_seq_cst);
    REQUIRE(*p2 == 1);
    REQUIRE(*p3 == 2);
}

TEST_CASE("atomic_compare_exchange_weak works in the same thread",
          "[shared_ptr][atomic]") {
    auto p = throwing::make_shared<int>(1);
    auto expected = p;
    auto desired = throwing::make_shared<int>(2);

    REQUIRE(atomic_compare_exchange_weak(&p, &expected, desired));
    REQUIRE(*p == 2);
}

TEST_CASE("atomic_compare_exchange_weak works in the same thread, compares "
          "pointer, not value",
          "[shared_ptr][atomic]") {
    auto p = throwing::make_shared<int>(1);
    auto expected = throwing::make_shared<int>(1);
    auto desired = throwing::make_shared<int>(3);

    REQUIRE_FALSE(atomic_compare_exchange_weak(&p, &expected, desired));
    REQUIRE(*expected == 1);
}

TEST_CASE("atomic_compare_exchange_strong works in the same thread",
          "[shared_ptr][atomic]") {
    auto p = throwing::make_shared<int>(1);
    auto expected = p;
    auto desired = throwing::make_shared<int>(2);

    REQUIRE(atomic_compare_exchange_strong(&p, &expected, desired));
    REQUIRE(*p == 2);
}

TEST_CASE("atomic_compare_exchange_strong works in the same thread, compares "
          "pointer, not value",
          "[shared_ptr][atomic]") {
    auto p = throwing::make_shared<int>(1);
    auto expected = throwing::make_shared<int>(1);
    auto desired = throwing::make_shared<int>(3);

    REQUIRE_FALSE(atomic_compare_exchange_strong(&p, &expected, desired));
    REQUIRE(*expected == 1);
}

TEST_CASE("atomic_compare_exchange_weak_explicit works in the same thread",
          "[shared_ptr][atomic]") {
    auto p = throwing::make_shared<int>(1);
    auto expected = p;
    auto desired = throwing::make_shared<int>(2);

    REQUIRE(atomic_compare_exchange_weak_explicit(&p, &expected, desired,
                                                  std::memory_order_seq_cst,
                                                  std::memory_order_seq_cst));
    REQUIRE(*p == 2);
}

TEST_CASE("atomic_compare_exchange_strong_explicit works in the same thread, "
          "compares pointer, not value",
          "[shared_ptr][atomic]") {
    auto p = throwing::make_shared<int>(1);
    auto expected = throwing::make_shared<int>(1);
    auto desired = throwing::make_shared<int>(3);

    REQUIRE_FALSE(atomic_compare_exchange_strong_explicit(
            &p, &expected, desired, std::memory_order_seq_cst,
            std::memory_order_seq_cst));
    REQUIRE(*expected == 1);
}
