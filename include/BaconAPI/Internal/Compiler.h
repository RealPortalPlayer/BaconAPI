// Purpose: Compiler specific information
// Created on: 1/14/24 @ 8:24 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Hacks.h"

#define BA_COMPILER_GCC 0
#define BA_COMPILER_MSVC 0
#define BA_COMPILER_UNKNOWN 0
#define BA_COMPILER_STANDARD_VERSION 0
#define BA_COMPILER_CLANG 0
#define BA_COMPILER_TINYC 0
#define BA_COMPILER_NAME "Unknown"

#if defined(__clang__)
#   undef BA_COMPILER_CLANG
#   undef BA_COMPILER_NAME
#   define BA_COMPILER_CLANG 1
#   define BA_COMPILER_NAME "Clang"
#elif defined(__GNUC__)
#   undef BA_COMPILER_GCC
#   undef BA_COMPILER_NAME
#   define BA_COMPILER_GCC 1
#   define BA_COMPILER_NAME "GCC"
#elif defined(_MSC_VER) // TODO: Getting MSVC version seems more complicated than other compilers. Typical Microsoft
#   undef BA_COMPILER_MSVC
#   undef BA_COMPILER_NAME
#   define BA_COMPILER_MSVC 1
#   define BA_COMPILER_NAME "MSVC"
#elif defined(__TINYC__) // TODO: Detect Tiny C compiler version
#   undef BA_COMPILER_TINYC
#   undef BA_COMPILER_NAME
#   define BA_COMPILER_TINYC 1
#   define BA_COMPILER_NAME "Tiny C"
#else
#   undef BA_COMPILER_UNKNOWN
#   define BA_COMPILER_UNKNOWN 1
#   warning "Your compiler is unknown"
#endif

#if defined(__STDC_VERSION__)
#   undef BA_COMPILER_STANDARD_VERSION
#   if __STDC_VERSION__ == 199409L
#       define BA_COMPILER_STANDARD_VERSION 1994
#   elif  __STDC_VERSION__ == 199901L
#       define BA_COMPILER_STANDARD_VERSION 1999
#   elif __STDC_VERSION__ == 201112L
#       define BA_COMPILER_STANDARD_VERSION 2011
#   elif __STDC_VERSION__ == 201710L
#       define BA_COMPILER_STANDARD_VERSION 2018
#   elif __STDC_VERSION__ == 202311L
#       define BA_COMPILER_STANDARD_VERSION 2023
#   else
#       define BA_COMPILER_STANDARD_VERSION 2018
#       warning "Unknown Standard Libraray version. Treating as C18"
#   endif
#elif defined(__STDC__)
#   undef BA_COMPILER_STANDARD_VERSION
#   define BA_COMPILER_STANDARD_VERSION 1989
#else
#   warning "Standard Library version is not defined?"
#endif

#define BA_COMPILER_VERSION 0
#define BA_COMPILER_VERSION_STRING BA_HACKS_STRINGIFY(BA_COMPILER_VERSION)
#define BA_COMPILER_GET_VERSION(major, minor, patch) (((major) << 24) + ((minor) << 16) + (patch))

#if BA_COMPILER_GCC
#   undef BA_COMPILER_VERSION
#   undef BA_COMPILER_VERSION_STRING
#   define BA_COMPILER_VERSION BA_COMPILER_GET_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#   define BA_COMPILER_VERSION_STRING BA_HACKS_STRINGIFY(__GNUC__) "." BA_HACKS_STRINGIFY(__GNUC_MINOR__) "." BA_HACKS_STRINGIFY(__GNUC_PATCHLEVEL__)
#elif BA_COMPILER_CLANG // FIXME: Clang and AppleClang reports different versions. In example, Clang 3.1 and AppleClang 4.0 are the same. I guess they had to "think different"
#   undef BA_COMPILER_VERSION
#   undef BA_COMPILER_VERSION_STRING
#   define BA_COMPILER_VERSION BA_COMPILER_GET_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#   define BA_COMPILER_VERSION_STRING __clang_version__
#endif