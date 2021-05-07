cmake_minimum_required (VERSION 3.16)
project (ScarlettMain)
set(CMAKE_CXX_STANDARD 17)

option(NDEBUG ON)

find_package(Boost REQUIRED COMPONENTS system program_options serialization)
find_package(spdlog REQUIRED)

if(CMAKE_BUILD_TARGET|"Release")
	add_compile_options(-DNDEBUG=1)
	set(RELEASE ON)
elseif(CMAKE_BUILD_TARGET|"Debug")
	set(DEBUG ON)
	set(ASAN OFF)
endif()

if(Boost_FOUND)
	if(DEBUG) 
		include_directories(${Boost_LIBRARY_DIR_DEBUG})
	else() 
		include_directories(${Boost_LIBRARY_DIR_RELEASE}) 
	endif()
	include_directories(${Boost_INCLUDE_DIRS})
endif()


if(GCC)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0 -Wextra -Wfatal-errors -pedantic -Wsign-conversion -Wconversion")
    set(CMAKE_CXX_FLAGS "-Wall -Werror")
	set(CMAKE_CXX_FLAGS_RELEASE "-O2")
elseif(MSVC)
	set(CMAKE_CXX_FLAGS "/EHa /MP")
	set(CMAKE_CXX_FLAGS_DEBUG "/DEBUG:FULL /Od")
	if(ASAN)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CSS_FLAGS_DEBUG} /fsanitize=address")
	endif()
	set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Oi")
endif()

add_executable(ScarlettArchiver main.cpp ParseOptions.cpp ScarlettCore.cpp)

target_link_libraries(ScarlettArchiver LINK_PRIVATE Scarlett spdlog::spdlog spdlog::spdlog_header_only ${Boost_LIBRARIES})
