cmake_minimum_required(VERSION 3.16)
project(Vun)
set(CMAKE_CXX_STANDARD 17)
find_package(cpprestsdk REQUIRED)

option(NDEBUG ON)
add_compile_options(-D_TURN_OFF_PLATFORM_STRING=1)

if(GCC)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0")
    set(CMAKE_CXX_FLAGS "-Wall -Werror -Wfatal-errors")
endif()

if(NDEBUG)
    add_compile_options(-DNDEBUG=1)
endif()


add_library(libVun STATIC helpers.cpp Reddit.cpp PushShift.cpp Internal/IntervalTracker.cpp)

target_link_libraries(libVun LINK_PUBLIC cpprestsdk::cpprest)
