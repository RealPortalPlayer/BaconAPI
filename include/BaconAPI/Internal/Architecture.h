// Purpose: CPU architecture information
// Created on: 9/19/24 @ 5:47 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#define BA_ARCHITECTURE_TYPE_X86 1978
#define BA_ARCHITECTURE_TYPE_ARM 1985
#define BA_ARCHITECTURE_TYPE_MIPS 1985_2
#define BA_ARCHITECTURE_TYPE_POWERPC 1992_2
#define BA_ARCHITECTURE_TYPE_ITANIUM 2001

#define BA_ARCHITECTURE_X8 8
#define BA_ARCHITECTURE_X16 16
#define BA_ARCHITECTURE_X32 32
#define BA_ARCHITECTURE_X64 64

#define BA_ARCHITECTURE_TYPE 0
#define BA_ARCHITECTURE 0
#define BA_ARCHITECTURE_TYPE_NAME "Unknown"

#if defined(_M_I86)
#   undef BA_ARCHITECTURE_TYPE
#   undef BA_ARCHITECTURE
#   undef BA_ARCHITECTURE_TYPE_NAME
#   define BA_ARCHITECTURE_TYPE BA_ARCHITECTURE_TYPE_X86
#   define BA_ARCHITECTURE BA_ARCHITECTURE_X16
#   define BA_ARCHITECTURE_TYPE_NAME "x86"
#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(__IA32__) || defined(_M_IX86) || defined(__X86__) || defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__) || defined(__386)
#   undef BA_ARCHITECTURE_TYPE
#   undef BA_ARCHITECTURE
#   undef BA_ARCHITECTURE_TYPE_NAME
#   define BA_ARCHITECTURE_TYPE BA_ARCHITECTURE_TYPE_X86
#   define BA_ARCHITECTURE BA_ARCHITECTURE_X32
#   define BA_ARCHITECTURE_TYPE_NAME "x86"
#elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#   undef BA_ARCHITECTURE_TYPE
#   undef BA_ARCHITECTURE
#   undef BA_ARCHITECTURE_TYPE_NAME
#   define BA_ARCHITECTURE_TYPE BA_ARCHITECTURE_TYPE_X86
#   define BA_ARCHITECTURE BA_ARCHITECTURE_X64
#   define BA_ARCHITECTURE_TYPE_NAME "x86"
#elif defined(__aarch64__) || defined(_M_ARM64)
#   undef BA_ARCHITECTURE_TYPE
#   undef BA_ARCHITECTURE
#   undef BA_ARCHITECTURE_TYPE_NAME
#   define BA_ARCHITECTURE_TYPE BA_ARCHITECTURE_TYPE_ARM
#   define BA_ARCHITECTURE BA_ARCHITECTURE_X64
#   define BA_ARCHITECTURE_TYPE_NAME "ARM"
#elif defined(__arm__) || defined(__thumb__) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) || defined(_ARM) || defined(_M_ARM) || defined(_M_ARMT) || defined(__arm)
#   undef BA_ARCHITECTURE_TYPE
#   undef BA_ARCHITECTURE
#   undef BA_ARCHITECTURE_TYPE_NAME
#   define BA_ARCHITECTURE_TYPE BA_ARCHITECTURE_TYPE_ARM
#   define BA_ARCHITECTURE BA_ARCHITECTURE_X32
#   define BA_ARCHITECTURE_TYPE_NAME "ARM"
#elif defined(__mips__) || defined(mips) || defined(__mips) || defined(__MIPS__)
#   undef BA_ARCHITECTURE_TYPE
#   undef BA_ARCHITECTURE
#   undef BA_ARCHITECTURE_TYPE_NAME
#   define BA_ARCHITECTURE_TYPE BA_ARCHITECTURE_TYPE_MIPS
#   define BA_ARCHITECTURE BA_ARCHITECTURE_X64
#   define BA_ARCHITECTURE_TYPE_NAME "MIPS"
#elif defined(__powerpc64__) || defined(__ppc64__) | defined(_ARCH_PPC64)
#   undef BA_ARCHITECTURE_TYPE
#   undef BA_ARCHITECTURE
#   undef BA_ARCHITECTURE_TYPE_NAME
#   define BA_ARCHITECTURE_TYPE BA_ARCHITECTURE_TYPE_POWERPC
#   define BA_ARCHITECTURE BA_ARCHITECTURE_X64
#   define BA_ARCHITECTURE_TYPE_NAME "PowerPC"
#elif defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(ARCH_PPC) || defined(_M_PPC) || defined(__ppc)
#   undef BA_ARCHITECTURE_TYPE
#   undef BA_ARCHITECTURE
#   undef BA_ARCHITECTURE_TYPE_NAME
#   define BA_ARCHITECTURE_TYPE BA_ARCHITECTURE_TYPE_POWERPC
#   define BA_ARCHITECTURE BA_ARCHITECTURE_X32
#   define BA_ARCHITECTURE_TYPE_NAME "PowerPC"
#elif defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(__ia64) || defined(_M_IA64) || defined(__itanium__)
#   undef BA_ARCHITECTURE_TYPE
#   undef BA_ARCHITECTURE
#   undef BA_ARCHITECTURE_TYPE_NAME
#   define BA_ARCHITECTURE_TYPE BA_ARCHITECTURE_TYPE_ITANIUM
#   define BA_ARCHITECTURE BA_ARCHITECTURE_X64
#   define BA_ARCHITECTURE_TYPE_NAME "Itanium"
#endif
