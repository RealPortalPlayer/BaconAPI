cmake_minimum_required(VERSION 3.26)
project(BaconAPI C)

set(CMAKE_C_STANDARD 99)

option(BA_ALLOW_DEBUG_LOGS "Always allow debug logs, even for release builds" OFF)
option(BA_DISABLE_HACKS "Disable operating system specific hacks" OFF)

include(cmake/Strip.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_definitions(DEBUG BA_ALLOW_DEBUG_LOGS)
elseif(BA_ALLOW_DEBUG_LOGS)
    add_compile_definitions(BA_ALLOW_DEBUG_LOGS)
endif()

set(BA_SOURCE_FILES)

if(NOT BA_DISABLE_HACKS)
    if(WIN32)
        set(BA_SOURCE_FILES source/Hacks/Windows.c)
    endif()
endif()

add_library(BaconAPI STATIC
        source/ArgumentHandler.c
        source/Logger.c
        source/ANSI.c
        source/User.c
        source/Thread.c
        source/String.c
        source/Storage/DynamicArray.c
        source/Storage/DynamicDictionary.c
        source/Debugging/Assert.c
        ${BA_SOURCE_FILES}
        source/Number.c)

if(BA_ALLOW_DEBUG_LOGS)
    target_compile_definitions(BaconAPI PUBLIC BA_ALLOW_DEBUG_LOGS)
endif()

target_include_directories(BaconAPI PUBLIC include)

add_subdirectory(test/normal)
add_subdirectory(test/threads)