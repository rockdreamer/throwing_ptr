//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
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

TEST(UniquePtrToArray, ConstuctWithPtr) {
    int *p = new int[10];
    throwing::unique_ptr<int[]> up(p);
    EXPECT_EQ(p, up.get());
}

TEST(UniquePtrToArray, ConstuctWithPtrAndNonReferenceDeleter) {
    EXPECT_EQ(0, Foo::object_count());
    Deleter<1> d1;
    EXPECT_EQ(1, Deleter<1>::object_count());
    Deleter<2> d2;
    EXPECT_EQ(1, Deleter<2>::object_count());
    {
        std::unique_ptr<Foo[], Deleter<1>> foo1(new Foo[10], d1);
        EXPECT_EQ(10, Foo::object_count());
        EXPECT_EQ(2, Deleter<1>::object_count());

        throwing::unique_ptr<Foo[], Deleter<2>> foo2(new Foo[10], d2);
        EXPECT_EQ(20, Foo::object_count());
        EXPECT_EQ(2, Deleter<2>::object_count());

        EXPECT_NE(nullptr, foo2.get());
        EXPECT_EQ(0, Deleter<2>::deleter_calls());
    }
    EXPECT_EQ(0, Foo::object_count());
    EXPECT_EQ(1, Deleter<1>::object_count());
    EXPECT_EQ(1, Deleter<2>::object_count());
    EXPECT_EQ(1, Deleter<2>::deleter_calls());
}

#if !defined(_MSC_VER) || _MSC_VER > 1800
TEST(UniquePtrToArray, ConstuctWithPtrAndReferenceDeleter) {
    EXPECT_EQ(0, Foo::object_count());
    Deleter<3> d1;
    EXPECT_EQ(1, Deleter<3>::object_count());
    Deleter<4> d2;
    EXPECT_EQ(1, Deleter<4>::object_count());
    {
        std::unique_ptr<Foo[], Deleter<3> &> foo1(new Foo[10], d1);
        EXPECT_EQ(10, Foo::object_count());
        EXPECT_EQ(1, Deleter<3>::object_count());

        throwing::unique_ptr<Foo[], Deleter<4> &> foo2(new Foo[10], d2);
        EXPECT_EQ(20, Foo::object_count());
        EXPECT_EQ(1, Deleter<4>::object_count());

        EXPECT_NE(nullptr, foo2.get());
        EXPECT_EQ(0, Deleter<4>::deleter_calls());
    }
    EXPECT_EQ(1, Deleter<3>::object_count());
    EXPECT_EQ(1, Deleter<4>::object_count());
    EXPECT_EQ(1, Deleter<4>::deleter_calls());
}
#endif

TEST(UniquePtrToArray, ConstuctWithPtrAndMovedReferenceDeleter) {
    EXPECT_EQ(0, Foo::object_count());
    Deleter<5> d1;
    EXPECT_EQ(1, Deleter<5>::object_count());
    Deleter<6> d2;
    EXPECT_EQ(1, Deleter<6>::object_count());
    {
        std::unique_ptr<Foo[], Deleter<5>> foo1(new Foo[10], std::move(d1));
        EXPECT_EQ(10, Foo::object_count());
        EXPECT_EQ(1, Deleter<5>::object_count());

        throwing::unique_ptr<Foo[], Deleter<6>> foo2(new Foo[10],
                                                     std::move(d2));
        EXPECT_EQ(20, Foo::object_count());
        EXPECT_EQ(1, Deleter<6>::object_count());

        EXPECT_NE(nullptr, foo2.get());
        EXPECT_EQ(0, Deleter<6>::deleter_calls());
    }
    EXPECT_EQ(Deleter<5>::object_count(), Deleter<6>::object_count());
    EXPECT_EQ(1, Deleter<6>::deleter_calls());
}

TEST(UniquePtrToArray, ConstuctFromConvertibleCopyDeleter) {
    EXPECT_EQ(0, Foo::object_count());
    Deleter<7> d1;
    EXPECT_EQ(1, Deleter<7>::object_count());
    Deleter<8> d2;
    EXPECT_EQ(1, Deleter<8>::object_count());
    {
        std::unique_ptr<Foo[], Deleter<7>> foo1(new Foo[10], d1);
        EXPECT_EQ(10, Foo::object_count());
        throwing::unique_ptr<Foo[], Deleter<8>> foo2(new Foo[10], d2);
        EXPECT_EQ(20, Foo::object_count());
        EXPECT_EQ(Deleter<7>::object_count(), Deleter<8>::object_count());

        EXPECT_NE(nullptr, foo2.get());
        EXPECT_EQ(0, Deleter<8>::deleter_calls());
        {
            std::unique_ptr<Foo[], Deleter<7>> moved_foo1(std::move(foo1));
            EXPECT_EQ(20, Foo::object_count());
            throwing::unique_ptr<Foo[], Deleter<8>> moved_foo2(std::move(foo2));
            EXPECT_EQ(20, Foo::object_count());
            EXPECT_EQ(Deleter<7>::object_count(), Deleter<8>::object_count());
            EXPECT_EQ(0, Deleter<8>::deleter_calls());
            EXPECT_NE(nullptr, moved_foo2.get());
        }
        EXPECT_EQ(Deleter<7>::object_count(), Deleter<8>::object_count());
        EXPECT_EQ(1, Deleter<8>::deleter_calls());
    }
    EXPECT_EQ(Deleter<7>::object_count(), Deleter<8>::object_count());
    EXPECT_EQ(1, Deleter<8>::deleter_calls());
}

#if !defined(_MSC_VER) || _MSC_VER > 1800
TEST(UniquePtrToArray, ConstuctFromConvertibleMoveDeleter) {
    EXPECT_EQ(0, Foo::object_count());
    Deleter<9> d1;
    EXPECT_EQ(1, Deleter<9>::object_count());
    Deleter<10> d2;
    EXPECT_EQ(1, Deleter<10>::object_count());
    {
        std::unique_ptr<Foo[], Deleter<9> &> foo1(new Foo[10], d1);
        EXPECT_EQ(10, Foo::object_count());
        EXPECT_EQ(1, Deleter<9>::object_count());

        throwing::unique_ptr<Foo[], Deleter<10> &> foo2(new Foo[10], d2);
        EXPECT_EQ(20, Foo::object_count());
        EXPECT_EQ(1, Deleter<10>::object_count());

        EXPECT_NE(nullptr, foo2.get());
        EXPECT_EQ(0, Deleter<10>::deleter_calls());
        {
            std::unique_ptr<Foo[], Deleter<9>> moved_foo1(std::move(foo1));
            EXPECT_EQ(20, Foo::object_count());
            throwing::unique_ptr<Foo[], Deleter<10>> moved_foo2(
                    std::move(foo2));
            EXPECT_EQ(20, Foo::object_count());
            EXPECT_EQ(Deleter<9>::object_count(), Deleter<10>::object_count());
            EXPECT_EQ(0, Deleter<10>::deleter_calls());
            EXPECT_NE(nullptr, moved_foo2.get());
        }
        EXPECT_EQ(Deleter<9>::object_count(), Deleter<10>::object_count());
        EXPECT_EQ(1, Deleter<10>::deleter_calls());
    }
    EXPECT_EQ(Deleter<9>::object_count(), Deleter<10>::object_count());
    EXPECT_EQ(1, Deleter<10>::deleter_calls());
}
#endif
