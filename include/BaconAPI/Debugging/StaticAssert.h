// Purpose: Compiler asserts
// Created on: 1/14/24 @ 4:31 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconAPI/Internal/Compiler.h"

#if defined(__STDC__) && defined(__STDC_VERSION__)
#   if BA_COMPILER_STANDARD_VERSION >= 2011
#       define BA_STATIC_ASSERT(check, message) _Static_assert(check, message)
#   else // FIXME: This doesn't work in certain situations
#       define BA_STATIC_ASSERT_INTERNAL2(check, message) check ## message
#       define BA_STATIC_ASSERT_INTERNAL(check, message) BA_STATIC_ASSERT_INTERNAL2(check, message)
#       define BA_STATIC_ASSERT(check, message) enum {BA_STATIC_ASSERT_INTERNAL(BACON_API_SAYS_HELLO_IF_YOUR_READING_THIS_A_STATIC_ASSERT_FAILED_ON_LINE_, __LINE__) = 1 / (message && (check))}
#   endif
#else
#   define BA_STATIC_ASSERT(check, message)
#endif

#define BA_STATIC_ASSERT_LOOKUP_TABLE_CHECK(table, size) BA_STATIC_ASSERT(sizeof(table) / sizeof(table[0]) == size, "Lookup table size mismatch")
