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
    typedef typename std::unique_ptr<T, Deleter> std_unique_ptr_type;
    typedef typename std::unique_ptr<T, Deleter>::pointer pointer;
    typedef typename std::unique_ptr<T, Deleter>::element_type element_type;
    typedef typename std::unique_ptr<T, Deleter>::deleter_type deleter_type;

    // allow access to p for other throwing::unique_ptr instantiations
    //    template <typename Y, deleter_type> friend class unique_ptr;

    /** \brief Constructs a unique_ptr that owns nothing.
     *
     * Value-initializes the stored pointer and the stored deleter.
     * Requires that Deleter is DefaultConstructible and that construction does
     * not throw an exception.
     */
    TSP_CONSTEXPR unique_ptr() TSP_NOEXCEPT = default;

    /** \brief Constructs a unique_ptr that owns nothing.
     *
     * Value-initializes the stored pointer and the stored deleter.
     * Requires that Deleter is DefaultConstructible and that construction does
     * not throw an exception.
     */
    TSP_CONSTEXPR unique_ptr(std::nullptr_t) TSP_NOEXCEPT {}

    /** \brief Constructs a unique_ptr which owns p.
     *
     * Initialises the stored pointer with ptr and value-initialises the stored
     * deleter.
     *
     * Requires that Deleter is DefaultConstructible and that construction does
     * not throw an exception.
     */
    explicit unique_ptr(pointer ptr) TSP_NOEXCEPT : p(ptr) {}

    /** \brief Constructs a std::unique_ptr object which owns p
     *
     * Initialises the stored pointer with ptr and initialises the stored
     * deleter with d1
     *
     * Requires that Deleter is nothrow-CopyConstructible
     */
    unique_ptr(pointer ptr,
               typename std::conditional<std::is_reference<Deleter>::value,
                                         Deleter, const Deleter &>::type d1)
            TSP_NOEXCEPT : p(ptr, std::forward<decltype(d1)>(d1)) {}

    /** \brief Constructs a std::unique_ptr object which owns p
     *
     * Initialises the stored pointer with ptr.
     * Moves d2 into stored_deleter.
     */
    unique_ptr(pointer ptr,
               typename std::remove_reference<Deleter>::type &&d2) TSP_NOEXCEPT
            : p(ptr, std::forward<decltype(d2)>(d2)) {}

    /** \brief Constructs a unique_ptr by transferring ownership from u to
     * *this.
     *
     * If Deleter is not a reference type, requires that it is
     * nothrow-MoveConstructible (if Deleter is a reference, get_deleter() and
     * u.get_deleter() after move construction reference the same value)
     */
    unique_ptr(unique_ptr &&u) TSP_NOEXCEPT : p(std::move(u.p)) {}

    /** \brief Constructs a unique_ptr by transferring ownership from u to
     * *this, where u is constructed with a specified deleter (E).
     *
     * It depends upon whether E is a reference type, as following:
     * a) if E is a reference type, this deleter is copy constructed from u's
     * deleter (requires that this construction does not throw)
     * b) if E is a non-reference type, this deleter is move constructed from
     * u's deleter (requires that this construction does not throw)
     *
     * This constructor only participates in overload resolution if all of the
     * following is true:
     * a) unique_ptr<U, E>::pointer is implicitly convertible to pointer
     * b) U is not an array type
     * c) Either Deleter is a reference type and E is the same type as D, or
     * Deleter is not a reference type and E is implicitly convertible to D
     */
    template <class U, class E>
    unique_ptr(unique_ptr<U, E> &&u) TSP_NOEXCEPT
            : p(std::move(u.get_unique_ptr())) {}

    /** \brief Returns a pointer to the managed object or nullptr if no object
     * is owned.
     */
    pointer get() const TSP_NOEXCEPT { return p.get(); }

    /** \brief Returns reference to the wrapped std::unique_ptr
     */
    std_unique_ptr_type &get_unique_ptr() { return p; }

private:
    std_unique_ptr_type p;
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

    /** \brief Constructs a unique_ptr that owns nothing.
     *
     * Value-initializes the stored pointer and the stored deleter.
     * Requires that Deleter is DefaultConstructible and that construction does
     * not throw an exception.
     */
    TSP_CONSTEXPR unique_ptr() TSP_NOEXCEPT = default;

    /** \brief Constructs a unique_ptr that owns nothing.
     *
     * Value-initializes the stored pointer and the stored deleter.
     * Requires that Deleter is DefaultConstructible and that construction does
     * not throw an exception.
     */
    TSP_CONSTEXPR unique_ptr(std::nullptr_t) TSP_NOEXCEPT {}

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
