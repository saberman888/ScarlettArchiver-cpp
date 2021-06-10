cmake_minimum_required (VERSION 3.16)
project (ScarlettMain CXX)
set(CMAKE_CXX_STANDARD 17)
find_package(Boost REQUIRED COMPONENTS system program_options)


set(SCARLETT_USE_ASAN CACHE BOOL OFF "Debug with MSVC's address sanitizer. For this to be effective, this must be built with MSVC")
set(SCARLETT_MAIN_COMPILE_DEFINITIONS "")

if(Boost_FOUND)
	if(DEBUG) 
		set(SCARLETT_MAIN_INCLUDES ${SCARLETT_MAIN_INCLUDES} ${Boost_LIBRARY_DIR_DEBUG})
	else() 
		set(SCARLETT_MAIN_INCLUDES ${SCARLETT_MAIN_INCLUDES} ${Boost_LIBRARY_DIR_RELEASE}) 
	endif()
	set(SCARLETT_MAIN_LINK_LIBRARIES ${SCARLETT_MAIN_LINK_LIBRARIES} ${Boost_LIBRARIES})
	set(SCARLETT_MAIN_INCLUDES ${Boost_INCLUDE_DIR})
endif()

if(CMAKE_BUILD_TARGET|"Release")
	set(SCARLETT_MAIN_COMPILE_DEFINITIONS NDEBUG)
	set(RELEASE ON)
	set(SCARLETT_USE_ASAN CACHE BOOL OFF)
endif()


if(GCC)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0 -Wextra -Wfatal-errors -pedantic -Wsign-conversion -Wconversion")
    set(CMAKE_CXX_FLAGS "-Wall -Werror")
	set(CMAKE_CXX_FLAGS_RELEASE "-O2")
elseif(MSVC)
	set(CMAKE_CXX_FLAGS "/EHa /MP")
	set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Zf /JMC /Od /Od")
	if(SCARLETT_USE_ASAN)
		set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "/fsanitize=address")
	endif()
	set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Oi")
endif()

add_executable(
	ScarlettArchiver
	main.cpp 
	ParseOptions.cpp 
	ScarlettCore.cpp
)

target_compile_definitions(
	ScarlettArchiver
	PRIVATE
	${SCARLETT_MAIN_COMPILE_DEFINITIONS}
)

target_include_directories(
	ScarlettArchiver
	PRIVATE
	${Scarlett_SOURCE_DIR}/include
	${Boost_INCLUDE_DIR}
)

target_link_libraries(
	ScarlettArchiver 
	PRIVATE 
	Scarlett
	${Boost_LIBRARIES}
)
