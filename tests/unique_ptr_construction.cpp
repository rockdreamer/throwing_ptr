//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>
#include <throwing/unique_ptr.hpp>
#ifdef _MSC_VER
#pragma warning(disable : 4521)
#endif

namespace {
struct Foo {
    Foo() : m_deleted(nullptr) {}
    Foo(bool &deleted) : m_deleted(&deleted) {}
    Foo(const Foo &p) : m_deleted(p.m_deleted) {}
    Foo(Foo &&p) : m_deleted(p.m_deleted) {}
    Foo &operator=(const Foo &other) {
        if (this != &other) {
            m_deleted = other.m_deleted;
        }
        return *this;
    }
    ~Foo() { *m_deleted = true; }
    bool *m_deleted;
};

struct Deleter {
    Deleter() : m_copied(nullptr), m_moved(nullptr), m_called(nullptr) {}
    Deleter(bool *copied, bool *moved, bool *called)
            : m_copied(copied), m_moved(moved), m_called(called) {}
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
    Deleter &operator=(const Deleter &other) {
        if (this != &other) {
            *m_copied = true;
        }
        return *this;
    }

    void operator()(Foo *p) const {
        *m_called = true;
        delete p;
    }
    bool *m_copied;
    bool *m_moved;
    bool *m_called;
};
} // namespace

TEST_CASE("unique_ptr to single object constructor from pointer",
          "[unique_ptr][single][constructor]") {
    int *p = new int;
    throwing::unique_ptr<int> up(p);
    REQUIRE(up.get() == p);
}

TEST_CASE("unique_ptr to single object constructor from pointer and non "
          "reference deleter",
          "[unique_ptr][single][constructor]") {
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
        REQUIRE(foo2.get() != nullptr);
        REQUIRE(d2_copied == d1_copied);
        REQUIRE(d2_moved == d1_moved);
        REQUIRE(d2_called == d1_called);
        REQUIRE_FALSE(ptr2_deleted);
    }
    REQUIRE(d2_copied == d1_copied);
    REQUIRE(d2_moved == d1_moved);
    REQUIRE(d2_called == d1_called);
    REQUIRE(ptr2_deleted);
}

#if !defined(_MSC_VER) || _MSC_VER > 1800
TEST_CASE("unique_ptr to single object constructor from pointer and reference "
          "to deleter",
          "[unique_ptr][single][constructor]") {
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
        REQUIRE(foo2.get() != nullptr);
        REQUIRE(d2_copied == d1_copied);
        REQUIRE(d2_moved == d1_moved);
        REQUIRE(d2_called == d1_called);
        REQUIRE_FALSE(ptr2_deleted);
    }
    REQUIRE(d2_copied == d1_copied);
    REQUIRE(d2_moved == d1_moved);
    REQUIRE(d2_called == d1_called);
    REQUIRE(ptr2_deleted);
}
#endif

TEST_CASE("unique_ptr to single object constructor from pointer and "
          "move-reference to deleter",
          "[unique_ptr][single][constructor]") {
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
        REQUIRE(foo2.get() != nullptr);
        REQUIRE(d2_copied == d1_copied);
        REQUIRE(d2_moved == d1_moved);
        REQUIRE(d2_called == d1_called);
        REQUIRE_FALSE(ptr2_deleted);
    }
    REQUIRE(d2_copied == d1_copied);
    REQUIRE(d2_moved == d1_moved);
    REQUIRE(d2_called == d1_called);
    REQUIRE(ptr2_deleted);
}

TEST_CASE("unique_ptr to single object constructor from convertible pointer "
          "and copied deleter",
          "[unique_ptr][single][constructor]") {
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
        REQUIRE(tup6a.get() != nullptr);
        REQUIRE(d2_copied == d1_copied);
        REQUIRE(d2_moved == d1_moved);
        REQUIRE(d2_called == d1_called);
        REQUIRE_FALSE(ptr2_deleted);
        clear_all();
        {
            std::unique_ptr<Foo, Deleter> sup6b(std::move(sup6a));
            REQUIRE(sup6b.get() != nullptr);
            REQUIRE_FALSE(ptr2_deleted);
            throwing::unique_ptr<Foo, Deleter> tup6b(std::move(tup6a));
            REQUIRE(tup6b.get() != nullptr);
            REQUIRE(d2_copied == d1_copied);
            REQUIRE(d2_moved == d1_moved);
            REQUIRE(d2_called == d1_called);
            REQUIRE_FALSE(ptr2_deleted);
            clear_all();
        }
        REQUIRE(ptr2_deleted);
    }
}

#if !defined(_MSC_VER) || _MSC_VER > 1800
TEST_CASE("unique_ptr to single object constructor from convertible object and "
          "moved deleter",
          "[unique_ptr][single][constructor]") {
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
        REQUIRE(tup6a.get() != nullptr);
        REQUIRE(d2_copied == d1_copied);
        REQUIRE(d2_moved == d1_moved);
        REQUIRE(d2_called == d1_called);
        REQUIRE_FALSE(ptr2_deleted);
        clear_all();
        {
            std::unique_ptr<Foo, Deleter> sup6b(std::move(sup6a));
            REQUIRE(sup6b.get() != nullptr);
            REQUIRE_FALSE(ptr2_deleted);

            throwing::unique_ptr<Foo, Deleter> tup6b(std::move(tup6a));
            REQUIRE(tup6b.get() != nullptr);
            REQUIRE(d2_copied == d1_copied);
            REQUIRE(d2_moved == d1_moved);
            REQUIRE(d2_called == d1_called);
            REQUIRE_FALSE(ptr2_deleted);
            clear_all();
        }
        REQUIRE(ptr2_deleted);
    }
}
#endif
