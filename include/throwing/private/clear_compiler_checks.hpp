//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/** \file throwing/private/clear_compiler_checks.hpp 
 * \brief Implementation details
 * This header file must not be included directly
 * and definitions herein may change without notice
 */

// Do not leak these definitions
#undef TSP_CONSTEXPR
#undef TSP_NOEXCEPT
#undef TSP_ARRAY_SUPPORT
