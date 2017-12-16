//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <throwing/shared_ptr.hpp>

namespace {
class Base {
public:
    virtual ~Base() = default;
    virtual bool mutating() { return true; }
    virtual bool is_derived() const { return false; }
};

class Derived : public Base {
public:
    virtual ~Derived() = default;
    virtual bool is_derived() const { return true; }
};

struct Reintepretable {
    int a;
};

} // namespace

TEST(Casts, StaticPointerCast) {
    auto base_ptr = throwing::make_shared<Base>();
    EXPECT_FALSE(base_ptr->is_derived());
    auto derived_ptr = throwing::make_shared<Derived>();
    EXPECT_TRUE(derived_ptr->is_derived());

    // static_pointer_cast to go up class hierarchy
    base_ptr = throwing::static_pointer_cast<Base>(derived_ptr);
    EXPECT_TRUE(derived_ptr->is_derived());
}

TEST(Casts, DynamicPointerCast) {
    auto base_ptr = throwing::make_shared<Base>();
    EXPECT_FALSE(base_ptr->is_derived());
    auto derived_ptr = throwing::make_shared<Derived>();
    EXPECT_TRUE(derived_ptr->is_derived());

    // static_pointer_cast to go up class hierarchy
    base_ptr = throwing::static_pointer_cast<Base>(derived_ptr);

    // dynamic_pointer_cast to go down/across class hierarchy
    auto downcast_ptr = throwing::dynamic_pointer_cast<Derived>(base_ptr);
    EXPECT_TRUE(downcast_ptr);
    EXPECT_TRUE(downcast_ptr->is_derived());
}

TEST(Casts, ConstPointerCast) {
    const auto const_ptr = throwing::make_shared<Base>();
    auto non_const_ptr = throwing::const_pointer_cast<Base>(const_ptr);
    EXPECT_TRUE(non_const_ptr->mutating());
}

TEST(Casts, ReinterpretPointerCast) {
    auto p = throwing::make_shared<Reintepretable>();
    EXPECT_TRUE(p);
    auto reinterpreted = throwing::reinterpret_pointer_cast<int>(p);
    EXPECT_TRUE(reinterpreted);
    EXPECT_EQ(2, p.use_count());
}
