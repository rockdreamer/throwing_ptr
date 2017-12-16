//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <memory>

#if (defined(_MSC_VER) && _MSC_VER < 1800) ||                                  \
        (!defined(_MSC_VER) && __cplusplus < 201103L)
#error throwing_ptr requires at least C++11
#endif

#if defined(__clang__)
// clang supports from 3.1 on
#define TSP_CONSTEXPR constexpr
#define TSP_NOEXCEPT noexcept

#elif defined(_MSC_VER)

#if _MSC_VER >= 1910
// Visual Studio 2017
#define TSP_CONSTEXPR constexpr
#define TSP_NOEXCEPT noexcept
#elif _MSC_VER >= 1900
// Visual Studio 2015
#define TSP_CONSTEXPR constexpr
#define TSP_NOEXCEPT noexcept
#elif _MSC_VER >= 1800
// Visual Studio 2013
#define TSP_CONSTEXPR
#define TSP_NOEXCEPT
#endif

#elif defined(__GNUC__)

#if __cpp_constexpr >= 200704
#define TSP_CONSTEXPR constexpr
#else
#define TSP_CONSTEXPR
#endif

#define TSP_NOEXCEPT noexcept

#else
// Unknown compiler, bare minimum
#define TSP_CONSTEXPR
#define TSP_NOEXCEPT
#endif

namespace throwing {

template <typename T> class shared_ptr {
public:
    typedef typename std::shared_ptr<T>::element_type element_type;

    // allow access to p for other throwing::shared_ptr instantiations
    template <typename Y> friend class shared_ptr;

    /** \brief Constructs a shared_ptr with no managed object, i.e. empty
     * shared_ptr.
     */
    TSP_CONSTEXPR shared_ptr() TSP_NOEXCEPT = default;

    /** \brief Constructs a shared_ptr with no managed object, i.e. empty
     * shared_ptr.
     */
    TSP_CONSTEXPR shared_ptr(std::nullptr_t ptr) TSP_NOEXCEPT : p(ptr) {}

    /** \brief Constructs a shared_ptr with ptr as the pointer to the managed
     * object.
     */
    template <class Y> explicit shared_ptr(Y *ptr) : p(ptr) {}

    /** \brief Constructs a shared_ptr with ptr as the pointer to the managed
     * object.
     *
     * Uses the specified deleter d as the deleter.
     *
     * The expression d(ptr) must be well formed, have well-defined behavior and
     * not throw any exceptions.
     *
     * The construction of d and of the stored deleter from d must not throw
     * exceptions.
     */
    template <class Y, class Deleter>
    shared_ptr(Y *ptr, Deleter d) : p(ptr, d) {}

    /** \brief Constructs a shared_ptr with ptr as the pointer to the managed
     * object.
     *
     * Uses the specified deleter d as the deleter.
     *
     * The expression d(ptr) must be well formed, have well-defined behavior and
     * not throw any exceptions.
     *
     * The construction of d and of the stored deleter from d must not throw
     * exceptions.
     */
    template <class Deleter>
    shared_ptr(std::nullptr_t ptr, Deleter d) : p(ptr, d) {}

    /** \brief Constructs a shared_ptr with ptr as the pointer to the managed
     * object.
     *
     * Uses the specified deleter d as the deleter.
     *
     * The expression d(ptr) must be well formed, have well-defined behavior and
     * not throw any exceptions.
     *
     * The construction of d and of the stored deleter from d must
     * not throw exceptions.
     *
     * Uses a copy of alloc for allocation of data for internal use.
     *
     * Alloc must be a Allocator.
     */
    template <class Y, class Deleter, class Alloc>
    shared_ptr(Y *ptr, Deleter d, Alloc alloc) : p(ptr, d, alloc) {}

    /** \brief  The aliasing constructor
     *
     * Constructs a shared_ptr which shares ownership information with r, but
     * holds an unrelated and unmanaged pointer ptr. Even if this shared_ptr is
     * the last of the group to go out of scope, it will call the destructor for
     * the object originally managed by r.
     *
     * However, calling get() on this will always return a copy of ptr.
     *
     * It is the responsibility of the programmer to make sure that this ptr
     * remains valid as long as this shared_ptr exists, such as in the typical
     * use cases where ptr is a member of the object managed by r or is an alias
     * (e.g., downcast) of r.get()
     */
    template <class Y>
    shared_ptr(const shared_ptr<Y> &r, element_type *ptr) TSP_NOEXCEPT
            : p(r.p, ptr) {}

    /** \brief  Constructs a shared_ptr which shares ownership of the object
     * managed by r.
     *
     * If r manages no object, *this manages no object too.
     */
    shared_ptr(const shared_ptr &r) TSP_NOEXCEPT : p(r.p) {}

    /** \brief  Constructs a shared_ptr which shares ownership of the object
     * managed by r.
     *
     * If r manages no object, *this manages no object too.
     */
    template <class Y>
    shared_ptr(const shared_ptr<Y> &r) TSP_NOEXCEPT : p(r.p) {}

    /** \brief  Move-constructs a shared_ptr from r.
     *
     * After the construction, *this contains a copy of the previous state of r,
     * r is empty and its stored pointer is null.
     */
    shared_ptr(shared_ptr &&r) TSP_NOEXCEPT : p(std::move(r.p)) {}

    /** \brief  Move-constructs a shared_ptr from r.
     *
     * After the construction, *this contains a copy of the previous state of r,
     * r is empty and its stored pointer is null.
     */
    template <class Y>
    shared_ptr(shared_ptr<Y> &&r) TSP_NOEXCEPT : p(std::move(r.p)) {}

    /** \brief  Constructs a shared_ptr which shares ownership of the object
     * managed by r.
     *
     * If r manages no object, *this manages no object too.
     */
    shared_ptr(const std::shared_ptr<T> &r) TSP_NOEXCEPT : p(r) {}

    /** \brief  Constructs a shared_ptr which shares ownership of the object
     * managed by r.
     *
     * If r manages no object, *this manages no object too.
     */
    template <class Y>
    shared_ptr(const std::shared_ptr<Y> &r) TSP_NOEXCEPT : p(r) {}

    /** \brief  Move-constructs a shared_ptr from r.
     *
     * After the construction, *this contains a copy of the previous state of r,
     * r is empty and its stored pointer is null.
     */
    shared_ptr(std::shared_ptr<T> &&r) TSP_NOEXCEPT : p(std::move(r)) {}

    /** \brief  Move-constructs a shared_ptr from r.
     *
     * After the construction, *this contains a copy of the previous state of r,
     * r is empty and its stored pointer is null.
     */
    template <class Y>
    shared_ptr(std::shared_ptr<Y> &&r) TSP_NOEXCEPT : p(std::move(r)) {}

    /** \brief Constructs a shared_ptr which shares ownership of the object
     * managed by r.
     *
     * Y* must be implicitly convertible to T*. (until C++17)
     *
     * This overload only participates in overload resolution if Y* is
     * compatible with T*. (since C++17)
     *
     * Note that r.lock() may be used for the same purpose: the difference is
     * that this constructor throws an exception if the argument is empty, while
     * std::weak_ptr<T>::lock() constructs an empty std::shared_ptr in that
     * case.
     */
    template <class Y> explicit shared_ptr(const std::weak_ptr<Y> &r) : p(r) {}

    /**\brief Constructs a shared_ptr which manages the object currently managed
     * by r.
     *
     * The deleter associated with r is stored for future deletion of the
     * managed object.
     *
     * r manages no object after the call.
     *
     * This overload doesn't participate in overload resolution if
     * std::unique_ptr<Y, Deleter>::pointer is not compatible with T*.
     * If r.get() is a null pointer, this overload is equivalent to the default
     * constructor (1). 	(since C++17)
     *
     * If Deleter is a reference type, equivalent to shared_ptr(r.release(),
     * std::ref(r.get_deleter()).
     * Otherwise, equivalent to shared_ptr(r.release(), r.get_deleter())
     */
    template <class Y, class Deleter>
    shared_ptr(std::unique_ptr<Y, Deleter> &&r) : p(std::move(r)) {}

    /** \brief Destructor
     * If *this owns an object and it is the last shared_ptr owning it, the
     * object is destroyed through the owned deleter.
     * After the destruction, the smart pointers that shared ownership with
     * *this, if any, will report a use_count() that is one less than its
     * previous value.

     * Notes:

     * Unlike throwing::unique_ptr, the throwing of std::shared_ptr is invoked
     * even if the managed pointer is null.
     */
    ~shared_ptr() = default;

    /** \brief Assignment operator
     *
     * Replaces the managed object with the one managed by r.
     * If *this already owns an object and it is the last shared_ptr owning it,
     * and r is not the same as *this, the object is destroyed through the owned
     * deleter.
     *
     * Shares ownership of the object managed by r. If r manages no object,
     * *this manages no object too.
     * Equivalent to shared_ptr<T>(r).swap(*this).
     */
    shared_ptr &operator=(const shared_ptr &r) TSP_NOEXCEPT {
        p = r.p;
        return *this;
    }

    /** \brief Assignment operator
     *
     * Replaces the managed object with the one managed by r.
     * If *this already owns an object and it is the last shared_ptr owning it,
     * and r is not the same as *this, the object is destroyed through the owned
     * deleter.
     *
     * Shares ownership of the object managed by r. If r manages no object,
     * *this manages no object too.
     * Equivalent to shared_ptr<T>(r).swap(*this).
     */
    template <class Y>
    shared_ptr &operator=(const shared_ptr<Y> &r) TSP_NOEXCEPT {
        p = r.p;
        return *this;
    }

    /** \brief Assignment operator
     *
     * Replaces the managed object with the one managed by r.
     * If *this already owns an object and it is the last shared_ptr owning it,
     * and r is not the same as *this, the object is destroyed through the owned
     * deleter.
     *
     * Move-assigns a shared_ptr from r. After the assignment, *this contains a
     * copy of the previous state of r, r is empty. Equivalent to
     * shared_ptr<T>(std::move(r)).swap(*this)
     */
    shared_ptr &operator=(shared_ptr &&r) TSP_NOEXCEPT {
        p = std::move(r.p);
        return *this;
    }

    /** \brief Assignment operator
     *
     * Replaces the managed object with the one managed by r.
     * If *this already owns an object and it is the last shared_ptr owning it,
     * and r is not the same as *this, the object is destroyed through the owned
     * deleter.
     *
     * Move-assigns a shared_ptr from r. After the assignment, *this contains a
     * copy of the previous state of r, r is empty. Equivalent to
     * shared_ptr<T>(std::move(r)).swap(*this)
     */
    template <class Y> shared_ptr &operator=(shared_ptr<Y> &&r) TSP_NOEXCEPT {
        p = std::move(r.p);
        return *this;
    }

    /** \brief Assignment operator
     *
     * Replaces the managed object with the one managed by r.
     * If *this already owns an object and it is the last shared_ptr owning it,
     * and r is not the same as *this, the object is destroyed through the owned
     * deleter.
     *
     * Transfers the ownership of the object managed by r to *this. The deleter
     * associated to r is stored for future deletion of the managed object. r
     * manages no object after the call. Equivalent to
     * shared_ptr<T>(std::move(r)).swap(*this).
     */
    template <class Y, class Deleter>
    shared_ptr &operator=(std::unique_ptr<Y, Deleter> &&r) {
        p = std::move(r);
        return *this;
    }

    /** \brief Returns the stored pointer.
     */
    T *get() const TSP_NOEXCEPT { return p.get(); }

    /** \brief Releases the ownership of the managed object, if any.
     *
     * If *this already owns an object and it is the last shared_ptr owning it,
     * the object is destroyed through the owned deleter.
     * If the object pointed to by ptr is already owned, the function results in
     * undefined behavior.
     *
     * After the call, *this manages no object.
     * Equivalent to shared_ptr().swap(*this);
     */
    void reset() TSP_NOEXCEPT { p.reset(); }

private:
    std::shared_ptr<T> p;
};
} // namespace throwing

// Do not leak these definitions
#undef TSP_CONSTEXPR
#undef TSP_NOEXCEPT
