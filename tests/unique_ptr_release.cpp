//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/unique_ptr.hpp>

namespace {
struct Deleter{
    Deleter() : called(false) {}
    void operator()(int *p) {
        called = true;
        delete p;
    }
    bool called;
};
}

TEST_CASE("unique_ptr to single object release",
          "[unique_ptr][release]") {
    int * p = new int;
    throwing::unique_ptr<int, Deleter> uptr(p);
    REQUIRE(uptr.get() == p);
    REQUIRE(uptr.release() == p);
    REQUIRE(uptr.get() == nullptr);
    delete p;
    REQUIRE(!uptr.get_deleter().called);
}

TEST_CASE("unique_ptr to array release",
          "[unique_ptr][release]") {
    int * p = new int[10];
    throwing::unique_ptr<int[], Deleter> uptr(p);
    REQUIRE(uptr.get() == p);
    REQUIRE(uptr.release() == p);
    REQUIRE(uptr.get() == nullptr);
    delete [] p;
    REQUIRE(!uptr.get_deleter().called);
}
