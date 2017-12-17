//          Copyright Claudio Bantaloukas 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <atomic>
#include <functional>
#include <iosfwd>
#include <memory>
#include <throwing/null_ptr_exception.hpp>
#include <throwing/private/compiler_checks.hpp>

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

    /** \brief Exchanges the contents of *this and r
     */
    void swap(shared_ptr &r) TSP_NOEXCEPT { p.swap(r.p); }

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

    /** \brief Replaces the managed object with an object pointed to by ptr.
     *
     * Y must be a complete type and implicitly convertible to T.
     *
     * Uses the delete expression as the deleter. A valid delete expression must
     * be available, i.e. delete ptr must be well formed, have well-defined
     * behavior and not throw any exceptions.
     *
     * Equivalent to shared_ptr<T>(ptr).swap(*this);
     */
    template <class Y> void reset(Y *ptr) { p.reset(ptr); }

    /** \brief Replaces the managed object with an object pointed to by ptr.
     *
     * Y must be a complete type and implicitly convertible to T.
     *
     * Uses the specified deleter d as the deleter. Deleter must be callable for
     * the type T, i.e. d(ptr) must be well formed, have well-defined behavior
     * and not throw any exceptions. Deleter must be CopyConstructible, and its
     * copy constructor and destructor must not throw exceptions.
     *
     * Equivalent to shared_ptr<T>(ptr, d).swap(*this);
     */
    template <class Y, class Deleter> void reset(Y *ptr, Deleter d) {
        p.reset(ptr, d);
    }

    /** \brief Replaces the managed object with an object pointed to by ptr.
     *
     * Y must be a complete type and implicitly convertible to T.
     *
     * Uses the specified deleter d as the deleter. Deleter must be callable for
     * the type T, i.e. d(ptr) must be well formed, have well-defined behavior
     * and not throw any exceptions. Deleter must be CopyConstructible, and its
     * copy constructor and destructor must not throw exceptions.
     *
     * Additionally uses a copy of alloc for allocation of data for internal
     * use. Alloc must be a Allocator. The copy constructor and destructor must
     * not throw exceptions.
     *
     * Equivalent to shared_ptr<T>(ptr, d, alloc).swap(*this);
     */
    template <class Y, class Deleter, class Alloc>
    void reset(Y *ptr, Deleter d, Alloc alloc) {
        p.reset(ptr, d, alloc);
    }

    /** \brief Returns the stored pointer.
     */
    element_type *get() const TSP_NOEXCEPT { return p.get(); }

    /** \brief Returns the underlying std::shared_pointer.
     */
    const std::shared_ptr<T> &get_std_shared_ptr() const TSP_NOEXCEPT {
        return p;
    }

    /** \brief Returns the underlying std::shared_pointer.
     */
    std::shared_ptr<T> &get_std_shared_ptr() TSP_NOEXCEPT { return p; }

    /** \brief Dereferences the stored pointer.
     *
     * Throws null_ptr_exception<T> if the pointer is null
     */
    T &operator*() const {
        const auto ptr = get();
        if (nullptr == ptr)
            throw null_ptr_exception<T>();
        return *ptr;
    }

    /** \brief Dereferences the stored pointer.
     *
     * Throws null_ptr_exception<T> if the pointer is null
     */
    T *operator->() const {
        const auto ptr = get();
        if (nullptr == ptr)
            throw null_ptr_exception<T>();
        return ptr;
    }

#if TSP_ARRAY_SUPPORT
    /** \brief Index into the array pointed to by the stored pointer.
     *
     * The behavior is undefined if the stored pointer is null or if idx is
     * negative.
     *
     * If T (the template parameter of shared_ptr) is an array type U[N], idx
     * must be less than N, otherwise the behavior is undefined.
     *
     * This method is available if the underlying compiler and c++ library
     * implementation support it
     * 
     * Throws null_ptr_exception<T> if the pointer is null
     */
    element_type &operator[](std::ptrdiff_t idx) {
        if (!p)
            throw null_ptr_exception<T>();
        return p.operator[](idx);
    }
#endif

    /** \brief Returns the number of different shared_ptr instances (this
     * included) managing the current object.
     *
     * If there is no managed object, ​0​ is returned.
     *
     * In multithreaded environment, the value returned by use_count is
     * approximate (typical implementations use a memory_order_relaxed load)
     */
    long use_count() const TSP_NOEXCEPT { return p.use_count(); }

    /** \brief Checks if *this stores a non-null pointer, i.e. whether get() !=
     * nullptr.
     */
    explicit operator bool() const TSP_NOEXCEPT { return p.operator bool(); }

    /** \brief Checks whether this shared_ptr precedes other in implementation
     * defined owner-based (as opposed to value-based) order.
     *
     * The order is such that two smart pointers compare equivalent only if they
     * are both empty or if they both own the same object, even if the values of
     * the pointers obtained by get() are different (e.g. because they point at
     * different subobjects within the same object)
     *
     * This ordering is used to make shared and weak pointers usable as keys in
     * associative containers, typically through std::owner_less.
     */
    template <class Y>
    bool owner_before(const shared_ptr<Y> &other) const TSP_NOEXCEPT {
        return p.owner_before(other.p);
    }

    /** \brief Checks whether this shared_ptr precedes other in implementation
     * defined owner-based (as opposed to value-based) order.
     *
     * The order is such that two smart pointers compare equivalent only if they
     * are both empty or if they both own the same object, even if the values of
     * the pointers obtained by get() are different (e.g. because they point at
     * different subobjects within the same object)
     *
     * This ordering is used to make shared and weak pointers usable as keys in
     * associative containers, typically through std::owner_less.
     */
    template <class Y>
    bool owner_before(const std::shared_ptr<Y> &other) const TSP_NOEXCEPT {
        return p.owner_before(other);
    }

    /** \brief Checks whether this shared_ptr precedes other in implementation
     * defined owner-based (as opposed to value-based) order.
     *
     * The order is such that two smart pointers compare equivalent only if they
     * are both empty or if they both own the same object, even if the values of
     * the pointers obtained by get() are different (e.g. because they point at
     * different subobjects within the same object)
     *
     * This ordering is used to make shared and weak pointers usable as keys in
     * associative containers, typically through std::owner_less.
     */
    template <class Y>
    bool owner_before(const std::weak_ptr<Y> &other) const TSP_NOEXCEPT {
        return p.owner_before(other);
    }

private:
    std::shared_ptr<T> p;
};

/** \brief Specializes the std::swap algorithm for throwing::shared_ptr.
 *
 * Swaps the pointers of lhs and rhs.
 *
 * Calls lhs.swap(rhs).
 */
template <class T>
void swap(throwing::shared_ptr<T> &lhs,
          throwing::shared_ptr<T> &rhs) TSP_NOEXCEPT {
    lhs.swap(rhs);
}

/** \brief Constructs an object of type T and wraps it in a throwing::shared_ptr
 * using args as the parameter list for the constructor of T.
 *
 * The object is constructed as if by the expression ::new (pv)
 * T(std::forward<Args>(args)...), where pv is an internal void* pointer to
 * storage suitable to hold an object of type T.
 *
 * The storage is typically larger than sizeof(T) in order to use one allocation
 * for both the control block of the shared pointer and the T object.
 *
 * The shared_ptr constructor called by this function enables
 * shared_from_this with a pointer to the newly constructed object of type T.
 *
 * This overload only participates in overload resolution if T is not an array
 * type
 */
template <class T, class... Args> shared_ptr<T> make_shared(Args &&... args) {
    return shared_ptr<T>(
            std::move(std::make_shared<T>(std::forward<Args>(args)...)));
}

/** \brief Constructs an object of type T and wraps it in a throwing::shared_ptr
 * using args as the parameter list for the constructor of T.
 *
 * The object is constructed as if by the expression
 * std::allocator_traits<A2>::construct(a, pv, v)), where pv is an internal
 * void* pointer to storage suitable to hold an object of type T and a is the
 * possibly-rebound copy of the allocator.
 *
 * The storage is typically larger than sizeof(T) in order to use one allocation
 * for both the control block of the shared pointer and the T object.
 *
 * The shared_ptr constructor called by this function enables shared_from_this
 * with a pointer to the newly constructed object of type T.
 *
 * All memory allocation is done using a copy of alloc, which must satisfy the
 * Allocator requirements.
 *
 * This overload only participates in overload resolution if T is not an array
 * type
 */
template <class T, class Alloc, class... Args>
shared_ptr<T> allocate_shared(const Alloc &alloc, Args &&... args) {
    return shared_ptr<T>(std::move(
            std::allocate_shared<T>(alloc, std::forward<Args>(args)...)));
}

/** \brief Creates a new instance of shared_ptr whose stored pointer is obtained
 * from r's stored pointer using a static_cast expression.
 *
 * If r is empty, so is the new shared_ptr (but its stored pointer is not
 * necessarily null).
 *
 * Otherwise, the new shared_ptr will share ownership with r.
 *
 * The behavior is undefined unless static_cast<T*>((U*)nullptr) is well formed.
 */
template <class T, class U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U> &r) TSP_NOEXCEPT {
    auto p = static_cast<typename shared_ptr<T>::element_type *>(r.get());
    return shared_ptr<T>(r, p);
}

/** \brief Creates a new instance of shared_ptr whose stored pointer is obtained
 * from r's stored pointer using a dynamic_cast expression.
 *
 * If r is empty, so is the new shared_ptr (but its stored pointer is not
 * necessarily null).
 *
 * Otherwise, the new shared_ptr will share ownership with r, except that it is
 * empty if the dynamic_cast performed by dynamic_pointer_cast returns a null
 * pointer.
 *
 * The behavior is undefined unless dynamic_cast<T*>((U*)nullptr) is well
 * formed.
 */
template <class T, class U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U> &r) TSP_NOEXCEPT {
    if (auto p =
                dynamic_cast<typename shared_ptr<T>::element_type *>(r.get())) {
        return shared_ptr<T>(r, p);
    } else {
        return shared_ptr<T>();
    }
}

/** \brief Creates a new instance of shared_ptr whose stored pointer is obtained
 * from r's stored pointer using a const_cast expression.
 *
 * If r is empty, so is the new shared_ptr (but its stored pointer is not
 * necessarily null).
 *
 * Otherwise, the new shared_ptr will share ownership with r.
 *
 * The behavior is undefined unless const_cast<T*>((U*)nullptr) is well formed.
 */
template <class T, class U>
shared_ptr<T> const_pointer_cast(const shared_ptr<U> &r) TSP_NOEXCEPT {
    auto p = const_cast<typename shared_ptr<T>::element_type *>(r.get());
    return shared_ptr<T>(r, p);
}

/** \brief Creates a new instance of shared_ptr whose stored pointer is obtained
 * from r's stored pointer using a reinterpret_cast expression.
 *
 * If r is empty, so is the new shared_ptr (but its stored pointer is not
 * necessarily null).
 *
 * Otherwise, the new shared_ptr will share ownership with r.
 *
 * The behavior is undefined unless reinterpret_cast<T*>((U*)nullptr) is well
 * formed.
 */
template <class T, class U>
shared_ptr<T> reinterpret_pointer_cast(const shared_ptr<U> &r) TSP_NOEXCEPT {
    auto p = reinterpret_cast<typename shared_ptr<T>::element_type *>(r.get());
    return shared_ptr<T>(r, p);
}

/** \brief Access to the p's deleter.
 *
 * If the shared pointer p owns a deleter of type cv-unqualified Deleter (e.g.
 * if it was created with one of the constructors that take a deleter as a
 * parameter), then returns a pointer to the deleter. Otherwise, returns a null
 * pointer.
 */
template <class Deleter, class T>
Deleter *get_deleter(const shared_ptr<T> &p) TSP_NOEXCEPT {
    return std::get_deleter<Deleter>(p.get_std_shared_ptr());
}

/** \brief Compare two shared_ptr objects
 * \return lhs.get() == rhs.get()
 */
template <class T, class U>
bool operator==(const shared_ptr<T> &lhs,
                const shared_ptr<U> &rhs) TSP_NOEXCEPT {
    return lhs.get_std_shared_ptr() == rhs.get_std_shared_ptr();
}

/** \brief Compare two shared_ptr objects
 * \return !(lhs == rhs)
 */
template <class T, class U>
bool operator!=(const shared_ptr<T> &lhs,
                const shared_ptr<U> &rhs) TSP_NOEXCEPT {
    return lhs.get_std_shared_ptr() != rhs.get_std_shared_ptr();
}

/** \brief Compare two shared_ptr objects
 * \return std::less<V>()(lhs.get(), rhs.get()), where V is the composite
 * pointer type of std::shared_ptr<T>::element_type* and
 * std::shared_ptr<U>::element_type*
 */
template <class T, class U>
bool operator<(const shared_ptr<T> &lhs,
               const shared_ptr<U> &rhs) TSP_NOEXCEPT {
    return lhs.get_std_shared_ptr() < rhs.get_std_shared_ptr();
}

/** \brief Compare two shared_ptr objects
 * \return rhs < lhs
 */
template <class T, class U>
bool operator>(const shared_ptr<T> &lhs,
               const shared_ptr<U> &rhs) TSP_NOEXCEPT {
    return lhs.get_std_shared_ptr() > rhs.get_std_shared_ptr();
}

/** \brief Compare two shared_ptr objects
 * \return !(rhs < lhs)
 */
template <class T, class U>
bool operator<=(const shared_ptr<T> &lhs,
                const shared_ptr<U> &rhs) TSP_NOEXCEPT {
    return lhs.get_std_shared_ptr() <= rhs.get_std_shared_ptr();
}

/** \brief Compare two shared_ptr objects
 * \return !(lhs < rhs)
 */
template <class T, class U>
bool operator>=(const shared_ptr<T> &lhs,
                const shared_ptr<U> &rhs) TSP_NOEXCEPT {
    return lhs.get_std_shared_ptr() >= rhs.get_std_shared_ptr();
}

/** \brief Compare a shared_ptr with a null pointer
 * \return !lhs
 */
template <class T>
bool operator==(const shared_ptr<T> &lhs, std::nullptr_t rhs) TSP_NOEXCEPT {
    return lhs.get_std_shared_ptr() == rhs;
}

/** \brief Compare a shared_ptr with a null pointer
 * \return !rhs
 */
template <class T>
bool operator==(std::nullptr_t lhs, const shared_ptr<T> &rhs) TSP_NOEXCEPT {
    return lhs == rhs.get_std_shared_ptr();
}

/** \brief Compare a shared_ptr with a null pointer
 * \return (bool)lhs
 */
template <class T>
bool operator!=(const shared_ptr<T> &lhs, std::nullptr_t rhs) TSP_NOEXCEPT {
    return lhs.get_std_shared_ptr() != rhs;
}

/** \brief Compare a shared_ptr with a null pointer
 * \return (bool)rhs
 */
template <class T>
bool operator!=(std::nullptr_t lhs, const shared_ptr<T> &rhs) TSP_NOEXCEPT {
    return lhs != rhs.get_std_shared_ptr();
}

/** \brief Compare a shared_ptr with a null pointer
 * \return std::less<shared_ptr<T>::element_type*>()(lhs.get(), nullptr)
 */
template <class T>
bool operator<(const shared_ptr<T> &lhs, std::nullptr_t rhs) TSP_NOEXCEPT {
    return lhs.get_std_shared_ptr() < rhs;
}

/** \brief Compare a shared_ptr with a null pointer
 * \return std::less<shared_ptr<T>::element_type*>()(nullptr, rhs.get())
 */
template <class T>
bool operator<(std::nullptr_t lhs, const shared_ptr<T> &rhs) TSP_NOEXCEPT {
    return lhs < rhs.get_std_shared_ptr();
}

/** \brief Compare a shared_ptr with a null pointer
 * \return nullptr < lhs
 */
template <class T>
bool operator>(const shared_ptr<T> &lhs, std::nullptr_t rhs) TSP_NOEXCEPT {
    return lhs.get_std_shared_ptr() > rhs;
}

/** \brief Compare a shared_ptr with a null pointer
 * \return rhs < nullptr
 */
template <class T>
bool operator>(std::nullptr_t lhs, const shared_ptr<T> &rhs) TSP_NOEXCEPT {
    return lhs > rhs.get_std_shared_ptr();
}

/** \brief Compare a shared_ptr with a null pointer
 * \return !(nullptr < lhs)
 */
template <class T>
bool operator<=(const shared_ptr<T> &lhs, std::nullptr_t rhs) TSP_NOEXCEPT {
    return lhs.get_std_shared_ptr() <= rhs;
}

/** \brief Compare a shared_ptr with a null pointer
 * \return !(rhs < nullptr)
 */
template <class T>
bool operator<=(std::nullptr_t lhs, const shared_ptr<T> &rhs) TSP_NOEXCEPT {
    return lhs <= rhs.get_std_shared_ptr();
}

/** \brief Compare a shared_ptr with a null pointer
 * \return !(lhs < nullptr)
 */
template <class T>
bool operator>=(const shared_ptr<T> &lhs, std::nullptr_t rhs) TSP_NOEXCEPT {
    return lhs.get_std_shared_ptr() >= rhs;
}

/** \brief Compare a shared_ptr with a null pointer
 * \return !(nullptr < rhs)
 */
template <class T>
bool operator>=(std::nullptr_t lhs, const shared_ptr<T> &rhs) TSP_NOEXCEPT {
    return lhs >= rhs.get_std_shared_ptr();
}

/** \brief Inserts the value of the pointer stored in ptr into the output stream
 * os
 *
 * Equivalent to os << ptr.get().
 * \return os
 */
template <class T, class U, class V>
std::basic_ostream<U, V> &operator<<(std::basic_ostream<U, V> &os,
                                     const shared_ptr<T> &ptr) {
    os << ptr.get();
    return os;
}

#if !(!defined(__clang__) && defined(__GNUC__) &&                              \
      __GNUC__ < 5) // These operations are not supported in GCC < 5.0

/** \brief Determines whether atomic access to the shared pointer pointed-to by
 * p is lock-free.
 */
template <class T> bool atomic_is_lock_free(shared_ptr<T> const *p) {
    return atomic_is_lock_free(reinterpret_cast<std::shared_ptr<T> const *>(p));
}

/** \brief Equivalent to atomic_load_explicit(p, std::memory_order_seq_cst)
 */
template <class T> shared_ptr<T> atomic_load(const shared_ptr<T> *p) {
    return std::move(
            atomic_load(reinterpret_cast<const std::shared_ptr<T> *>(p)));
}

/** \brief Returns the shared pointer pointed-to by p.
 *
 * As with the non-specialized std::atomic_load_explicit, mo cannot be
 * std::memory_order_release or std::memory_order_acq_rel
 */
template <class T>
shared_ptr<T> atomic_load_explicit(const shared_ptr<T> *p,
                                   std::memory_order mo) {
    return std::move(atomic_load_explicit(
            reinterpret_cast<const std::shared_ptr<T> *>(p), mo));
}

/** \brief Equivalent to atomic_store_explicit(p, r, memory_order_seq_cst)
 */
template <class T> void atomic_store(shared_ptr<T> *p, shared_ptr<T> r) {
    atomic_store(reinterpret_cast<std::shared_ptr<T> *>(p),
                 r.get_std_shared_ptr());
}

/** \brief Stores the shared pointer r in the shared pointer pointed-to by p
 * atomically, effectively executing p->swap(r).
 *
 * As with the non-specialized std::atomic_store_explicit, mo cannot be
 * std::memory_order_acquire or std::memory_order_acq_rel.
 */
template <class T>
void atomic_store_explicit(shared_ptr<T> *p, shared_ptr<T> r,
                           std::memory_order mo) {
    atomic_store_explicit(reinterpret_cast<std::shared_ptr<T> *>(p),
                          r.get_std_shared_ptr(), mo);
}

/** \brief Equivalent to atomic_exchange(p, r, memory_order_seq_cst)
 */
template <class T>
shared_ptr<T> atomic_exchange(shared_ptr<T> *p, shared_ptr<T> r) {
    return std::move(atomic_exchange(reinterpret_cast<std::shared_ptr<T> *>(p),
                                     r.get_std_shared_ptr()));
}

/** \brief Stores the shared pointer r in the shared pointer pointed to by p and
 * returns the value formerly pointed-to by p, atomically.
 *
 * Effectively executes p->swap(r) and returns a copy of r after the swap.
 */
template <class T>
shared_ptr<T> atomic_exchange_explicit(shared_ptr<T> *p, shared_ptr<T> r,
                                       std::memory_order mo) {
    return std::move(
            atomic_exchange_explicit(reinterpret_cast<std::shared_ptr<T> *>(p),
                                     r.get_std_shared_ptr(), mo));
}

/** \brief Equivalent to atomic_compare_exchange_weak_explicit(p, expected,
 * desired, std::memory_order_seq_cst, std::memory_order_seq_cst)
 */
template <class T>
bool atomic_compare_exchange_weak(shared_ptr<T> *p, shared_ptr<T> *expected,
                                  shared_ptr<T> desired) {
    return atomic_compare_exchange_weak(
            reinterpret_cast<std::shared_ptr<T> *>(p),
            reinterpret_cast<std::shared_ptr<T> *>(expected),
            desired.get_std_shared_ptr());
}

/** \brief Equivalent to atomic_compare_exchange_strong_explicit(p, expected,
 * desired, std::memory_order_seq_cst, std::memory_order_seq_cst)
 */
template <class T>
bool atomic_compare_exchange_strong(shared_ptr<T> *p, shared_ptr<T> *expected,
                                    shared_ptr<T> desired) {
    return atomic_compare_exchange_strong(
            reinterpret_cast<std::shared_ptr<T> *>(p),
            reinterpret_cast<std::shared_ptr<T> *>(expected),
            desired.get_std_shared_ptr());
}

/** \brief Compares the shared pointers pointed-to by p and expected. If they
 * are equivalent (store the same pointer value, and either share ownership of
 * the same object or are both empty), assigns desired into *p using the memory
 * ordering constraints specified by success and returns true. If they are not
 * equivalent, assigns *p into *expected using the memory ordering constraints
 * specified by failure and returns false.
 */
template <class T>
bool atomic_compare_exchange_strong_explicit(shared_ptr<T> *p,
                                             shared_ptr<T> *expected,
                                             shared_ptr<T> desired,
                                             std::memory_order success,
                                             std::memory_order failure) {
    return atomic_compare_exchange_strong_explicit(
            reinterpret_cast<std::shared_ptr<T> *>(p),
            reinterpret_cast<std::shared_ptr<T> *>(expected),
            desired.get_std_shared_ptr(), success, failure);
}

/** \brief Compares the shared pointers pointed-to by p and expected. If they
 * are equivalent (store the same pointer value, and either share ownership of
 * the same object or are both empty), assigns desired into *p using the memory
 * ordering constraints specified by success and returns true. If they are not
 * equivalent, assigns *p into *expected using the memory ordering constraints
 * specified by failure and returns false, but may fail spuriously.
 */
template <class T>
bool atomic_compare_exchange_weak_explicit(shared_ptr<T> *p,
                                           shared_ptr<T> *expected,
                                           shared_ptr<T> desired,
                                           std::memory_order success,
                                           std::memory_order failure) {
    return atomic_compare_exchange_weak_explicit(
            reinterpret_cast<std::shared_ptr<T> *>(p),
            reinterpret_cast<std::shared_ptr<T> *>(expected),
            desired.get_std_shared_ptr(), success, failure);
}
#endif // atomic methods supported by compiler

} // namespace throwing

namespace std {

/** \brief Template specialization of std::hash for throwing::shared_ptr<T>
 */
template <class T> struct hash<throwing::shared_ptr<T>> {
    size_t operator()(const throwing::shared_ptr<T> &x) const {
        return std::hash<typename throwing::shared_ptr<T>::element_type *>()(
                x.get());
    }
};

} // namespace std

#include <throwing/private/clear_compiler_checks.hpp>
