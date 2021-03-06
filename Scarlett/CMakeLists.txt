# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.16)
project ("Scarlett")
set(CMAKE_CXX_STANDARD 17)

find_package(cpprestsdk REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options serialization)
find_package(OpenMP REQUIRED)
find_package(spdlog REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(NDEBUG ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0")
set(CMAKE_CXX_FLAGS "-I . -Wall -Wfatal-errors")
set(_TURN_OFF_PLATFORM_STRING ON)

if(NDEBUG)
    add_compile_options(-DNDEBUG=1)
endif()

include_directories(${CMAKE_SOURCE_DIR})
add_executable(Scarlett exceptions.cpp Logger.cpp Misc.cpp StringOps.cpp SubredditMetadata.cpp Images/Imgur.cpp Reddit/Linkable.cpp Reddit/Postable.cpp Reddit/Comment.cpp Reddit/TextPost.cpp Reddit/Link.cpp Reddit/Video.cpp Reddit/Galleries.cpp Reddit/Subreddit.cpp ParseOptions.cpp Scarlett.cpp main.cpp)

target_link_libraries(Scarlett LINK_PUBLIC libVun spdlog::spdlog OpenMP::OpenMP_CXX cpprestsdk::cpprest Boost::program_options Boost::serialization)
