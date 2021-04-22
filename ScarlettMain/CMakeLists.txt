cmake_minimum_required (VERSION 3.16)
project (ScarlettMain)
set(CMAKE_CXX_STANDARD 17)

option(NDEBUG ON)

find_package(cpprestsdk REQUIRED)
find_package(Boost REQUIRED COMPONENTS system program_options serialization)
find_package(spdlog REQUIRED)


if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

if(GCC)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0")
    set(CMAKE_CXX_FLAGS "-I . -Wall -Wfatal-errors")
endif()

if(MSVC)
    add_compile_options(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(NDEBUG)
    add_compile_options(-DNDEBUG=1)
endif()

add_compile_options(-D_TURN_OFF_PLATFORM_STRING=1)

add_executable(Scarlett main.cpp ParseOptions.cpp ScarlettCore.cpp)

target_link_libraries(Scarlett LINK_PRIVATE libScarlett spdlog::spdlog spdlog::spdlog_header_only cpprestsdk::cpprest ${BOOST_LIBRARIES})
