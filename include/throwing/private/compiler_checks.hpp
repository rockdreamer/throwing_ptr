//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// This header file must not be included directly
// and definitions herein may change without notice

#if (defined(_MSC_VER) && _MSC_VER < 1800) ||                                  \
        (!defined(_MSC_VER) && __cplusplus < 201103L)
#error throwing_ptr requires at least C++11
#endif

#if defined(__clang__)
// clang supports from 3.1 on
#define TSP_CONSTEXPR constexpr
#define TSP_NOEXCEPT noexcept
#define TSP_ARRAY_SUPPORT                                                      \
    defined(__cpp_lib_shared_ptr_arrays) && __cpp_lib_shared_ptr_arrays
#elif defined(_MSC_VER)

#if _MSC_VER >= 1910
// Visual Studio 2017 15.5
#define TSP_CONSTEXPR constexpr
#define TSP_NOEXCEPT noexcept
#define TSP_ARRAY_SUPPORT (_MSVC_LANG > 201402)
#elif _MSC_VER >= 1910
// Visual Studio 2017 15.0
#define TSP_CONSTEXPR constexpr
#define TSP_NOEXCEPT noexcept
#define TSP_ARRAY_SUPPORT false
#elif _MSC_VER >= 1900
// Visual Studio 2015 14.0
#define TSP_CONSTEXPR constexpr
#define TSP_NOEXCEPT noexcept
#define TSP_ARRAY_SUPPORT false
#elif _MSC_VER >= 1800
// Visual Studio 2013 
#define TSP_CONSTEXPR
#define TSP_NOEXCEPT
#define TSP_ARRAY_SUPPORT false
#endif

#elif defined(__GNUC__)

#if __cpp_constexpr >= 200704
#define TSP_CONSTEXPR constexpr
#else
#define TSP_CONSTEXPR
#endif

#define TSP_NOEXCEPT noexcept
#define TSP_ARRAY_SUPPORT                                                      \
    defined(__cpp_lib_shared_ptr_arrays) && __cpp_lib_shared_ptr_arrays

#else
// Unknown compiler, bare minimum
#define TSP_CONSTEXPR
#define TSP_NOEXCEPT
#define TSP_ARRAY_SUPPORT false
#endif
