//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <memory>

#if (defined(_MSC_VER) && _MSC_VER < 1800) || __cplusplus < 201103L
#error trowing_ptr requires at least C++11
#endif

#if defined(__clang__)
// clang supports from 3.1 on
#define TSP_CONSTEXPR constexpr

#elif defined(_MSC_VER)
#if _MSC_VER < 1900

// Visual Studio 2013
#define TSP_CONSTEXPR

#elif _MSC_VER < 1910

// Visual Studio 2015
#define TSP_CONSTEXPR constexpr

#else

// Visual Studio 2017
#define TSP_CONSTEXPR constexpr

#endif

#elif defined(__GNUC__)

#if __cpp_constexpr >= 200704
#define TSP_CONSTEXPR constexpr
#else
#define TSP_CONSTEXPR
#endif

#else
// Unknown compiler, bare minimum
#define TSP_CONSTEXPR
#endif

#if (defined(_MSC_VER) && _MSC_VER < 1900) || __cplusplus < 201103L
#define TSP_CONSTEXPR
#else
#define TSP_CONSTEXPR constexpr
#endif

namespace throwing {

template <typename T> class shared_ptr {
public:
    /** \brief Constructs a shared_ptr with no managed object, i.e. empty
     * shared_ptr.
     */
    TSP_CONSTEXPR shared_ptr() noexcept = default;

    /** \brief Constructs a shared_ptr with no managed object, i.e. empty
     * shared_ptr.
     */
    TSP_CONSTEXPR shared_ptr(std::nullptr_t ptr) noexcept : p(ptr){};

    /** \brief Returns the stored pointer.
     */
    T *get() const noexcept { return p.get(); }

private:
    std::shared_ptr<T> p;
};
}

// Do not leak these definitions
#undef TSP_CONSTEXPR
