// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdlib.h>
#include <BaconAPI/Logger.h>

#include "BaconAPI/Number.h"

#define BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(type, to, min, max, function) \
BA_NUMBER_STRING_CONVERT(type, to) {                                          \
    char* pointer;                                                            \
    long parsedValue = function(string, &pointer, 0);                         \
    BA_Boolean errored = pointer[0] != '\0' || parsedValue < min || parsedValue > max; \
    if (endPointer != NULL)                                                   \
        *endPointer = pointer;                                                \
    if (isError != NULL)                                                      \
        *isError = errored;                                                   \
    if (errored && errorMessage != NULL)                                      \
        BA_LOGGER_ERROR("%s", errorMessage);                                  \
    return !errored ? (type) parsedValue : defaultValue;                      \
}

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(char, Character, CHAR_MIN, CHAR_MAX, strtol)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(unsigned char, UnsignedCharacter, 0, UCHAR_MAX, strtoul)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(short, Short, SHRT_MIN, SHRT_MAX, strtol)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(unsigned short, UnsignedShort, 0, USHRT_MAX, strtoul)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(int, Integer, INT_MIN, INT_MAX, strtol)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(unsigned, Unsigned, 0, UINT_MAX, strtoul)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(long, Long, LONG_MIN, LONG_MAX, strtol)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(unsigned long, UnsignedLong, 0, ULONG_MAX, strtoul)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(long long, LongLong, LLONG_MIN, LLONG_MAX, strtoll)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(unsigned long long, UnsignedLongLong, 0, ULLONG_MAX, strtoull)
BA_CPLUSPLUS_SUPPORT_GUARD_END()
