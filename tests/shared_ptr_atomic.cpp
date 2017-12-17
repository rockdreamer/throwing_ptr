//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <throwing/shared_ptr.hpp>

TEST(SharedPtr, IsLockFree) {
    const auto std_p = std::make_shared<int>(42);
    const auto p = throwing::make_shared<int>(42);
    EXPECT_EQ(atomic_is_lock_free(&std_p), atomic_is_lock_free(&p));
}

TEST(SharedPtr, AtomicLoad) {
    const auto p = throwing::make_shared<int>(42);
    auto p2 = atomic_load(&p);
    EXPECT_EQ(42, *p2);
}

TEST(SharedPtr, AtomicLoadExplicit) {
    const auto p = throwing::make_shared<int>(42);
    auto p2 = atomic_load_explicit(&p, std::memory_order_seq_cst);
    EXPECT_EQ(42, *p2);
}

TEST(SharedPtr, AtomicStore) {
    const auto p1 = throwing::make_shared<int>(1);
    auto p2 = throwing::make_shared<int>(2);

    atomic_store(&p2, p1);
    EXPECT_EQ(1, *p2);
}

TEST(SharedPtr, AtomicStoreExcplicit) {
    const auto p1 = throwing::make_shared<int>(1);
    auto p2 = throwing::make_shared<int>(2);

    atomic_store_explicit(&p2, p1, std::memory_order_seq_cst);
    EXPECT_EQ(1, *p2);
}

TEST(SharedPtr, AtomicExchange) {
    const auto p1 = throwing::make_shared<int>(1);
    auto p2 = throwing::make_shared<int>(2);
    auto p3 = atomic_exchange(&p2, p1);
    EXPECT_EQ(1, *p2);
    EXPECT_EQ(2, *p3);
}

TEST(SharedPtr, AtomicExchangeExplicit) {
    const auto p1 = throwing::make_shared<int>(1);
    auto p2 = throwing::make_shared<int>(2);
    auto p3 = atomic_exchange_explicit(&p2, p1, std::memory_order_seq_cst);
    EXPECT_EQ(1, *p2);
    EXPECT_EQ(2, *p3);
}

TEST(SharedPtr, AtomicCompareExchangeWeak) {
    auto p = throwing::make_shared<int>(1);
    auto expected = p;
    auto desired = throwing::make_shared<int>(2);

    EXPECT_TRUE(atomic_compare_exchange_weak(&p, &expected, desired));
    EXPECT_EQ(2, *p);
}

TEST(SharedPtr, AtomicCompareExchangeWeakNotExpected) {
    auto p = throwing::make_shared<int>(1);
    auto expected = throwing::make_shared<int>(1);
    auto desired = throwing::make_shared<int>(3);

    EXPECT_FALSE(atomic_compare_exchange_weak(&p, &expected, desired));
    EXPECT_EQ(1, *expected);
}

TEST(SharedPtr, AtomicCompareExchangeStrongSuccess) {
    auto p = throwing::make_shared<int>(1);
    auto expected = p;
    auto desired = throwing::make_shared<int>(2);

    EXPECT_TRUE(atomic_compare_exchange_strong(&p, &expected, desired));
    EXPECT_EQ(2, *p);
}

TEST(SharedPtr, AtomicCompareExchangeStrongFailure) {
    auto p = throwing::make_shared<int>(1);
    auto expected = throwing::make_shared<int>(1);
    auto desired = throwing::make_shared<int>(3);

    EXPECT_FALSE(atomic_compare_exchange_strong(&p, &expected, desired));
    EXPECT_EQ(1, *expected);
}

TEST(SharedPtr, AtomicCompareExchangeWeakExplicit) {
    auto p = throwing::make_shared<int>(1);
    auto expected = p;
    auto desired = throwing::make_shared<int>(2);

    EXPECT_TRUE(atomic_compare_exchange_weak_explicit(
            &p, &expected, desired, std::memory_order_seq_cst,
            std::memory_order_seq_cst));
    EXPECT_EQ(2, *p);
}

TEST(SharedPtr, AtomicCompareExchangeStrongExplicit) {
    auto p = throwing::make_shared<int>(1);
    auto expected = throwing::make_shared<int>(1);
    auto desired = throwing::make_shared<int>(3);

    EXPECT_FALSE(atomic_compare_exchange_strong_explicit(
            &p, &expected, desired, std::memory_order_seq_cst,
            std::memory_order_seq_cst));
    EXPECT_EQ(1, *expected);
}
