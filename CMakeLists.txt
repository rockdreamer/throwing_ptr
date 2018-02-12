project(throwing_ptr_tests CXX)
cmake_minimum_required(VERSION 3.8)

#Honour the CMAKE_CXX_STANDARD in try_compile
cmake_policy(SET CMP0067 NEW)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if(NOT MSVC AND NOT DEFINED ENV{CXX_STANDARD})
    message(FATAL_ERROR "CXX_STANDARD environment variable not set")
endif()

if(DEFINED ENV{CXX_STANDARD})
    set(CMAKE_CXX_STANDARD $ENV{CXX_STANDARD})
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

check_cxx_source_compiles("
#include <memory>

int main(){
    int* p = new int[10];
    std::shared_ptr<int[]> sp(p);
    return *p = sp[0];
}
" HAVE_SHARED_PTR_TO_ARRAY)

add_executable( compile_it 
	tests/compile_it.cpp
	include/throwing/shared_ptr.hpp
	include/throwing/unique_ptr.hpp
	include/throwing/null_ptr_exception.hpp
	include/throwing/private/compiler_checks.hpp
	include/throwing/private/clear_compiler_checks.hpp
)

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
    enable_cxx_compiler_flag_if_supported("-Wno-zero-as-null-pointer-constant")
    enable_cxx_compiler_flag_if_supported("-Wno-padded")
    # do not warn about unused variables in tests
    enable_cxx_compiler_flag_if_supported("-Wno-unused")
    enable_cxx_compiler_flag_if_supported("-Wno-unused-member-function")
endif()

set(TESTS
    shared_ptr_access
    shared_ptr_assignment
    shared_ptr_cast
    shared_ptr_comparison
    shared_ptr_construction
    shared_ptr_hash
    shared_ptr_make_shared
    shared_ptr_ordering
    shared_ptr_ostream
    shared_ptr_reset
    shared_ptr_swap
    unique_ptr_access
    unique_ptr_construction
)

if(HAVE_SHARED_PTR_TO_ARRAY)
    list(APPEND TESTS shared_ptr_to_array )
endif()
# GCC < 5.0 has no support for atomic_ operations on std::shared_ptr
# See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=57250
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" 
    AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 5.0)
    message(STATUS "Skipping tests on atomic_* because GCC ${CMAKE_CXX_COMPILER_VERSION} doesn't support them")
else()
    list(APPEND TESTS shared_ptr_atomic )
endif()

foreach(TEST ${TESTS})
    add_executable(${TEST} tests/${TEST}.cpp)
    target_link_libraries(${TEST} ${CONAN_LIBS})
    add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()

set(COMPILE_FAIL_TESTS
    unique_ptr_s_operator
)
if(NOT HAVE_SHARED_PTR_TO_ARRAY)
    list(APPEND COMPILE_FAIL_TESTS shared_ptr_to_array)
endif()

foreach(test_name ${COMPILE_FAIL_TESTS})
    add_executable(must_fail_${test_name} tests/compile_fail/${test_name}.cpp)
    set_target_properties(must_fail_${test_name} PROPERTIES
        EXCLUDE_FROM_ALL TRUE
        EXCLUDE_FROM_DEFAULT_BUILD TRUE)
    add_test(NAME must_fail_${test_name}
        COMMAND ${CMAKE_COMMAND} --build . --target must_fail_${test_name} --config $<CONFIGURATION>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    set_tests_properties(must_fail_${test_name} PROPERTIES WILL_FAIL TRUE)
endforeach()
