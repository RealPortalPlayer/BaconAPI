cmake_minimum_required(VERSION 3.26)
project(BaconAPI C)

set(CMAKE_C_STANDARD 11)

option(BA_ALLOW_DEBUG_LOGS "Always allow debug logs, even for release builds" OFF)
option(BA_DISABLE_HACKS "Disable operating system specific hacks" OFF)
option(BA_SINGLE_THREADED "Don't allow the creation of a second thread" OFF)
option(BA_DISABLE_TEST_PROJECTS "Don't compile any of the test projects" OFF)
option(BA_DISABLE_FREETYPE "Don't use Freetype. Does nothing on non-Unix" OFF)
option(BA_DISABLE_X11 "Don't use X11. Does nothing on non-Unix" OFF)
set(BA_ASAN_SANITIZER "" CACHE STRING "Set what ASAN sanitizer to use")
option(BA_NATIVE_SHUT_UP "Don't show the native binary warning, does nothing on Windows" OFF)
option(BA_FAST_MATH "Use fast math" OFF)
option(BA_LOGGER_INITIALIZER_DEBUG_LOGS "Show debug logs on initialization" OFF)
option(BA_LOGGER_FORCE_ENABLE_ALL_LEVELS "Allow printing all log types" OFF)
option(BA_LOGGER_DISABLED "Disable the logging system" OFF)
option(BA_LOGGER_DISABLE_INVALID_TYPE_MESSAGE "Don't print 'invalid log type'" OFF)
option(BA_UNSTABLE_OPTIMIZATION "Enables O3 optimizations" OFF)

if(("${CMAKE_BUILD_TYPE}" STREQUAL "Native" OR "${CMAKE_BUILD_TYPE}" STREQUAL "NatWithDebInfo" OR "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeNat") AND NOT MSVC AND NOT BA_NATIVE_SHUT_UP)
    message(WARNING "Build type is native. Output binaries might only work on your computer")
endif()

if("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC" AND NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    message(WARNING "MSVC on non-Windows platform. Make sure your CMake toolchain is valid")
endif()

include(cmake/Strip.cmake)
include(cmake/Options.cmake)

if(UNIX)
    if(NOT BA_DISABLE_X11)
        find_package(X11)
    endif ()

    if(NOT BA_DISABLE_FREETYPE)
        find_package(Freetype)
    endif()

    if(NOT X11_FOUND AND NOT BA_DISABLE_X11)
        message(WARNING "Failed to find X11. BA_MessageBox_Open won't work properly")
    endif()

    if(NOT FREETYPE_FOUND AND NOT BA_DISABLE_FREETYPE)
        message(WARNING "Failed to find Freetype. BA_MessageBox_Open might have visual issues")
    endif()
endif()

set(BA_SOURCE_FILES)

if(NOT BA_DISABLE_HACKS)
    if(WIN32)
        set(BA_SOURCE_FILES source/Hacks/Windows.c
                include/BaconAPI/Prompt.h
                source/Prompt.c)
    endif()
endif()

add_library(BaconAPI STATIC
        source/StringImplementation.h
        
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
        source/MessageBox.c
        source/OperatingSystem.c
        source/StringImplementation.c
        source/WideString.c
        source/StringManager.c
        source/Translations.c
        source/Prompt.c)

target_include_directories(BaconAPI PUBLIC include)
ba_strip_static(BaconAPI)
ba_apply_compiler_options(BaconAPI)

if(NOT BA_DISABLE_TEST_PROJECTS)
    add_subdirectory(test)
endif()
