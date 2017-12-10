//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <memory>

#if __cplusplus < 201103L
#error trowing_ptr requires at least C++11
#endif

namespace throwing {

template <typename T> class shared_ptr {
public:
    shared_ptr() = default;

    T *get() {
        if (!p)
            throw std::runtime_error("nullptr access");
        return p.get();
    }

private:
    std::shared_ptr<T> p;
};
}