//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <throwing/unique_ptr.hpp>

int main() {
    // cannot assign from unique_ptr to another
    throwing::unique_ptr<int> from;
    throwing::unique_ptr<int> to;
    to = from;
    return 0;
}
