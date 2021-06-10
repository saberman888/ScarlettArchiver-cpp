cmake_minimum_required(VERSION 3.20)
project(Scarlett_Testing)
set(CMAKE_CXX_STANDARD 17)

find_package(tinyxml2 REQUIRED)

add_executable(oauth2_tests)

target_sources( 
	oauth2_tests
	PRIVATE
	oauth2_tests/main.cpp
)

target_precompile_headers(
	oauth2_tests
	PRIVATE
	scarlett_pch.hpp
)

target_compile_definitions(
	oauth2_tests
	PRIVATE
	_TURN_OFF_PLATFORM_STRING
)

target_include_directories(
	oauth2_tests
	PRIVATE
	${SCARLETT_INCLUDES}
)

target_link_libraries(
	oauth2_tests
	PRIVATE
	tinyxml2::tinyxml2
	Scarlett_tests
	cpprest
	GTest::gmock_main
	GTest::gtest_main
)
