cmake_minimum_required(VERSION 2.8)
project(litehtml)

#dynamic runtime on UWP
if ( ${CMAKE_SYSTEM_NAME} STREQUAL "WindowsStore" )
    set ( DYNAMIC_CRT true )
endif ()

if(NOT MSVC)
	set(CMAKE_CXX_FLAGS "-std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DDEBUG -g")
	set(CMAKE_C_FLAGS_DEBUG "-std=c99 -O0 -DDEBUG -g")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	set(CMAKE_C_FLAGS_RELEASE "-std=c99 -O3")
endif()

if ( WIN32 )
    #static runtime by default
    if ( DYNAMIC_CRT )
        set ( CRT_TYPE_DEBUG "/MDd" )
        set ( CRT_TYPE_RELEASE "/MD" )
    else ()
        set ( CRT_TYPE_DEBUG "/MTd" )
        set ( CRT_TYPE_RELEASE "/MT" )
    endif ()
    
    set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CRT_TYPE_DEBUG}" )
    set ( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CRT_TYPE_RELEASE}" )
    
endif ()

set ( INCLUDE_PATTERNS "include/*.h" )
set ( HEADERS_PATTERNS "src/*.h" )
set ( SOURCES_PATTERNS "src/*.c*" )

file ( GLOB_RECURSE INCLUDE_FILES ${INCLUDE_PATTERNS} )
file ( GLOB_RECURSE HEADER_FILES ${HEADERS_PATTERNS} )
file ( GLOB_RECURSE SOURCE_FILES ${SOURCES_PATTERNS} )

include_directories ( "include" "src/gumbo" )
add_library(litehtml STATIC ${INCLUDE_FILES} ${HEADER_FILES} ${SOURCE_FILES})
