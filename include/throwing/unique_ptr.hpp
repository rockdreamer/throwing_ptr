//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <functional>
#include <memory>
#include <throwing/null_ptr_exception.hpp>
#include <throwing/private/compiler_checks.hpp>

namespace throwing {

/** \brief unique_ptr that manages a single object
 *
 * throwing::unique_ptr is a smart pointer that owns and manages another object
 * through a pointer and disposes of that object when the unique_ptr goes out of
 * scope.
 *
 * The object is disposed of using the associated deleter when either of the
 * following happens:
 * - the managing unique_ptr object is destroyed
 * - the managing unique_ptr object is assigned another pointer via operator= or
 * reset().
 *
 * The object is disposed of using a potentially user-supplied deleter by
 * calling get_deleter()(ptr). The default deleter uses the delete operator,
 * which destroys the object and deallocates the memory.
 *
 * A unique_ptr may alternatively own no object, in which case it is called
 * empty.
 */
template <typename T, typename Deleter = std::default_delete<T>>
class unique_ptr {
public:
    typedef typename std::unique_ptr<T, Deleter>::pointer pointer;
    typedef typename std::unique_ptr<T, Deleter>::element_type element_type;
    typedef typename std::unique_ptr<T, Deleter>::deleter_type deleter_type;

    /** \brief Constructs a std::unique_ptr that owns nothing.
     *
     * Value-initializes the stored pointer and the stored deleter.
     * Requires that Deleter is DefaultConstructible and that construction does
     * not throw an exception.
     */
    TSP_CONSTEXPR unique_ptr() TSP_NOEXCEPT = default;

    /** \brief Constructs a std::unique_ptr that owns nothing.
     *
     * Value-initializes the stored pointer and the stored deleter.
     * Requires that Deleter is DefaultConstructible and that construction does
     * not throw an exception.
     */
    TSP_CONSTEXPR unique_ptr( std::nullptr_t ) TSP_NOEXCEPT {}

    /** \brief Returns a pointer to the managed object or nullptr if no object
     * is owned.
     */
    pointer get() const TSP_NOEXCEPT { return p.get(); }

private:
    std::unique_ptr<T, Deleter> p;
};

/** \brief unique_ptr that manages a dynamically-allocated array of objects
 *
 * throwing::unique_ptr is a smart pointer that owns and manages another object
 * through a pointer and disposes of that object when the unique_ptr goes out of
 * scope.
 *
 * The object is disposed of using the associated deleter when either of the
 * following happens:
 * - the managing unique_ptr object is destroyed
 * - the managing unique_ptr object is assigned another pointer via operator= or
 * reset().
 *
 * The object is disposed of using a potentially user-supplied deleter by
 * calling get_deleter()(ptr). The default deleter uses the delete operator,
 * which destroys the object and deallocates the memory.
 *
 * A unique_ptr may alternatively own no object, in which case it is called
 * empty.
 */
template <typename T, typename Deleter> class unique_ptr<T[], Deleter> {
public:
    typedef typename std::unique_ptr<T[], Deleter>::pointer pointer;
    typedef typename std::unique_ptr<T[], Deleter>::element_type element_type;
    typedef typename std::unique_ptr<T[], Deleter>::deleter_type deleter_type;

    /** \brief Constructs a std::unique_ptr that owns nothing.
     *
     * Value-initializes the stored pointer and the stored deleter.
     * Requires that Deleter is DefaultConstructible and that construction does
     * not throw an exception.
     */
    TSP_CONSTEXPR unique_ptr() TSP_NOEXCEPT = default;

    /** \brief Constructs a std::unique_ptr that owns nothing.
     *
     * Value-initializes the stored pointer and the stored deleter.
     * Requires that Deleter is DefaultConstructible and that construction does
     * not throw an exception.
     */
    TSP_CONSTEXPR unique_ptr( std::nullptr_t ) TSP_NOEXCEPT {}

    /** \brief Returns a pointer to the managed object or nullptr if no object
     * is owned.
     */
    pointer get() const TSP_NOEXCEPT { return p.get(); }

    /** \brief provides access to elements of an array managed by a unique_ptr.
     *
     * The parameter i shall be less than the number of elements in the array;
     * otherwise, the behavior is undefined.
     *
     * \throw throwing::null_ptr_exception<element_type> if the unique_ptr is
     * empty
     */
    T &operator[](size_t i) const {
        if (!p)
            throw null_ptr_exception<element_type>();
        return p[i];
    }

private:
    std::unique_ptr<T[], Deleter> p;
};
}

#include <throwing/private/clear_compiler_checks.hpp>
