cmake_minimum_required(VERSION 3.16)
project(CppShift)
set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)
find_package(nlohmann_json  REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

option(NDEBUG ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0")
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wfatal-errors")

if(NDEBUG)
    add_compile_option(-DNDEBUG=1)
endif()


add_library(CppShift STATIC BasicRequest.cpp PushShift.cpp)

target_link_libraries(CppShift LINK_PUBLIC CURL::libcurl nlohmann_json::nlohmann_json Boost::program_options)
