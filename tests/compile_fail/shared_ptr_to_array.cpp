//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <throwing/shared_ptr.hpp>

int main() {
    int *p = new int[10];
    throwing::shared_ptr<int[]> sp(p);
    return *p = sp[0];
}
