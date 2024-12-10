// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

// Purpose: OS specific definitions.
// Created on: 4/1/22 @ 7:25 PM

#pragma once

#include "Internal/CPlusPlusSupport.h"
#include "Internal/Boolean.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    /**
     * Always "Windows" on Windows
     */
    char* systemName;
    char* release;

    /**
     * Is the host 64-bits. Will obviously always be true on 64-bit binaries.
     * @note Unrelated to the architecture of the binary. Checkout Architecture.h for that
     */
    BA_Boolean is64bits;
} BA_OperatingSystem_Version;

void BA_OperatingSystem_GetVersion(BA_OperatingSystem_Version* version);

void BA_OperatingSystem_Free(BA_OperatingSystem_Version* version);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BA_OPERATINGSYSTEM_NAME "Unknown"
#define BA_OPERATINGSYSTEM_WINDOWS 0
#define BA_OPERATINGSYSTEM_APPLE 0
#define BA_OPERATINGSYSTEM_POSIX_COMPLIANT 0
#define BA_OPERATINGSYSTEM_IPHONE_SIMULATOR 0
#define BA_OPERATINGSYSTEM_MAC_CATALYST 0
#define BA_OPERATINGSYSTEM_IPHONE 0
#define BA_OPERATINGSYSTEM_MAC 0
#define BA_OPERATINGSYSTEM_APPLE_UNKNOWN 0
#define BA_OPERATINGSYSTEM_UNKNOWN 0
#define BA_OPERATINGSYSTEM_LINUX 0
#define BA_OPERATINGSYSTEM_UNIX 0
#define BA_OPERATINGSYSTEM_SERENITY 0
#define BA_OPERATINGSYSTEM_EMSCRIPTEN 0

#if defined(WIN32)
#   undef BA_OPERATINGSYSTEM_WINDOWS
#   undef BA_OPERATINGSYSTEM_NAME
#   define BA_OPERATINGSYSTEM_WINDOWS 1
#   define BA_OPERATINGSYSTEM_NAME "Windows"
#elif defined(__APPLE__)
#   include <TargetConditionals.h>
#   undef BA_OPERATINGSYSTEM_APPLE
#   undef BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   define BA_OPERATINGSYSTEM_APPLE 1
#   define BA_OPERATINGSYSTEM_POSIX_COMPLIANT 1
#   if TARGET_IPHONE_SIMULATOR
#       undef BA_OPERATINGSYSTEM_IPHONE_SIMULATOR
#       undef BA_OPERATINGSYSTEM_NAME
#       define BA_OPERATINGSYSTEM_IPHONE_SIMULATOR 1
#       define BA_OPERATINGSYSTEM_NAME "iOS Simulator"
#   elif TARGET_OS_MACCATALYST
#       undef BA_OPERATINGSYSTEM_MAC_CATALYST
#       undef BA_OPERATINGSYSTEM_NAME
#       define BA_OPERATINGSYSTEM_MAC_CATALYST 1
#       define BA_OPERATINGSYSTEM_NAME "Mac Catalyst"
#   elif TARGET_OS_IPHONE
#       undef BA_OPERATINGSYSTEM_IPHONE
#       undef BA_OPERATINGSYSTEM_NAME
#       define BA_OPERATINGSYSTEM_IPHONE 1
#       define BA_OPERATINGSYSTEM_NAME "iOS"
#   elif TARGET_OS_MAC
#       undef BA_OPERATINGSYSTEM_MAC
#       undef BA_OPERATINGSYSTEM_NAME
#       define BA_OPERATINGSYSTEM_MAC 1
#       define BA_OPERATINGSYSTEM_NAME "macOS"
#   else
#       undef BA_OPERATINGSYSTEM_APPLE_UNKNOWN
#       undef BA_OPERATINGSYSTEM_UNKNOWN
#       define BA_OPERATINGSYSTEM_APPLE_UNKNOWN 1
#       define BA_OPERATINGSYSTEM_UNKNOWN 1
#       warning "Your Apple device is currently unknown. There might be some issues."
#   endif
#elif defined(__linux__)
#   undef BA_OPERATINGSYSTEM_LINUX
#   undef BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   undef BA_OPERATINGSYSTEM_NAME
#   define BA_OPERATINGSYSTEM_LINUX 1
#   define BA_OPERATINGSYSTEM_POSIX_COMPLIANT 1
#   define BA_OPERATINGSYSTEM_NAME "GNU/Linux"
#elif defined(__serenity__)
#   undef BA_OPERATINGSYSTEM_SERENITY
#   undef BA_OPERATINGSYSTEM_NAME
#   undef BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   define BA_OPERATINGSYSTEM_SERENITY 1
#   define BA_OPERATINGSYSTEM_POSIX_COMPLIANT 1
#   define BA_OPERATINGSYSTEM_NAME "SerenityOS"
#elif defined(__EMSCRIPTEN__)
#   undef BA_OPERATINGSYSTEM_EMSCRIPTEN
#   undef BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   undef BA_OPERATINGSYSTEM_NAME
#   define BA_OPERATINGSYSTEM_EMSCRIPTEN 1
#   define BA_OPERATINGSYSTEM_POSIX_COMPLIANT 1
#   define BA_OPERATINGSYSTEM_NAME "Emscripten"
#elif defined(__unix__)
#   undef BA_OPERATINGSYSTEM_UNIX
#   undef BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   undef BA_OPERATINGSYSTEM_NAME
#   define BA_OPERATINGSYSTEM_UNIX 1
#   define BA_OPERATINGSYSTEM_POSIX_COMPLIANT 1
#   define BA_OPERATINGSYSTEM_NAME "Unix"
#else
#   undef BA_OPERATINGSYSTEM_UNKNOWN
#   define BA_OPERATINGSYSTEM_UNKNOWN 1
#   warning "Your Operating System is unknown. This will cause issues."
#endif
