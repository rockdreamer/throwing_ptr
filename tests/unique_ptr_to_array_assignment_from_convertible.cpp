//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/unique_ptr.hpp>

namespace {
class A {
    int dummy_a;

public:
    int dummy() { return dummy_a; }
};

struct DeleterA {
    DeleterA() : called(false) {}
    void operator()(A *p) {
        called = true;
        delete p;
    }
    bool called;
};
} // namespace

TEST_CASE("move assignment from throwing::unique_ptr to array of convertible "
          "type",
          "[unique_ptr][assignment][array][conv.qual]") {
    const A* p1 = new A[10];
    throwing::unique_ptr<const A[]> t_ptr1(p1);
    A* p2 = new A[10];
    throwing::unique_ptr<A[]> t_ptr2(p2);
	t_ptr1 = std::move(t_ptr2);
    REQUIRE(t_ptr1.get() == p2);
    REQUIRE(!t_ptr2);
}
