// Purpose: Makes using numbers a little easier
// Created on: 1/14/24 @ 1:03 AM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <limits.h>
#include <stdint.h>

#include "BaconAPI/Internal/CPlusPlusSupport.h"
#include "BaconAPI/Internal/Boolean.h"

#define BA_NUMBER_STRING_CONVERT(type, to) type BA_Number_StringTo ## to(const char* string, char** endPointer, BA_Boolean* isError, const char* errorMessage, type defaultValue)

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_NUMBER_STRING_CONVERT(int8_t, Character);
BA_NUMBER_STRING_CONVERT(uint8_t, UnsignedCharacter);
BA_NUMBER_STRING_CONVERT(int16_t, Short);
BA_NUMBER_STRING_CONVERT(uint16_t, UnsignedShort);
BA_NUMBER_STRING_CONVERT(int32_t, Integer);
BA_NUMBER_STRING_CONVERT(uint32_t, Unsigned);
BA_NUMBER_STRING_CONVERT(int64_t, Long);
BA_NUMBER_STRING_CONVERT(uint64_t, UnsignedLong);
BA_NUMBER_STRING_CONVERT(intmax_t, LongLong);
BA_NUMBER_STRING_CONVERT(uintmax_t, UnsignedLongLong);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BA_NUMBER_MIN(number, minimum) ((number) > (minimum) ? (number) : (minimum))
#define BA_NUMBER_MAX(number, maximum) ((number) < (maximum) ? (number) : (maximum))
#define BA_NUMBER_MIN_MAX(number, minimum, maximum) BA_NUMBER_MAX(BA_NUMBER_MIN(number, minimum), maximum)
#define BA_NUMBER_CONVERT_INTERNAL(number, result, minimum, maximum) ((result) BA_NUMBER_MIN_MAX(number, minimum, maximum))
#define BA_NUMBER_TO_CHARACTER(number) BA_NUMBER_CONVERT_INTERNAL(number, int8_t, CHAR_MIN, CHAR_MAX)
#define BA_NUMBER_TO_UNSIGNED_CHARACTER(number) BA_NUMBER_CONVERT_INTERNAL(number, uint8_t, 0, UCHAR_MAX)
#define BA_NUMBER_TO_SHORT(number) BA_NUMBER_CONVERT_INTERNAL(number, int16_t, SHRT_MIN, SHRT_MAX)
#define BA_NUMBER_TO_UNSIGNED_SHORT(number) BA_NUMBER_CONVERT_INTERNAL(number, uint16_t, 0, USHRT_MAX)
#define BA_NUMBER_TO_INTEGER(number) BA_NUMBER_CONVERT_INTERNAL(number, int32_t, INT_MIN, INT_MAX)
#define BA_NUMBER_TO_UNSIGNED(number) BA_NUMBER_CONVERT_INTERNAL(number, uint32_t, 0, UINT_MAX)
#define BA_NUMBER_TO_LONG(number) BA_NUMBER_CONVERT_INTERNAL(number, int64_t, LONG_MIN, LONG_MAX)
#define BA_NUMBER_TO_UNSIGNED_LONG(number) BA_NUMBER_CONVERT_INTERNAL(number, uint64_t, 0, ULONG_MAX)
#define BA_NUMBER_TO_LONG_LONG(number) BA_NUMBER_CONVERT_INTERNAL(number, intmax_t, LLONG_MIN, LLONG_MAX)
#define BA_NUMBER_TO_UNSIGNED_LONG_LONG(number) BA_NUMBER_CONVERT_INTERNAL(number, uintmax_t, 0, ULONG_LONG_MAX)

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
