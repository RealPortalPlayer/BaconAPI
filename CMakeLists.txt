cmake_minimum_required(VERSION 3.26)
project(BaconAPI C)

set(CMAKE_C_STANDARD 11)

option(BA_ALLOW_DEBUG_LOGS "Always allow debug logs, even for release builds" OFF)
option(BA_DISABLE_HACKS "Disable operating system specific hacks" OFF)
option(BA_SINGLE_THREADED "Don't allow the creation of a second thread" OFF)
option(BA_DISABLE_TEST_PROJECTS "Don't compile any of the test projects" OFF)

include(cmake/Strip.cmake)
include(cmake/Options.cmake)

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
        source/Number.c
        source/Configuration.c
        source/Memory.c)

target_include_directories(BaconAPI PUBLIC include)
ba_strip_static(BaconAPI)
ba_apply_compiler_options(BaconAPI)

if(NOT BA_DISABLE_TEST_PROJECTS)
    add_subdirectory(test)
endif()
