//          Copyright Claudio Bantaloukas 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/** \file throwing/null_ptr_exception.hpp 
 * \brief Exceptions thrown by smart pointers in namespace throwing
 */

#pragma once
#include <exception>
#include <string>
#include <typeinfo>

namespace throwing {

/** \brief Base class thrown upon dereferencing a null shared_ptr.
 *
 * Use to catch all such errors
 */
class base_null_ptr_exception : public std::logic_error {
public:
    base_null_ptr_exception() : std::logic_error("Dereference of nullptr") {}
    virtual std::string what_type() const { return ""; }
};

/** \brief Concrete class thrown upon dereferencing a null shared_ptr.
 *
 * Use to catch dereferencing of specific types
 */
template <typename T>
class null_ptr_exception : public base_null_ptr_exception {
public:
    virtual std::string what_type() const {
        std::string result("Dereferenced nullptr of type ");
        result.append(typeid(T).name());
        return result;
    }
};

} // namespace throwing
