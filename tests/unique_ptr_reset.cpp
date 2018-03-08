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
struct ArrayDeleter{
    ArrayDeleter() : called(false) {}
    void operator()(int *p) {
        called = true;
        delete [] p;
    }
    bool called;
};
}

TEST_CASE("unique_ptr to single object reset",
          "[unique_ptr][reset]") {
    throwing::unique_ptr<int, Deleter> uptr(new int);
    REQUIRE(uptr.get());
    REQUIRE(!uptr.get_deleter().called);
    uptr.reset(new int);
    REQUIRE(uptr.get());
    REQUIRE(uptr.get_deleter().called);
}

TEST_CASE("unique_ptr to single object reset with nullptr",
          "[unique_ptr][reset][nullptr]") {
    throwing::unique_ptr<int, Deleter> uptr(new int);
    REQUIRE(uptr.get());
    REQUIRE(!uptr.get_deleter().called);
    uptr.reset(nullptr);
    REQUIRE(!uptr.get());
    REQUIRE(uptr.get_deleter().called);
}

TEST_CASE("unique_ptr to array reset",
          "[unique_ptr][array][reset]") {
    throwing::unique_ptr<int[], ArrayDeleter> uptr(new int[10]);
    REQUIRE(uptr.get());
    REQUIRE(!uptr.get_deleter().called);
    uptr.reset(new int[10]);
    REQUIRE(uptr.get());
    REQUIRE(uptr.get_deleter().called);
}

TEST_CASE("unique_ptr to array reset with nullptr",
          "[unique_ptr][reset][nullptr]") {
    throwing::unique_ptr<int[], ArrayDeleter> uptr(new int[10]);
    REQUIRE(uptr.get());
    REQUIRE(!uptr.get_deleter().called);
    uptr.reset(nullptr);
    REQUIRE(!uptr.get());
    REQUIRE(uptr.get_deleter().called);
}
