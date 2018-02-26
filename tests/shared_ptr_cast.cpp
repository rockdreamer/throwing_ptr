//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
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

TEST_CASE("static_pointer_cast to base class", "[shared_ptr][cast]") {
    auto base_ptr = throwing::make_shared<Base>();
    REQUIRE_FALSE(base_ptr->is_derived());
    auto derived_ptr = throwing::make_shared<Derived>();
    REQUIRE(derived_ptr->is_derived());

    // static_pointer_cast to go up class hierarchy
    base_ptr = throwing::static_pointer_cast<Base>(derived_ptr);
    REQUIRE(derived_ptr->is_derived());
}

TEST_CASE("dynamic_pointer_cast to derived class", "[shared_ptr][cast]") {
    auto base_ptr = throwing::make_shared<Base>();
    REQUIRE_FALSE(base_ptr->is_derived());
    auto derived_ptr = throwing::make_shared<Derived>();
    REQUIRE(derived_ptr->is_derived());

    // static_pointer_cast to go up class hierarchy
    base_ptr = throwing::static_pointer_cast<Base>(derived_ptr);

    // dynamic_pointer_cast to go down/across class hierarchy
    auto downcast_ptr = throwing::dynamic_pointer_cast<Derived>(base_ptr);
    REQUIRE(downcast_ptr);
    REQUIRE(downcast_ptr->is_derived());
}

TEST_CASE("const_pointer_cast", "[shared_ptr][cast]") {
    const auto const_ptr = throwing::make_shared<Base>();
    auto non_const_ptr = throwing::const_pointer_cast<Base>(const_ptr);
    REQUIRE(non_const_ptr->mutating());
}

TEST_CASE("reinterpret_pointer_cast", "[shared_ptr][cast]") {
    auto p = throwing::make_shared<Reintepretable>();
    REQUIRE(p);
    auto reinterpreted = throwing::reinterpret_pointer_cast<int>(p);
    REQUIRE(reinterpreted);
    REQUIRE(p.use_count() == 2);
}
