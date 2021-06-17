cmake_minimum_required(VERSION 3.20)
project(Scarlett_OAuth2_Tests)
set(CMAKE_CXX_STANDARD 17)

find_package(tinyxml2 REQUIRED)
find_package(GTest REQUIRED)

add_executable(scarlett_password_grant)
add_executable(scarlett_authorization)

target_sources( 
	scarlett_password_grant
	PRIVATE
	oauth2_tests/scarlett_password_grant.cpp
)

target_precompile_headers(
	scarlett_password_grant
	PRIVATE
	scarlett_pch.hpp
)

target_compile_definitions(
	scarlett_password_grant
	PRIVATE
	_TURN_OFF_PLATFORM_STRING
)

target_include_directories(
	scarlett_password_grant
	PRIVATE
	${SCARLETT_INCLUDES}
)

target_link_libraries(
	scarlett_password_grant
	PRIVATE
	tinyxml2::tinyxml2
	Scarlett_tests
	cpprest
	GTest::gmock_main
	GTest::gtest_main
)


target_sources( 
	scarlett_authorization
	PRIVATE
	oauth2_tests/scarlett_authorization.cpp
)

target_precompile_headers(
	scarlett_authorization
	PRIVATE
	scarlett_pch.hpp
)

target_compile_definitions(
	scarlett_authorization
	PRIVATE
	_TURN_OFF_PLATFORM_STRING
)

target_include_directories(
	scarlett_authorization
	PRIVATE
	${SCARLETT_INCLUDES}
)

target_link_libraries(
	scarlett_authorization
	PRIVATE
	tinyxml2::tinyxml2
	Scarlett_tests
	cpprest
	GTest::gmock_main
	GTest::gtest_main
)
