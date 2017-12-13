//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <throwing/shared_ptr.hpp>

namespace {
class A {
    int dummy;
};

class B : public A {
    int dummy2;
};

struct A_D {
    void operator()(A *p) const { delete p; }
};

struct Contained {
    // some data that we want to point to
    int dummy;
};

struct Container {
    Contained things;
};
}

TEST(Get, NullPtr) {
    throwing::shared_ptr<int> nothing;
    EXPECT_EQ(nullptr, nothing.get());

    throwing::shared_ptr<int> nothing_nullptr(nullptr);
    EXPECT_EQ(nullptr, nothing.get());

    throwing::shared_ptr<int> nothing_null(NULL);
    EXPECT_EQ(nullptr, nothing.get());
}

TEST(Construction, ConstructFromPointer) {
    A *ptr1 = new A;
    auto t_ptr1 = throwing::shared_ptr<A>(ptr1);
    EXPECT_EQ(ptr1, t_ptr1.get());
}

TEST(Construction, ConstructFromPointerAndDeleter) {
    A *ptr1 = new A;
    auto t_ptr1 = throwing::shared_ptr<A>(ptr1, A_D());
    EXPECT_EQ(ptr1, t_ptr1.get());
}

TEST(Construction, ConstructFromPointerAndLamdaDeleter) {
    A *ptr1 = new A;
    bool lamda_called = false;
    {
        auto t_ptr1 = throwing::shared_ptr<A>(ptr1, [&lamda_called](A *p) {
            delete p;
            lamda_called = true;
        });
        EXPECT_EQ(ptr1, t_ptr1.get());
        EXPECT_FALSE(lamda_called);
    }
    EXPECT_TRUE(lamda_called);
}

TEST(Construction, ConstructFromPointerToDerived) {
    B *ptr1 = new B;
    auto t_ptr1 = throwing::shared_ptr<A>(ptr1);
    EXPECT_EQ(ptr1, t_ptr1.get());
}

TEST(Construction, ConstructFromBaseType) {
    int *ptr1 = new int;
    auto t_ptr1 = throwing::shared_ptr<int>(ptr1);
    EXPECT_EQ(ptr1, t_ptr1.get());
}

TEST(Construction, ConstructFromNullPtrAndLamdaDeleter) {
    bool lamda_called = false;
    {
        auto t_ptr1 = throwing::shared_ptr<A>(nullptr, [&lamda_called](A *p) {
            delete p;
            lamda_called = true;
        });
        EXPECT_EQ(nullptr, t_ptr1.get());
        EXPECT_FALSE(lamda_called);
    }
    EXPECT_TRUE(lamda_called);
}

TEST(Construction, ConstructFromNullPtrLamdaDeleterAndAllocator) {
    A *ptr1 = new A;
    std::allocator<void *> allocator;
    bool lamda_called = false;
    {
        auto t_ptr1 = throwing::shared_ptr<A>(ptr1,
                                              [&lamda_called](A *p) {
                                                  delete p;
                                                  lamda_called = true;
                                              },
                                              allocator);
        EXPECT_EQ(ptr1, t_ptr1.get());
        EXPECT_FALSE(lamda_called);
    }
    EXPECT_TRUE(lamda_called);
}

TEST(Construction, AliasingConstructor) {
    Container *ptr1 = new Container;
    bool lamda_called = false;
    {
        auto t_ptr1 = throwing::shared_ptr<Container>(
                ptr1, [&lamda_called](Container *p) {
                    delete p;
                    lamda_called = true;
                });
        EXPECT_EQ(ptr1, t_ptr1.get());
        EXPECT_FALSE(lamda_called);
        auto via_aliasing =
                throwing::shared_ptr<Contained>(t_ptr1, &ptr1->things);
        EXPECT_FALSE(lamda_called);
        t_ptr1.reset();
        EXPECT_FALSE(lamda_called);
        via_aliasing.reset();
        EXPECT_TRUE(lamda_called);
    }
}

TEST(Construction, CopyConstructor) {
    A *ptr1 = new A;
    auto t_ptr1 = throwing::shared_ptr<A>(ptr1);
    throwing::shared_ptr<A> t_ptr2 = t_ptr1;
    EXPECT_EQ(t_ptr1.get(), t_ptr2.get());
    t_ptr1.reset();
    EXPECT_EQ(ptr1, t_ptr2.get());
}

TEST(Construction, CopyConstructorFromDerived) {
    B *ptr1 = new B;
    auto t_ptr1 = throwing::shared_ptr<B>(ptr1);
    throwing::shared_ptr<A> t_ptr2 = t_ptr1;
    EXPECT_EQ(t_ptr1.get(), t_ptr2.get());
    t_ptr1.reset();
    EXPECT_EQ(ptr1, t_ptr2.get());
}

TEST(Construction, MoveConstructor) {
    A *ptr1 = new A;
    auto t_ptr1 = throwing::shared_ptr<A>(ptr1);
    throwing::shared_ptr<A> t_ptr2(std::move(t_ptr1));
    EXPECT_EQ(nullptr, t_ptr1.get());
    EXPECT_EQ(ptr1, t_ptr2.get());
}

TEST(Construction, MoveConstructorFromDerived) {
    B *ptr1 = new B;
    auto t_ptr1 = throwing::shared_ptr<B>(ptr1);
    throwing::shared_ptr<A> t_ptr2(std::move(t_ptr1));
    EXPECT_EQ(nullptr, t_ptr1.get());
    EXPECT_EQ(ptr1, t_ptr2.get());
}

TEST(Construction, ConstructorFromStdSharedPtr) {
    A *ptr1 = new A;
    auto t_ptr1 = std::shared_ptr<A>(ptr1);
    throwing::shared_ptr<A> t_ptr2 = t_ptr1;
    EXPECT_EQ(t_ptr1.get(), t_ptr2.get());
    t_ptr1.reset();
    EXPECT_EQ(ptr1, t_ptr2.get());
}

TEST(Construction, ConstructorFromDerivedStdSharedPtr) {
    B *ptr1 = new B;
    auto t_ptr1 = std::shared_ptr<B>(ptr1);
    throwing::shared_ptr<A> t_ptr2 = t_ptr1;
    EXPECT_EQ(t_ptr1.get(), t_ptr2.get());
    t_ptr1.reset();
    EXPECT_EQ(ptr1, t_ptr2.get());
}

TEST(Construction, MoveConstructorFromStdSharedPtr) {
    A *ptr1 = new A;
    auto t_ptr1 = std::shared_ptr<A>(ptr1);
    throwing::shared_ptr<A> t_ptr2(std::move(t_ptr1));
    EXPECT_EQ(nullptr, t_ptr1.get());
    EXPECT_EQ(ptr1, t_ptr2.get());
}

TEST(Construction, MoveConstructorFromDerivedStdSharedPtr) {
    B *ptr1 = new B;
    auto t_ptr1 = std::shared_ptr<B>(ptr1);
    throwing::shared_ptr<A> t_ptr2(std::move(t_ptr1));
    EXPECT_EQ(nullptr, t_ptr1.get());
    EXPECT_EQ(ptr1, t_ptr2.get());
}

TEST(Construction, ConstructorFromDerivedStdWeakPtr) {
    B *ptr1 = new B;
    auto t_ptr1 = std::shared_ptr<B>(ptr1);
    std::weak_ptr<B> weak = t_ptr1;
    throwing::shared_ptr<A> t_ptr2(weak);
    EXPECT_EQ(t_ptr1.get(), t_ptr2.get());
    t_ptr1.reset();
    EXPECT_EQ(ptr1, t_ptr2.get());
}

TEST(Construction, ConstructorFromDerivedStdUniquePtr) {
    B *ptr1 = new B;
    auto u_ptr1 = std::unique_ptr<B>(ptr1);
    throwing::shared_ptr<A> t_ptr2(std::move(u_ptr1));
    EXPECT_EQ(nullptr, u_ptr1.get());
    EXPECT_EQ(ptr1, t_ptr2.get());
}
