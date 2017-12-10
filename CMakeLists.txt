project(throwing_ptr CXX)
cmake_minimum_required(VERSION 3.8)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if(DEFINED ENV{CXX_STANDARD})
    set(CMAKE_CXX_STANDARD ENV{CXX_STANDARD})
else()
    set(CMAKE_CXX_STANDARD 11)
endif()
message(STATUS "Building tests for C++${CMAKE_CXX_STANDARD}")

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

