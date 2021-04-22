cmake_minimum_required(VERSION 3.16)
project(libScarlett)
set(CMAKE_CXX_STANDARD 17)

find_package(cpprestsdk REQUIRED)
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
endif()

if(GCC)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0")
    set(CMAKE_CXX_FLAGS "-Wall -Werror -Wfatal-errors")
elseif(MSVC)
	set(CMAKE_CXX_FLAGS " /MP")
	set(CMAKE_CXX_FLAGS_DEBUG "/Zs /Zi /Zf /W3 /Od /JMC /fsanitize")
endif()

if(Boost_FOUND)
	if(DEBUG) 
		include_directories(${Boost_LIBRARY_DIR_DEBUG})
	else() 
		include_directories(${Boost_LIBRARY_DIR_RELEASE}) 
	endif()
	include_directories(${Boost_INCLUDE_DIRS})
endif()



set(Internal Internal/exceptions.cpp Internal/Helpers.cpp Internal/IntervalTracker.cpp Internal/Logger.cpp)
set(Client Client/Reddit.cpp Client/PushShift.cpp)
set(Reddit Reddit/BaseTypes/Postable.cpp Reddit/BaseTypes/Linkable.cpp Reddit/BaseTypes/Link.cpp Reddit/BaseTypes/TextPost.cpp Reddit/Comment.cpp Reddit/Galleries.cpp Reddit/SelfPost.cpp Reddit/Video.cpp Reddit/SubredditMetadata.cpp Reddit/Subreddit.cpp)
set(Media Media/Content.cpp Media/Imgur.cpp)
set(Libraries spdlog::spdlog spdlog::spdlog_header_only cpprestsdk::cpprest tinyxml2::tinyxml2 ${Boost_LIBRARIES})

add_library(libScarlett STATIC ${Internal} ${Media} ${Client} ${Reddit})
target_link_libraries(libScarlett LINK_PUBLIC  ${Libraries})
