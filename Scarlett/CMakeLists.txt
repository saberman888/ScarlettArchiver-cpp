cmake_minimum_required(VERSION 3.16)
project(Scarlett CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS program_options serialization)
find_package(spdlog REQUIRED)
find_package(tinyxml2 REQUIRED)
find_package(GTest)


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

set(CPPREST_EXCLUDE_WEBSOCKETS INTERNAL CACHE BOOL ON)
set(CPPREST_EXCLUDE_COMPRESSION INTERNAL CACHE BOOL ON)
set(CPPREST_EXCLUDE_BROTLI INTERNAL CACHE BOOL ON)
set(CPPREST_INSTALL_HEADERS INTERNAL CACHE BOOL OFF)
set(CPPREST_INSTALL INTERNAL CACHE BOOL OFF)
set(Boost_USE_STATIC_LIBS INTERNAL CACHE BOOL ON)

set(BUILD_SHARED_LIBS CACHE BOOL OFF)
set(BUILD_TESTS CACHE BOOL OFF)
set(BUILD_SAMPLES CACHE BOOL OFF)

message(NOTICE "Cloning submodule: cpprestsdk")
set(git_cmd git submodule update --init)
execute_process(COMMAND ${git_cmd})
add_subdirectory(${Scarlett_SOURCE_DIR}/submodule/cpprestsdk)

include_directories("${Scarlett_SOURCE_DIR}/include")
set(SCARLETT_TESTS CACHE BOOL OFF)

set(SCARLETT_COMPILE_DEFINITIONS
	SPDLOG_FMT_EXTERNAL
	CRT_SECURE_NO_WARNINGS
	_TURN_OFF_PLATFORM_STRING)

set(SCARLETT_SOURCES
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
	src/Client/Reddit.cpp)

set(SCARLETT_LINK_LIBRARIES
	spdlog::spdlog_header_only
	cpprest
	tinyxml2::tinyxml2
	${Boost_LIBRARIES}
)

set(SCARLETT_INCLUDES
	${Scarlett_SOURCE_DIR}/include
	${Scarlett_SOURCE_DIR}/submodule/cpprestsdk/Release/include
	)

if(Boost_FOUND)
	if(DEBUG) 
		set(SCARLETT_INCLUDES ${SCARLETT_INCLUDES} ${Boost_LIBRARY_DIR_DEBUG})
	else() 
		set(SCARLETT_INCLUDES ${SCARLETT_INCLUDES} ${Boost_LIBRARY_DIR_RELEASE}) 
	endif()
	set(SCARLETT_INCLUDES ${SCARLETT_INCLUDES} ${Boost_INCLUDE_DIRS})
endif()

add_library(Scarlett STATIC)


target_include_directories(
	Scarlett
	PUBLIC
	${SCARLETT_INCLUDES}
)

target_sources(
	Scarlett PRIVATE 
	${SCARLETT_SOURCES}
)

target_compile_definitions(
        Scarlett
        PRIVATE
		${SCARLETT_COMPILE_DEFINITIONS}
)

target_link_libraries(
	Scarlett 
	INTERFACE  
	${SCARLETT_LINK_LIBRARIES})

if(SCARLETT_TESTS)
	add_library(Scarlett_tests STATIC)

	target_sources(
		Scarlett_tests
		PRIVATE
		${SCARLETT_SOURCES}
	)

	target_include_directories(
		Scarlett_tests
		PUBLIC
		${SCARLETT_INCLUDES}
	)

	target_compile_definitions(
		Scarlett_tests
		PRIVATE
		${SCARLETT_COMPILE_DEFINITIONS}
	)

	target_link_libraries(
		Scarlett_tests
		INTERFACE
		GTest::gmock
		GTest::gtest
		${SCARLETT_LINK_LIBRARIES}
	)

	add_subdirectory("tests")

	enable_testing()

	add_test(NAME scarlett_oauth2_tests COMMAND ${CMAKE_CTEST_COMMAND} --build-and-test "${Scarlett_SOURCE_DIR}/tests/oauth2_tests" "${CMAKE_CURRENT_BINARY_DIR}/oauth2_tests" --build-generator "${CMAKE_GENERATOR}" --test-command "${CMAKE_CTEST_COMMAND}")

endif()

