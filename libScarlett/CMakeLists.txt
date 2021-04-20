cmake_minimum_required(VERSION 3.16)
project(libScarlett)
set(CMAKE_CXX_STANDARD 17)

find_package(cpprestsdk REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options serialization)
find_package(spdlog REQUIRED)

option(NDEBUG ON)
add_compile_options(-D_TURN_OFF_PLATFORM_STRING=1)

if(MSVC)
	add_compile_options(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(GCC)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0")
    set(CMAKE_CXX_FLAGS "-Wall -Werror -Wfatal-errors")
endif()

if(NDEBUG)
    add_compile_options(-DNDEBUG=1)
endif()

if(Boost_FOUND)
	include_directories(${BOOST_INCLUDE_DIRS})
	include_directories(${BOOST_LIBRARY_DIRS})
endif()



set(Internal Internal/exceptions.cpp Internal/Helpers.cpp Internal/IntervalTracker.cpp Internal/Logger.cpp)
set(Client Client/Reddit.cpp Client/PushShift.cpp)
set(Reddit Reddit/BaseTypes/Postable.cpp Reddit/BaseTypes/Linkable.cpp Reddit/BaseTypes/Link.cpp Reddit/BaseTypes/TextPost.cpp Reddit/Comment.cpp Reddit/Galleries.cpp Reddit/SelfPost.cpp Reddit/Video.cpp Reddit/SubredditMetadata.cpp Reddit/Subreddit.cpp)
set(Media Media/Content.cpp Media/Imgur.cpp)
set(Libraries spdlog::spdlog spdlog::spdlog_header_only cpprestsdk::cpprest)

add_library(libScarlett STATIC ${Internal} ${Media} ${Client} ${Reddit} PushShift.cpp)
target_link_libraries(libScarlett LINK_PUBLIC  ${Internal} ${Media})
