# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.16)
project ("Scarlett")
set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options serialization)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(NDEBUG ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0")
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wfatal-errors")

if(NDEBUG)
    add_compile_options(-DNDEBUG=1)
endif()


add_executable(Scarlett ParseOptions.cpp Scarlett.cpp main.cpp)

target_link_libraries(Scarlett LINK_PUBLIC CURL::libcurl CppShift nlohmann_json::nlohmann_json Boost::program_options Boost::serialization)
