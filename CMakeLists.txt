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

enable_cxx_compiler_flag_if_supported("-Weverything")
enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")
enable_cxx_compiler_flag_if_supported("-Werror")

# do not warn about known problems in googletest 1.8
enable_cxx_compiler_flag_if_supported("-Wno-undef")
enable_cxx_compiler_flag_if_supported("-Wno-c++98-compat-pedantic")
enable_cxx_compiler_flag_if_supported("-Wno-c++98-compat")
enable_cxx_compiler_flag_if_supported("-Wno-missing-noreturn")
enable_cxx_compiler_flag_if_supported("-Wno-shift-sign-overflow")
enable_cxx_compiler_flag_if_supported("-Wno-used-but-marked-unused")
enable_cxx_compiler_flag_if_supported("-Wno-global-constructors")
enable_cxx_compiler_flag_if_supported("-Wno-weak-vtables")
enable_cxx_compiler_flag_if_supported("-Wno-error-undefined")
enable_cxx_compiler_flag_if_supported("-Wno-padded")

include_directories("include")

enable_testing()
if(WIN32)
   if(MSVC_VERSION)
       if(MSVC_VERSION GREATER_EQUAL 1910)
        add_definitions(-DGTEST_LANG_CXX11=1)
       endif()
   endif()
endif()

add_executable(shared_ptr_dereference tests/shared_ptr_dereference.cpp)
target_link_libraries(shared_ptr_dereference ${CONAN_LIBS})
add_test(NAME shared_ptr_dereference COMMAND shared_ptr_dereference)

