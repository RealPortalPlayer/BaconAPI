// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdlib.h>

#include "BaconAPI/Number.h"

#define BA_NUMBER_STRING_CONVERT(type, to, min, max) \
type BA_Number_StringTo ## to(const char* string, char** endPointer, BA_Boolean* isError) { \
    char* pointer;                                   \
    long parsedValue = strtol(string, &pointer, 0);  \
    BA_Boolean errored = pointer[0] != '\0' || parsedValue < min || parsedValue > max; \
    if (endPointer != NULL)                          \
        *endPointer = pointer;                       \
    if (isError != NULL)                             \
        *isError = errored;                          \
    return !errored ? (type) parsedValue : 0;        \
}

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_NUMBER_STRING_CONVERT(char, Character, CHAR_MIN, CHAR_MAX)
BA_NUMBER_STRING_CONVERT(unsigned char, UnsignedCharacter, 0, UCHAR_MAX)
BA_NUMBER_STRING_CONVERT(short, Short, SHRT_MIN, SHRT_MAX)
BA_NUMBER_STRING_CONVERT(unsigned short, UnsignedShort, 0, USHRT_MAX)
BA_NUMBER_STRING_CONVERT(int, Integer, INT_MIN, INT_MAX)
BA_NUMBER_STRING_CONVERT(unsigned, Unsigned, 0, UINT_MAX)
BA_CPLUSPLUS_SUPPORT_GUARD_END()
