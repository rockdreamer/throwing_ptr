//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

class TestBaseClass {
    int dummy_a = 1;

public:
    int dummy() { return dummy_a; }
};

class TestDerivedClass : public TestBaseClass {
    int dummy_b = 2;

public:
    int dummy() { return dummy_b; }
};

struct TestBaseClassSimpleDeleter {
    void operator()(A *p) const { delete p; }
};

struct TestContentClass {
    // some data that we want to point to
    int dummy_ = 3;
    int dummy() { return dummy_; }
};

struct TestContainingClass {
    Contained things;
};

struct MemoryPositionHelper {
    // helper for owner_before
    int m1;
    int m2;
    Foo(int a, int b) : m1(a), m2(b) {}
};