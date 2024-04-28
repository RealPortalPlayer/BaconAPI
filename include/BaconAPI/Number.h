// Purpose: Makes using numbers a little easier
// Created on: 1/14/24 @ 1:03 AM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <limits.h>

#include "BaconAPI/Internal/CPlusPlusSupport.h"
#include "BaconAPI/Internal/Boolean.h"

#define BA_NUMBER_STRING_CONVERT(type, to) type BA_Number_StringTo ## to(const char* string, char** endPointer, BA_Boolean* isError, const char* errorMessage, type defaultValue)

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_NUMBER_STRING_CONVERT(char, Character);
BA_NUMBER_STRING_CONVERT(unsigned char, UnsignedCharacter);
BA_NUMBER_STRING_CONVERT(short, Short);
BA_NUMBER_STRING_CONVERT(unsigned short, UnsignedShort);
BA_NUMBER_STRING_CONVERT(int, Integer);
BA_NUMBER_STRING_CONVERT(unsigned, Unsigned);
BA_NUMBER_STRING_CONVERT(long, Long);
BA_NUMBER_STRING_CONVERT(unsigned long, UnsignedLong);
BA_NUMBER_STRING_CONVERT(long long, LongLong);
BA_NUMBER_STRING_CONVERT(unsigned long long, UnsignedLongLong);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BA_NUMBER_MIN(number, minimum) ((number) > (minimum) ? (number) : (minimum))
#define BA_NUMBER_MAX(number, maximum) ((number) < (maximum) ? (number) : (maximum))
#define BA_NUMBER_MIN_MAX(number, minimum, maximum) BA_NUMBER_MAX(BA_NUMBER_MIN(number, minimum), maximum)
#define BA_NUMBER_CONVERT_INTERNAL(number, result, minimum, maximum) ((result) BA_NUMBER_MIN_MAX(number, minimum, maximum))
#define BA_NUMBER_TO_CHARACTER(number) BA_NUMBER_CONVERT_INTERNAL(number, char, CHAR_MIN, CHAR_MAX)
#define BA_NUMBER_TO_UNSIGNED_CHARACTER(number) BA_NUMBER_CONVERT_INTERNAL(number, unsigned char, 0, UCHAR_MAX)
#define BA_NUMBER_TO_SHORT(number) BA_NUMBER_CONVERT_INTERNAL(number, short, SHRT_MIN, SHRT_MAX)
#define BA_NUMBER_TO_UNSIGNED_SHORT(number) BA_NUMBER_CONVERT_INTERNAL(number, unsigned short , 0, USHRT_MAX)
#define BA_NUMBER_TO_INTEGER(number) BA_NUMBER_CONVERT_INTERNAL(number, int, INT_MIN, INT_MAX)
#define BA_NUMBER_TO_UNSIGNED(number) BA_NUMBER_CONVERT_INTERNAL(number, unsigned, 0, UINT_MAX)
#define BA_NUMBER_TO_LONG(number) BA_NUMBER_CONVERT_INTERNAL(number, long, LONG_MIN, LONG_MAX)
#define BA_NUMBER_TO_UNSIGNED_LONG(number) BA_NUMBER_CONVERT_INTERNAL(number, unsigned long, 0, ULONG_MAX)
#define BA_NUMBER_TO_LONG_LONG(number) BA_NUMBER_CONVERT_INTERNAL(number, long long, LLONG_MIN, LLONG_MAX)
#define BA_NUMBER_TO_UNSIGNED_LONG_LONG(number) BA_NUMBER_CONVERT_INTERNAL(number, unsigned long long, 0, ULONG_LONG_MAX)

#define BA_NUMBER_TO_SIGNED_CHARACTER(number) BA_NUMBER_TO_CHARACTER(number)
#define BA_NUMBER_TO_SHORT_INTEGER(number) BA_NUMBER_TO_SHORT(number)
#define BA_NUMBER_TO_SIGNED_SHORT(number) BA_NUMBER_TO_SHORT(number)
#define BA_NUMBER_TO_SIGNED_SHORT_INTEGER(number) BA_NUMBER_TO_SHORT(number)
#define BA_NUMBER_TO_UNSIGNED_SHORT_INTEGER(number) BA_NUMBER_TO_UNSIGNED_SHORT(number)
#define BA_NUMBER_TO_SIGNED(number) BA_NUMBER_TO_INTEGER(number)
#define BA_NUMBER_TO_SIGNED_INTEGER(number) BA_NUMBER_TO_INTEGER(number)
#define BA_NUMBER_TO_UNSIGNED_INTEGER(number) BA_NUMBER_TO_UNSIGNED(number)
#define BA_NUMBER_TO_LONG_INTEGER(number) BA_NUMBER_TO_LONG(number)
#define BA_NUMBER_TO_SIGNED_LONG(number) BA_NUMBER_TO_LONG(number)
#define BA_NUMBER_TO_SIGNED_LONG_INTEGER(number) BA_NUMBER_TO_LONG(number)
#define BA_NUMBER_TO_UNSIGNED_LONG_INTEGER(number) BA_NUMBER_TO_UNSIGNED_LONG(number)
#define BA_NUMBER_TO_LONG_LONG_INTEGER(number) BA_NUMBER_TO_LONG_LONG(number)
#define BA_NUMBER_TO_SIGNED_LONG_LONG(number) BA_NUMBER_TO_LONG_LONG(number)
#define BA_NUMBER_TO_SIGNED_LONG_LONG_INTEGER(number) BA_NUMBER_TO_LONG_LONG(number)
#define BA_NUMBER_TO_UNSIGNED_LONG_LONG_INTEGER(number) BA_NUMBER_TO_UNSIGNED_LONG_LONG(number)
