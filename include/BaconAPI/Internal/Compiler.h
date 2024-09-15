// Purpose: Compiler specific information
// Created on: 1/14/24 @ 8:24 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#define BA_COMPILER_GCC 0
#define BA_COMPILER_MSVC 0
#define BA_COMPILER_UNKNOWN 0
#define BA_COMPILER_STANDARD_VERSION 0
#define BA_COMPILER_CLANG 0
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
#elif defined(_MSC_VER)
#   undef BA_COMPILER_MSVC
#   undef BA_COMPILER_NAME
#   define BA_COMPILER_MSVC 1
#   define BA_COMPILER_NAME "MSVC"
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

#ifdef __cplusplus
#   define BA_COMPILER_VERSION BA_CXX_COMPILER_VERSION
#else
#   define BA_COMPILER_VERSION BA_C_COMPILER_VERSION
#endif
