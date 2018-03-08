//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/unique_ptr.hpp>

namespace {
struct ArrayDeleter {
    ArrayDeleter() : called(false) {}
    void operator()(int *p) {
        called = true;
        delete[] p;
    }
    bool called;
};
} // namespace

// This can only compile if the underlying implementation of
// std::unique_ptr allows reset via convertible pointer
TEST_CASE("unique_ptr to array reset to convertible",
          "[unique_ptr][array][reset][conv.qual]") {
    const int *p1 = new int[10];
    throwing::unique_ptr<const int[], ArrayDeleter> uptr(p1);
    REQUIRE(uptr.get());
    REQUIRE(!uptr.get_deleter().called);
    int *p2 = new int[10];
    uptr.reset(p2);
    REQUIRE(uptr.get());
    REQUIRE(uptr.get_deleter().called);
}
