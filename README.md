[![Build Status](https://travis-ci.org/rockdreamer/throwing_ptr.svg?branch=master)](https://travis-ci.org/rockdreamer/throwing_ptr) [![Build status](https://ci.appveyor.com/api/projects/status/6hv5yx4cj6sy5asn?svg=true)](https://ci.appveyor.com/project/rockdreamer/throwing-ptr)

# Throwing smart pointers

A header only library wrapping [std::shared_ptr](http://en.cppreference.com/w/cpp/memory/shared_ptr) and [std::unique_ptr](http://en.cppreference.com/w/cpp/memory/unique_ptr) that throws an exception when a wrapped null pointer is dereferenced

## Rationale

Dereferencing a null standard smart pointer is undefined behaviour, probably leading to a program crash.
This library aims to fully replicate the behaviour of the wrapped standard smart pointers with the exception (sic) that an exception will be thrown when the ->, * and [] operators are called. Such exceptions can be caught and handled in an application specific manner (for example by showing an error pop up)

Situations where this library may be useful:
- Legacy code that used a custom smart pointer class which threw on dereference: this library provides a gradual path towards moving to the standard smart pointers. Such code may depend on the throwing behaviour, making a direct migration bug prone. 
- Programs where resilience to null dereference bugs is important, such as GUIs

There is an obvious cost to using this library in that every dereference will add a branch. In these cases the library exposes the underlying standard smart pointer and the user may opt to use that in hot code paths where attention has been taken not to dereference null pointers.

## Design decisions

The library relies on the underlying implementation of std::shared_ptr and std::unique_ptr for all functionality, including participation in overload resolution. While this exposes users to slight differences in behaviour across platforms and compilers, the upside is that it's easier to give access to the stored std pointers and when the user decides to no longer rely on these classes and move to the std implementations, they will get less surprising results.

Exceptions are throwing::make_unique and throwing::unique_ptr::operator<< which are provided by the implementation.

## Using the library

### Accessing the functionality

throwing_ptr is a header-only library. You can copy the include throwing_ptr folder in your source code or add the include directory in your include path and start using it right away. 

### Examples

Example use 1:
```c++
#include <throwing/shared_ptr.hpp>
auto pointer = throwing::make_shared<int>();
```

Example use 2:
```c++
#include <throwing/unique_ptr.hpp>
auto pointer = throwing::make_unique<SomeClass>(some_parameter);
```

Example use 3 (exception handling):
```c++
#include <throwing/unique_ptr.hpp>

// Initialise a null pointer
throwing::unique_ptr<int> null_int_ptr; 
auto non_null_float_ptr = throwing::make_unique<float>(0);
try {
    *non_null_float_ptr;
    (*null_int_ptr)++; // dereference of null pointer, throws exception
} catch (const throwing::null_ptr_exception<float> &) {
    // It is possible to handle exceptions differently based on the
    // type of object handled by the pointer
    // This block will not be called as it only catches
    // dereferences of int pointers
} catch (const throwing::base_null_ptr_exception &e) {
    // what() returns a generic message
    std::cout << e.what() << std::endl;
    // what_type() returns platform-specific message
    // indicating the type
    std::cout << e.what_type() << std::endl; 
}

```

## Testing the library

The library comes with a thorough unit testing suite, based on [catch 1.9](https://github.com/catchorg/Catch2), [CMake](http://www.cmake.org) and [Conan.io](https://conan.io).

To run it, install cmake and conan and build the package for your environment.

## Documentation

All methods have [doxygen documentation](https://rockdreamer.github.io/throwing_ptr/), largely based on the high quality documentation of the standard library from [cppreference](http://en.cppreference.com)

## License

The library is distributed under the Boost Software License, Version 1.0.

The documentation is licensed under Creative Commons Attribution-Sharealike 3.0 Unported License (CC-BY-SA) and by the GNU Free Documentation License (GFDL) (unversioned, with no invariant sections,front-cover texts, or back-cover texts).

## Further reading

- Customising std::shared_ptr or boost::shared_ptr to throw an exception on NULL dereference on [Stack Overflow](https://stackoverflow.com/questions/12452565/customising-stdshared-ptr-or-boostshared-ptr-to-throw-an-exception-on-null-d?rq=1)
- How to make shared_ptr wrapper class work with make_shared on [Stack Overflow](https://stackoverflow.com/questions/29074577/how-to-make-shared-ptr-wrapper-class-work-with-make-shared?noredirect=1&lq=1)
