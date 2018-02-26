//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/unique_ptr.hpp>

namespace {

struct Foo {
    static int &object_count() {
        static int count = 0;
        return count;
    }
    static bool all_objects_deleted() { return object_count() == 0; }
    Foo() { ++object_count(); }
    Foo(const Foo &) { ++object_count(); }
    Foo(Foo &&) {}
    ~Foo() { --object_count(); }
};

template <int id> struct Deleter {
    static int &object_count() {
        static int count = 0;
        return count;
    }
    static int &deleter_calls() {
        static int count = 0;
        return count;
    }
    Deleter() { ++object_count(); }
    Deleter(const Deleter &) { ++object_count(); }
    Deleter(Deleter &&) {}
    ~Deleter() { --object_count(); }

    void operator()(Foo *p) const {
        ++deleter_calls();
        delete[] p;
    }
};
} // namespace

TEST_CASE("unique_ptr to array construction from pointer",
          "[unique_ptr][array][constructor]") {
    int *p = new int[10];
    throwing::unique_ptr<int[]> up(p);
    REQUIRE(up.get() == p);
}

TEST_CASE("unique_ptr to array construction from pointer and non reference "
          "deleter",
          "[unique_ptr][array][constructor]") {
    REQUIRE(Foo::object_count() == 0);
    Deleter<1> d1;
    REQUIRE(Deleter<1>::object_count() == 1);
    Deleter<2> d2;
    REQUIRE(Deleter<2>::object_count() == 1);
    {
        std::unique_ptr<Foo[], Deleter<1>> foo1(new Foo[10], d1);
        REQUIRE(Foo::object_count() == 10);

        throwing::unique_ptr<Foo[], Deleter<2>> foo2(new Foo[10], d2);
        REQUIRE(Foo::object_count() == 20);
        REQUIRE(Deleter<2>::object_count() == Deleter<1>::object_count());

        REQUIRE(foo2.get() != nullptr);
        REQUIRE(Deleter<2>::deleter_calls() == 0);
    }
    REQUIRE(Foo::object_count() == 0);
    REQUIRE(Deleter<2>::object_count() == Deleter<1>::object_count());
    REQUIRE(Deleter<2>::deleter_calls() == 1);
}

#if !defined(_MSC_VER) || _MSC_VER > 1800
TEST_CASE("unique_ptr to array construction from pointer and reference deleter",
          "[unique_ptr][array][constructor]") {
    REQUIRE(Foo::object_count() == 0);
    Deleter<3> d1;
    REQUIRE(Deleter<3>::object_count() == 1);
    Deleter<4> d2;
    REQUIRE(Deleter<4>::object_count() == 1);
    {
        std::unique_ptr<Foo[], Deleter<3> &> foo1(new Foo[10], d1);
        REQUIRE(Foo::object_count() == 10);
        REQUIRE(Deleter<3>::object_count() == 1);

        throwing::unique_ptr<Foo[], Deleter<4> &> foo2(new Foo[10], d2);
        REQUIRE(Foo::object_count() == 20);
        REQUIRE(Deleter<4>::object_count() == 1);

        REQUIRE(foo2.get() != nullptr);
        REQUIRE(Deleter<4>::deleter_calls() == 0);
    }
    REQUIRE(Deleter<3>::object_count() == 1);
    REQUIRE(Deleter<4>::object_count() == 1);
    REQUIRE(Deleter<4>::deleter_calls() == 1);
}
#endif

TEST_CASE("unique_ptr to array construction from pointer and move reference "
          "deleter",
          "[unique_ptr][array][constructor]") {
    REQUIRE(Foo::object_count() == 0);
    Deleter<5> d1;
    REQUIRE(Deleter<5>::object_count() == 1);
    Deleter<6> d2;
    REQUIRE(Deleter<6>::object_count() == 1);
    {
        std::unique_ptr<Foo[], Deleter<5>> foo1(new Foo[10], std::move(d1));
        REQUIRE(Foo::object_count() == 10);

        throwing::unique_ptr<Foo[], Deleter<6>> foo2(new Foo[10],
                                                     std::move(d2));
        REQUIRE(Foo::object_count() == 20);
        REQUIRE(Deleter<5>::object_count() == Deleter<6>::object_count());

        REQUIRE(foo2.get() != nullptr);
        REQUIRE(Deleter<6>::deleter_calls() == 0);
    }
    REQUIRE(Deleter<6>::object_count() == Deleter<5>::object_count());
    REQUIRE(Deleter<6>::deleter_calls() == 1);
}

TEST_CASE("unique_ptr to array construction from convertible pointer and copy "
          "deleter",
          "[unique_ptr][array][constructor]") {
    REQUIRE(Foo::object_count() == 0);
    Deleter<7> d1;
    REQUIRE(Deleter<7>::object_count() == 1);
    Deleter<8> d2;
    REQUIRE(Deleter<8>::object_count() == 1);
    {
        std::unique_ptr<Foo[], Deleter<7>> foo1(new Foo[10], d1);
        REQUIRE(Foo::object_count() == 10);
        throwing::unique_ptr<Foo[], Deleter<8>> foo2(new Foo[10], d2);
        REQUIRE(Foo::object_count() == 20);
        REQUIRE(Deleter<8>::object_count() == Deleter<7>::object_count());

        REQUIRE(foo2.get() != nullptr);
        REQUIRE(Deleter<8>::deleter_calls() == 0);
        {
            std::unique_ptr<Foo[], Deleter<7>> moved_foo1(std::move(foo1));
            REQUIRE(Foo::object_count() == 20);
            throwing::unique_ptr<Foo[], Deleter<8>> moved_foo2(std::move(foo2));
            REQUIRE(Foo::object_count() == 20);
            REQUIRE(Deleter<8>::object_count() == Deleter<7>::object_count());
            REQUIRE(Deleter<8>::deleter_calls() == 0);
            REQUIRE(moved_foo2.get() != nullptr);
        }
        REQUIRE(Deleter<8>::object_count() == Deleter<7>::object_count());
        REQUIRE(Deleter<8>::deleter_calls() == 1);
    }
    REQUIRE(Deleter<8>::object_count() == Deleter<7>::object_count());
    REQUIRE(Deleter<8>::deleter_calls() == 1);
}

#if !defined(_MSC_VER) || _MSC_VER > 1800
TEST_CASE("unique_ptr to array construction from convertible pointer and move "
          "deleter",
          "[unique_ptr][array][constructor]") {
    REQUIRE(Foo::object_count() == 0);
    Deleter<9> d1;
    REQUIRE(Deleter<9>::object_count() == 1);
    Deleter<10> d2;
    REQUIRE(Deleter<10>::object_count() == 1);
    {
        std::unique_ptr<Foo[], Deleter<9> &> foo1(new Foo[10], d1);
        REQUIRE(Foo::object_count() == 10);

        throwing::unique_ptr<Foo[], Deleter<10> &> foo2(new Foo[10], d2);
        REQUIRE(Foo::object_count() == 20);
        REQUIRE(Deleter<10>::object_count() == Deleter<9>::object_count());

        REQUIRE(foo2.get() != nullptr);
        REQUIRE(Deleter<10>::deleter_calls() == 0);
        {
            std::unique_ptr<Foo[], Deleter<9>> moved_foo1(std::move(foo1));
            REQUIRE(Foo::object_count() == 20);
            throwing::unique_ptr<Foo[], Deleter<10>> moved_foo2(
                    std::move(foo2));
            REQUIRE(Foo::object_count() == 20);
            REQUIRE(Deleter<10>::object_count() == Deleter<9>::object_count());
            REQUIRE(Deleter<10>::deleter_calls() == 0);
            REQUIRE(moved_foo2.get() != nullptr);
        }
        REQUIRE(Deleter<10>::object_count() == Deleter<9>::object_count());
        REQUIRE(Deleter<10>::deleter_calls() == 1);
    }
    REQUIRE(Deleter<10>::object_count() == Deleter<9>::object_count());
    REQUIRE(Deleter<10>::deleter_calls() == 1);
}
#endif
