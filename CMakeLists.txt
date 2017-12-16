project(throwing_ptr_tests CXX)
cmake_minimum_required(VERSION 3.5)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if(DEFINED ENV{CXX_STANDARD})
    set(CMAKE_CXX_STANDARD ENV{CXX_STANDARD})
else()
    set(CMAKE_CXX_STANDARD 11)
endif()
message(STATUS "Building tests for C++${CMAKE_CXX_STANDARD}")

include(CheckCXXCompilerFlag)

function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()

if(MSVC)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4 /WX" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
    endif()
else()
    enable_cxx_compiler_flag_if_supported("-Weverything")
    enable_cxx_compiler_flag_if_supported("-Wall")
    enable_cxx_compiler_flag_if_supported("-Wextra")
    enable_cxx_compiler_flag_if_supported("-pedantic")
    enable_cxx_compiler_flag_if_supported("-Werror")
endif()

include_directories("include")

add_executable(compile_it tests/compile_it.cpp)

enable_testing()
if(WIN32)
   if(MSVC_VERSION)
       if(MSVC_VERSION GREATER_EQUAL 1910)
        add_definitions(-DGTEST_LANG_CXX11=1)
       endif()
   endif()
endif()


if(NOT MSVC)
    # do not warn about known problems in googletest 1.8
    enable_cxx_compiler_flag_if_supported("-Wno-undef")
    enable_cxx_compiler_flag_if_supported("-Wno-c++98-compat-pedantic")
    enable_cxx_compiler_flag_if_supported("-Wno-c++98-compat")
    enable_cxx_compiler_flag_if_supported("-Wno-missing-noreturn")
    enable_cxx_compiler_flag_if_supported("-Wno-shift-sign-overflow")
    enable_cxx_compiler_flag_if_supported("-Wno-used-but-marked-unused")
    enable_cxx_compiler_flag_if_supported("-Wno-global-constructors")
    enable_cxx_compiler_flag_if_supported("-Wno-weak-vtables")
    enable_cxx_compiler_flag_if_supported("-Wno-error=deprecated")
    enable_cxx_compiler_flag_if_supported("-Wno-error=zero-as-null-pointer-constant")
    enable_cxx_compiler_flag_if_supported("-Wno-padded")
    # do not warn about unused variables in tests
    enable_cxx_compiler_flag_if_supported("-Wno-unused")
    enable_cxx_compiler_flag_if_supported("-Wno-unused-member-function")
endif()

set(TESTS
    shared_ptr_construction
    shared_ptr_assignment
    shared_ptr_reset
    shared_ptr_swap
    shared_ptr_access
    shared_ptr_ordering
    shared_ptr_make_shared
)

foreach(TEST ${TESTS})
    add_executable(${TEST} tests/${TEST}.cpp)
    target_link_libraries(${TEST} ${CONAN_LIBS})
    add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()

