//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <throwing/unique_ptr.hpp>

int main() {
    // operator[] is not present in single element unique_ptr
    throwing::unique_ptr<int> up;
    return up[0];
}
