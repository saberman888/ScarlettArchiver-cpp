cmake_minimum_required(VERSION 3.16)
project(Scarlett CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS program_options serialization)
find_package(spdlog REQUIRED)
find_package(tinyxml2 REQUIRED)

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

message(STATUS "Cloning submodule: cpprestsdk")

set(git_cmd "git")
set(git_arg "submodule update --init")

execute_process(COMMAND ${git_cmd} ${git_arg})

set(CPPREST_EXCLUDE_WEBSOCKETS CACHE BOOL ON)
set(CPPREST_EXCLUDE_COMPRESSION CACHE BOOL ON)
set(CPPREST_EXCLUDE_BROTLI CACHE BOOL ON)
set(BUILD_SHARED_LIBS CACHE BOOL OFF)
set(BUILD_TESTS CACHE BOOL OFF)
set(BUILD_SAMPLES CACHE BOOL OFF)
set(CPPREST_INSTALL_HEADERS CACHE BOOL OFF)
set(CPPREST_INSTALL CACHE BOOL OFF)

add_subdirectory("submodule/cpprestsdk")

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

target_compile_definitions(
        Scarlett
        PRIVATE
        SPDLOG_FMT_EXTERNAL=ON
        CRT_SECURE_NO_WARNINGS
        _TURN_OFF_PLATFORM_STRING
)

target_link_libraries(
	Scarlett 
	INTERFACE  
	spdlog::spdlog
	cpprest
	tinyxml2::tinyxml2
	${Boost_LIBRARIES})
