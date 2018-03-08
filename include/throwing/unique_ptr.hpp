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
    template <typename OtherT, typename OtherDeleter> friend class unique_ptr;

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
     *
     * @todo the current implementation fails for VS2013
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

    /** \brief Destructor
     * If get() == nullptr there are no effects. Otherwise, the owned object is
     * destroyed via get_deleter()(get()) on the underlying unique_ptr.
     *
     * Requires that get_deleter()(get()) does not throw exceptions.
     */
    ~unique_ptr() = default;

    /** \brief Assignment operator
     *
     * Transfers ownership from r to *this as if by calling reset(r.release())
     * followed by an assignment of get_deleter() from
     * std::forward<E>(r.get_deleter()).
     *
     * If Deleter is not a reference type, requires that it is
     * nothrow-MoveAssignable.
     *
     * If Deleter is a reference type, requires that
     * std::remove_reference<Deleter>::type is nothrow-CopyAssignable.
     */
    unique_ptr &operator=(unique_ptr &&r) TSP_NOEXCEPT {
        p = std::move(r.p);
        return *this;
    }

    /** \brief Assignment operator
     *
     * Transfers ownership from r to *this as if by calling reset(r.release())
     * followed by an assignment of get_deleter() from
     * std::forward<E>(r.get_deleter()).
     *
     * If Deleter is not a reference type, requires that it is
     * nothrow-MoveAssignable.
     *
     * If Deleter is a reference type, requires that
     * std::remove_reference<Deleter>::type is nothrow-CopyAssignable.
     *
     * Only participates in overload resolution if U is not an array type and
     * unique_ptr<U,E>::pointer is implicitly convertible to pointer and
     * std::is_assignable<Deleter&, E&&>::value is true (since C++17).
     */
    template <class U, class E>
    unique_ptr &operator=(unique_ptr<U, E> &&r) TSP_NOEXCEPT {
        p = std::move(r.p);
        return *this;
    }

    /** \brief Assignment operator
     *
     * Effectively the same as calling reset().
     */
    unique_ptr &operator=(std::nullptr_t) TSP_NOEXCEPT {
        p = nullptr;
        return *this;
    }

    /** \brief Releases the ownership of the managed object if any.
     *
     * get() returns nullptr after the call.
     *
     * \return Pointer to the managed object or nullptr if there was no managed
     * object, i.e. the value which would be returned by get() before the call.
     */
    pointer release() TSP_NOEXCEPT { return p.release(); }

    /** \brief Replaces the managed object.
     *
     * Given current_ptr, the pointer that was managed by *this, performs the
     * following actions, in this order:
     *
     * - Saves a copy of the current pointer old_ptr = current_ptr
     * - Overwrites the current pointer with the argument current_ptr = ptr
     * - If the old pointer was non-empty, deletes the previously managed object
     * if(old_ptr != nullptr) get_deleter()(old_ptr).
     */
    void reset(pointer ptr = pointer()) TSP_NOEXCEPT { p.reset(ptr); }

    /** \brief Swaps the managed objects and associated deleters of *this and
     * another unique_ptr object other.
     */
    void swap(unique_ptr &other) TSP_NOEXCEPT { p.swap(other.p); }

    /** \brief Returns a pointer to the managed object or nullptr if no object
     * is owned.
     */
    pointer get() const TSP_NOEXCEPT { return p.get(); }

    /** \brief Returns the deleter object which would be used for destruction of
     * the managed object.
     */
    Deleter &get_deleter() TSP_NOEXCEPT { return p.get_deleter(); }

    /** \brief Returns the deleter object which would be used for destruction of
     * the managed object.
     */
    const Deleter &get_deleter() const TSP_NOEXCEPT { return p.get_deleter(); }

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
    typedef typename std::unique_ptr<T[], Deleter> std_unique_ptr_type;
    typedef typename std::unique_ptr<T[], Deleter>::pointer pointer;
    typedef typename std::unique_ptr<T[], Deleter>::element_type element_type;
    typedef typename std::unique_ptr<T[], Deleter>::deleter_type deleter_type;

    // allow access to p for other throwing::unique_ptr instantiations
    template <typename OtherT, typename OtherDeleter> friend class unique_ptr;

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
     *
     * This constructor is ill-formed if Deleter is of pointer or reference
     * type. (until C++17)
     *
     * This overload only participates in overload resolution if
     * std::is_default_constructible<Deleter>::value is true and Deleter is not
     * a pointer type. The program is ill-formed if this constructor is selected
     * by class template argument deduction. (since C++17)
     *
     * This overload will not participate in overload resolution unless one of
     * the following is true:
     * - U is the same type as pointer, or
     * - U is std::nullptr_t, or
     * - pointer is the same type as element_type* and U is some pointer type V*
     * such that V(*)[] is implicitly convertible to element_type(*)[]
     */
    template <class U> explicit unique_ptr(U ptr) TSP_NOEXCEPT : p(ptr) {}

    /** \brief Constructs a std::unique_ptr object which owns ptr
     *
     * Initialises the stored pointer with ptr and initialises the stored
     * deleter with d1
     *
     * This overload will not participate in overload resolution unless one of
     * the following is true:
     * - U is the same type as pointer, or
     * - U is std::nullptr_t, or
     * - pointer is the same type as element_type* and U is some pointer type V*
     * such that V(*)[] is implicitly convertible to element_type(*)[]
     *
     * @todo the current implementation fails for VS2013
     */
    template <class U>
    unique_ptr(U ptr,
               typename std::conditional<std::is_reference<Deleter>::value,
                                         Deleter, const Deleter &>::type d1)
            TSP_NOEXCEPT : p(ptr, std::forward<decltype(d1)>(d1)) {}

    /** \brief Constructs a std::unique_ptr object which owns ptr
     *
     * Initialises the stored pointer with ptr.
     * Moves d2 into stored_deleter.
     *
     * This overload will not participate in overload resolution unless one of
     * the following is true:
     * - U is the same type as pointer, or
     * - U is std::nullptr_t, or
     * - pointer is the same type as element_type* and U is some pointer type V*
     * such that V(*)[] is implicitly convertible to element_type(*)[]
     */
    template <class U>
    unique_ptr(U ptr,
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
     * following are true:
     * a) unique_ptr<U, E>::pointer is implicitly convertible to pointer
     * b) U is an array type
     * c) pointer is the same type as element_type*
     * d) unique_ptr<U,E>::pointer is the same type as
     * unique_ptr<U,E>::element_type*
     * e) unique_ptr<U,E>::element_type(*)[] is convertible to element_type(*)[]
     * f) Either Deleter is a reference type and E is the same type as Deleter,
     * or Deleter is not a reference type and E is implicitly convertible to
     * Deleter.
     */
    template <class U, class E>
    unique_ptr(unique_ptr<U, E> &&u) TSP_NOEXCEPT
            : p(std::move(u.get_unique_ptr())) {}

    /** \brief Destructor
     * If get() == nullptr there are no effects. Otherwise, the owned object is
     * destroyed via get_deleter()(get()) on the underlying unique_ptr.
     *
     * Requires that get_deleter()(get()) does not throw exceptions.
     */
    ~unique_ptr() = default;

    /** \brief Assignment operator
     *
     * Transfers ownership from r to *this as if by calling reset(r.release())
     * followed by an assignment of get_deleter() from
     * std::forward<E>(r.get_deleter()).
     *
     * If Deleter is not a reference type, requires that it is
     * nothrow-MoveAssignable.
     *
     * If Deleter is a reference type, requires that
     * std::remove_reference<Deleter>::type is nothrow-CopyAssignable.
     */
    unique_ptr &operator=(unique_ptr &&r) TSP_NOEXCEPT {
        p = std::move(r.p);
        return *this;
    }

    /** \brief Assignment operator
     *
     * Transfers ownership from r to *this as if by calling reset(r.release())
     * followed by an assignment of get_deleter() from
     * std::forward<E>(r.get_deleter()).
     *
     * If Deleter is not a reference type, requires that it is
     * nothrow-MoveAssignable.
     *
     * If Deleter is a reference type, requires that
     * std::remove_reference<Deleter>::type is nothrow-CopyAssignable.
     *
     * Only participates in overload resolution if all of the following is true:
     * - U is an array type
     * - pointer is the same type as element_type*
     * - unique_ptr<U,E>::pointer is the same type as
     * unique_ptr<U,E>::element_type*
     * - unique_ptr<U,E>::element_type(*)[] is convertible to element_type(*)[]
     * - std::is_assignable<Deleter&, E&&>::value is true
     */
    template <class U, class E>
    unique_ptr &operator=(unique_ptr<U, E> &&r) TSP_NOEXCEPT {
        p = std::move(r.p);
        return *this;
    }

    /** \brief Assignment operator
     *
     * Effectively the same as calling reset().
     */
    unique_ptr &operator=(std::nullptr_t) TSP_NOEXCEPT {
        p = nullptr;
        return *this;
    }

    /** \brief Releases the ownership of the managed object if any.
     *
     * get() returns nullptr after the call.
     *
     * \return Pointer to the managed object or nullptr if there was no managed
     * object, i.e. the value which would be returned by get() before the call.
     */
    pointer release() TSP_NOEXCEPT { return p.release(); }

    /** \brief Replaces the managed object.
     *
     * Given current_ptr, the pointer that was managed by *this, performs the
     * following actions, in this order:
     *
     * - Saves a copy of the current pointer old_ptr = current_ptr
     * - Overwrites the current pointer with the argument current_ptr = ptr
     * - If the old pointer was non-empty, deletes the previously managed object
     * if(old_ptr != nullptr) get_deleter()(old_ptr).
     */
    void reset(pointer ptr = pointer()) TSP_NOEXCEPT { p.reset(ptr); }

    /** \brief Replaces the managed object.
     *
     * Given current_ptr, the pointer that was managed by *this, performs the
     * following actions, in this order:
     *
     * - Saves a copy of the current pointer old_ptr = current_ptr
     * - Overwrites the current pointer with the argument current_ptr = ptr
     * - If the old pointer was non-empty, deletes the previously managed object
     * if(old_ptr != nullptr) get_deleter()(old_ptr).
     *
     * Will only participate in overload resolution if either:
     * - U is the same type as pointer, or
     * - pointer is the same type as element_type* and U is a pointer type V*
     * such that V(*)[] is convertible to element_type(*)[].
     * (available since C++17)
     */
    template <class U> void reset(U ptr) TSP_NOEXCEPT { p.reset(ptr); }

    /** \brief Equivalent to reset(pointer())
     */
    void reset(std::nullptr_t ptr = nullptr) TSP_NOEXCEPT { p.reset(ptr); }

    /** \brief Swaps the managed objects and associated deleters of *this and
     * another unique_ptr object other.
     */
    void swap(unique_ptr &other) TSP_NOEXCEPT { p.swap(other.p); }

    /** \brief Returns a pointer to the managed object or nullptr if no object
     * is owned.
     */
    pointer get() const TSP_NOEXCEPT { return p.get(); }

    /** \brief Returns the deleter object which would be used for destruction of
     * the managed object.
     */
    Deleter &get_deleter() TSP_NOEXCEPT { return p.get_deleter(); }

    /** \brief Returns the deleter object which would be used for destruction of
     * the managed object.
     */
    const Deleter &get_deleter() const TSP_NOEXCEPT { return p.get_deleter(); }

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

    /** \brief Returns reference to the wrapped std::unique_ptr
     */
    std_unique_ptr_type &get_unique_ptr() { return p; }

private:
    std::unique_ptr<T[], Deleter> p;
};
} // namespace throwing

#include <throwing/private/clear_compiler_checks.hpp>
