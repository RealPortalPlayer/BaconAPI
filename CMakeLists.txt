cmake_minimum_required(VERSION 3.26)
project(BaconAPI C)

set(CMAKE_C_STANDARD 11)

option(BA_ALLOW_DEBUG_LOGS "Always allow debug logs, even for release builds" OFF)
option(BA_DISABLE_HACKS "Disable operating system specific hacks" OFF)
option(BA_SINGLE_THREADED "Don't allow the creation of a second thread" OFF)
option(BA_DISABLE_TEST_PROJECTS "Don't compile any of the test projects" OFF)
option(BA_DISABLE_FREETYPE "Don't use Freetype. Does nothing on non-Unix" OFF)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC" AND NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    message(WARNING "MSVC on non-Windows platform. Make sure your CMake toolchain is valid")
endif()

include(cmake/Strip.cmake)
include(cmake/Options.cmake)

if(UNIX)
    find_package(X11)

    if(NOT BA_DISABLE_FREETYPE)
        find_package(Freetype)
    endif()

    if(NOT X11_FOUND)
        message(WARNING "Failed to find X11. BA_MessageBox_Open won't work properly")
    endif()

    if(NOT FREETYPE_FOUND)
        message(WARNING "Failed to find Freetype. BA_MessageBox_Open might have visual issues")
    endif()
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
        source/Number.c
        source/Configuration.c
        source/Memory.c
        source/Debugging/Stack.c
        source/MessageBox.c)

target_include_directories(BaconAPI PUBLIC include)
ba_strip_static(BaconAPI)
ba_apply_compiler_options(BaconAPI)

if(NOT BA_DISABLE_TEST_PROJECTS)
    add_subdirectory(test)
endif()
