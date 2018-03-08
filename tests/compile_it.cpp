//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "throwing/shared_ptr.hpp"
#include "throwing/unique_ptr.hpp"

int main(int, char **) {
    // Have one instance of each class in throwing::
    throwing::shared_ptr<int> ptr;
    ptr.reset();
    return 0;
}
