cmake_minimum_required(VERSION 3.16)
project(Scarlett CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(cpprestsdk CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options serialization)
find_package(spdlog REQUIRED)
find_package(tinyxml2 REQUIRED)

add_compile_options(-D_TURN_OFF_PLATFORM_STRING=1)
if(MSVC)
	add_compile_options(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(CMAKE_BUILD_TARGET|"Release")
	add_compile_options(-DNDEBUG=1)
	set(RELEASE ON)
elseif(CMAKE_BUILD_TARGET|"Debug")
	set(DEBUG ON)
	set(ASAN OFF)
endif()

if(GCC)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0 -Wextra -Wfatal-errors -pedantic -Wsign-conversion -Wconversion")
    set(CMAKE_CXX_FLAGS "-Wall -Werror")
	set(CMAKE_CXX_FLAGS_RELEASE "-O2")
elseif(MSVC)
	set(CMAKE_CXX_FLAGS "/EHa /MP")
	set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Zf /JMC /Od")
	if(ASAN)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /fsanitize=address")
	endif()
	set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Oi")
endif()

if(Boost_FOUND)
	if(DEBUG) 
		include_directories(${Boost_LIBRARY_DIR_DEBUG})
	else() 
		include_directories(${Boost_LIBRARY_DIR_RELEASE}) 
	endif()
	include_directories(${Boost_INCLUDE_DIRS})
endif()

add_library(Scarlett STATIC)


target_include_directories(
	Scarlett
	PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

target_sources(
	Scarlett PRIVATE 
	src/Internal/Logger.cpp
	src/Internal/Exceptions.cpp
	src/Internal/Helpers.cpp
	src/Media/Content.cpp
	src/Media/Imgur.cpp
	src/Reddit/BaseTypes/Postable.cpp
	src/Reddit/BaseTypes/Linkable.cpp
	src/Reddit/BaseTypes/Link.cpp
	src/Reddit/BaseTypes/TextPost.cpp
	src/Reddit/Comment.cpp
	src/Reddit/Galleries.cpp
	src/Reddit/SelfPost.cpp
	src/Reddit/Video.cpp
	src/Reddit/SubredditMetadata.cpp
	src/Reddit/Subreddit.cpp
	src/Client/IntervalTracker.cpp
	src/Client/PushShift.cpp
	src/Client/Reddit.cpp
)

target_link_libraries(
	Scarlett 
	INTERFACE  
	spdlog::spdlog
	cpprestsdk::cpprest
	tinyxml2::tinyxml2
	${Boost_LIBRARIES})
