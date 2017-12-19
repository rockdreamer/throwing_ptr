//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <throwing/unique_ptr.hpp>

namespace {
struct Foo {
    Foo(bool &deleted) : m_deleted(deleted) {}
    Foo(const Foo &p) : m_deleted(p.m_deleted) {}
    Foo(Foo &&) = default;
    ~Foo() { m_deleted = true; }
    bool &m_deleted;
};

struct Deleter {
    Deleter(bool *copied, bool *moved, bool *called)
            : m_copied(copied), m_moved(moved), m_called(called){};
    Deleter(const Deleter &d)
            : m_copied(d.m_copied), m_moved(d.m_moved), m_called(d.m_called) {
        *m_copied = true;
    }
    Deleter(Deleter &d)
            : m_copied(d.m_copied), m_moved(d.m_moved), m_called(d.m_called) {
        *m_copied = true;
    }
    Deleter(Deleter &&d)
            : m_copied(std::move(d.m_copied)), m_moved(std::move(d.m_moved)),
              m_called(std::move(d.m_called)) {
        *m_moved = true;
    }
    void operator()(Foo *p) const {
        *m_called = true;
        delete p;
    };
    bool *m_copied;
    bool *m_moved;
    bool *m_called;
};
}

TEST(UniquePtrSingle, ConstuctWithPtr) {
    int *p = new int;
    throwing::unique_ptr<int> up(p);
    EXPECT_EQ(p, up.get());
}

TEST(UniquePtrSingle, ConstuctWithPtrAndNonReferenceDeleter) {
    bool d1_copied = false;
    bool d1_moved = false;
    bool d1_called = false;
    bool ptr1_deleted = false;
    bool d2_copied = false;
    bool d2_moved = false;
    bool d2_called = false;
    bool ptr2_deleted = false;
    Deleter d1(&d1_copied, &d1_moved, &d1_called);
    Deleter d2(&d2_copied, &d2_moved, &d2_called);
    {
        std::unique_ptr<Foo, Deleter> foo1(new Foo(ptr1_deleted), d1);
        throwing::unique_ptr<Foo, Deleter> foo2(new Foo(ptr2_deleted), d2);
        EXPECT_NE(nullptr, foo2.get());
        EXPECT_EQ(d1_copied, d2_copied);
        EXPECT_EQ(d1_moved, d2_moved);
        EXPECT_EQ(d1_called, d2_called);
        EXPECT_FALSE(ptr2_deleted);
    }
    EXPECT_EQ(d1_copied, d2_copied);
    EXPECT_EQ(d1_moved, d2_moved);
    EXPECT_EQ(d1_called, d2_called);
    EXPECT_TRUE(ptr2_deleted);
}

TEST(UniquePtrSingle, ConstuctWithPtrAndReferenceDeleter) {
    bool d1_copied = false;
    bool d1_moved = false;
    bool d1_called = false;
    bool ptr1_deleted = false;
    bool d2_copied = false;
    bool d2_moved = false;
    bool d2_called = false;
    bool ptr2_deleted = false;
    Deleter d1(&d1_copied, &d1_moved, &d1_called);
    Deleter d2(&d2_copied, &d2_moved, &d2_called);
    {
        std::unique_ptr<Foo, Deleter &> foo1(new Foo(ptr1_deleted), d1);
        throwing::unique_ptr<Foo, Deleter &> foo2(new Foo(ptr2_deleted), d2);
        EXPECT_NE(nullptr, foo2.get());
        EXPECT_EQ(d1_copied, d2_copied);
        EXPECT_EQ(d1_moved, d2_moved);
        EXPECT_EQ(d1_called, d2_called);
        EXPECT_FALSE(ptr2_deleted);
    }
    EXPECT_EQ(d1_copied, d2_copied);
    EXPECT_EQ(d1_moved, d2_moved);
    EXPECT_EQ(d1_called, d2_called);
    EXPECT_TRUE(ptr2_deleted);
}

TEST(UniquePtrSingle, ConstuctWithPtrAndMovedReferenceDeleter) {
    bool d1_copied = false;
    bool d1_moved = false;
    bool d1_called = false;
    bool ptr1_deleted = false;
    bool d2_copied = false;
    bool d2_moved = false;
    bool d2_called = false;
    bool ptr2_deleted = false;
    Deleter d1(&d1_copied, &d1_moved, &d1_called);
    Deleter d2(&d2_copied, &d2_moved, &d2_called);
    {
        std::unique_ptr<Foo, Deleter> foo1(new Foo(ptr1_deleted),
                                           std::move(d1));
        throwing::unique_ptr<Foo, Deleter> foo2(new Foo(ptr2_deleted),
                                                std::move(d2));
        EXPECT_NE(nullptr, foo2.get());
        EXPECT_EQ(d1_copied, d2_copied);
        EXPECT_EQ(d1_moved, d2_moved);
        EXPECT_EQ(d1_called, d2_called);
        EXPECT_FALSE(ptr2_deleted);
    }
    EXPECT_EQ(d1_copied, d2_copied);
    EXPECT_EQ(d1_moved, d2_moved);
    EXPECT_EQ(d1_called, d2_called);
    EXPECT_TRUE(ptr2_deleted);
}

TEST(UniquePtrSingle, ConstuctFromConvertibleCopyDeleter) {
    bool d1_copied = false;
    bool d1_moved = false;
    bool d1_called = false;
    bool ptr1_deleted = false;
    bool d2_copied = false;
    bool d2_moved = false;
    bool d2_called = false;
    bool ptr2_deleted = false;
    auto clear_all = [&]() {
        d1_copied = d2_copied = d1_called = d2_called = d1_moved = d2_moved =
                false;
    };
    Deleter d1(&d1_copied, &d1_moved, &d1_called);
    Deleter d2(&d2_copied, &d2_moved, &d2_called);
    {
        std::unique_ptr<Foo, Deleter> sup6a(new Foo(ptr1_deleted), d1);
        throwing::unique_ptr<Foo, Deleter> tup6a(new Foo(ptr2_deleted), d2);
        EXPECT_NE(nullptr, tup6a.get());
        EXPECT_EQ(d1_copied, d2_copied);
        EXPECT_EQ(d1_moved, d2_moved);
        EXPECT_EQ(d1_called, d2_called);
        EXPECT_FALSE(ptr2_deleted);
        clear_all();
        {
            std::unique_ptr<Foo, Deleter> sup6b(std::move(sup6a));
            EXPECT_NE(nullptr, sup6b.get());
            EXPECT_FALSE(ptr2_deleted);
            throwing::unique_ptr<Foo, Deleter> tup6b(std::move(tup6a));
            EXPECT_NE(nullptr, tup6b.get());
            EXPECT_EQ(d1_copied, d2_copied);
            EXPECT_EQ(d1_moved, d2_moved);
            EXPECT_EQ(d1_called, d2_called);
            EXPECT_FALSE(ptr2_deleted);
            clear_all();
        }
        EXPECT_TRUE(ptr2_deleted);
    }
}

TEST(UniquePtrSingle, ConstuctFromConvertibleMoveDeleter) {
    bool d1_copied = false;
    bool d1_moved = false;
    bool d1_called = false;
    bool ptr1_deleted = false;
    bool d2_copied = false;
    bool d2_moved = false;
    bool d2_called = false;
    bool ptr2_deleted = false;
    auto clear_all = [&]() {
        d1_copied = d2_copied = d1_called = d2_called = d1_moved = d2_moved =
                false;
    };
    Deleter d1(&d1_copied, &d1_moved, &d1_called);
    Deleter d2(&d2_copied, &d2_moved, &d2_called);
    {
        std::unique_ptr<Foo, Deleter &> sup6a(new Foo(ptr1_deleted), d1);
        throwing::unique_ptr<Foo, Deleter &> tup6a(new Foo(ptr2_deleted), d2);
        EXPECT_NE(nullptr, tup6a.get());
        EXPECT_EQ(d1_copied, d2_copied);
        EXPECT_EQ(d1_moved, d2_moved);
        EXPECT_EQ(d1_called, d2_called);
        EXPECT_FALSE(ptr2_deleted);
        clear_all();
        {
            std::unique_ptr<Foo, Deleter> sup6b(std::move(sup6a));
            EXPECT_NE(nullptr, sup6b.get());
            EXPECT_FALSE(ptr2_deleted);

            throwing::unique_ptr<Foo, Deleter> tup6b(std::move(tup6a));
            EXPECT_NE(nullptr, tup6b.get());
            EXPECT_EQ(d1_copied, d2_copied);
            EXPECT_EQ(d1_moved, d2_moved);
            EXPECT_EQ(d1_called, d2_called);
            EXPECT_FALSE(ptr2_deleted);
            clear_all();
        }
        EXPECT_TRUE(ptr2_deleted);
    }
}
