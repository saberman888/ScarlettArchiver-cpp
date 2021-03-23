# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.16)
project ("Scarlett")
set(CMAKE_CXX_STANDARD 17)

option(NDEBUG ON)

find_package(cpprestsdk REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options serialization)
find_package(spdlog REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(${Boost_LIBRARY_DIRS})
endif()

if(GCC)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0")
    set(CMAKE_CXX_FLAGS "-I . -Wall -Wfatal-errors")
endif()

if(NDEBUG)
    add_compile_options(-DNDEBUG=1)
endif()

add_compile_options(-D_TURN_OFF_PLATFORM_STRING=1)
set(sourceFiles Core/exceptions.cpp Core/Logger.cpp Core/Misc.cpp Core/StringOps.cpp Reddit/SubredditMetadata.cpp Media/Imgur.cpp Reddit/BaseTypes/Linkable.cpp Reddit/BaseTypes/Postable.cpp Reddit/Comment.cpp Reddit/BaseTypes/TextPost.cpp Reddit/BaseTypes/Link.cpp Reddit/Video.cpp Reddit/SelfPost.cpp Reddit/Galleries.cpp Reddit/Subreddit.cpp Core/ParseOptions.cpp Core/Scarlett.cpp)

add_executable(Scarlett ${sourceFiles} Core/main.cpp)

target_link_libraries(Scarlett LINK_PUBLIC libVun spdlog::spdlog cpprestsdk::cpprest ${BOOST_LIBRARIES})
