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
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(int8_t, Character, CHAR_MIN, CHAR_MAX, strtol)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(uint8_t, UnsignedCharacter, 0, UCHAR_MAX, strtoul)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(int16_t, Short, SHRT_MIN, SHRT_MAX, strtol)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(uint16_t, UnsignedShort, 0, USHRT_MAX, strtoul)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(int32_t, Integer, INT_MIN, INT_MAX, strtol)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(uint32_t, Unsigned, 0, UINT_MAX, strtoul)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(int64_t, Long, LONG_MIN, LONG_MAX, strtol)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(uint64_t, UnsignedLong, 0, ULONG_MAX, strtoul)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(intmax_t, LongLong, LLONG_MIN, LLONG_MAX, strtoll)
BA_NUMBER_STRING_CONVERT_IMPLEMENTATION(uintmax_t, UnsignedLongLong, 0, ULLONG_MAX, strtoull)
BA_CPLUSPLUS_SUPPORT_GUARD_END()
