cmake_minimum_required(VERSION 2.8)
project(litehtml)

if(NOT MSVC)
	set(CMAKE_CXX_FLAGS "-std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DDEBUG -g")
	set(CMAKE_C_FLAGS_DEBUG "-std=c99 -O0 -DDEBUG -g")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	set(CMAKE_C_FLAGS_RELEASE "-std=c99 -O3")
endif()

set ( HEADERS_PATTERNS "src/*.h" )
set ( SOURCES_PATTERNS "src/*.c*" )

file ( GLOB_RECURSE HEADER_FILES ${HEADERS_PATTERNS} )
file ( GLOB_RECURSE SOURCE_FILES ${SOURCES_PATTERNS} )

add_library(litehtml STATIC ${HEADER_FILES} ${SOURCE_FILES})

if(WIN32)
     target_include_directories(litehtml PUBLIC src/gumbo)
endif()

