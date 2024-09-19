// Purpose: Shared string and wide string implementation
// Created on: 9/18/24 @ 1:13 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <wchar.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "BaconAPI/Internal/CPlusPlusSupport.h"
#include "BaconAPI/Internal/Boolean.h"
#include "BaconAPI/Storage/DynamicArray.h"
#include "BaconAPI/Storage/DynamicDictionary.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    union {
        char* string;
        wchar_t* wideString;
    };
    BA_Boolean isWideString;
} BA_StringImplementation;

BA_Boolean BA_StringImplementation_Contains(const BA_StringImplementation* string, const BA_StringImplementation* compare, BA_Boolean caseless);
BA_Boolean BA_StringImplementation_Equals(const BA_StringImplementation* string, const BA_StringImplementation* compare, BA_Boolean caseless);
BA_Boolean BA_StringImplementation_StartsWith(const BA_StringImplementation* string, const BA_StringImplementation* compare, BA_Boolean caseless);
BA_Boolean BA_StringImplementation_EndsWith(const BA_StringImplementation* string, const BA_StringImplementation* compare, BA_Boolean caseless);
BA_Boolean BA_StringImplementation_ContainsCharacter(const BA_StringImplementation* string, char compare, wchar_t wideCompare, BA_Boolean caseless);

BA_StringImplementation* BA_StringImplementation_Copy(const BA_StringImplementation* duplicateFrom);
BA_StringImplementation* BA_StringImplementation_Append(BA_StringImplementation* target, const BA_StringImplementation* stringToAppend);
BA_StringImplementation* BA_StringImplementation_Prepend(BA_StringImplementation* target, const BA_StringImplementation* stringToPrepend);
BA_StringImplementation* BA_StringImplementation_ToLower(BA_StringImplementation* string);
BA_StringImplementation* BA_StringImplementation_ToUpper(BA_StringImplementation* string);
BA_StringImplementation* BA_StringImplementation_AppendCharacter(BA_StringImplementation* target, char character, wchar_t wideCharacter);
BA_StringImplementation* BA_StringImplementation_PrependCharacter(BA_StringImplementation* target, char character, wchar_t wideCharacter);
BA_StringImplementation* BA_StringImplementation_FormatPremadeList(BA_StringImplementation* target, va_list arguments);
BA_DynamicArray* BA_StringImplementation_Split(const BA_StringImplementation* target, const BA_StringImplementation* splitBy);
BA_DynamicArray* BA_StringImplementation_SplitCharacter(const BA_StringImplementation* target, char splitBy, wchar_t wideSplitBy);
BA_StringImplementation* BA_StringImplementation_FormatSafePremadeList(BA_StringImplementation* target, int amountOfFormatters, va_list arguments);
BA_StringImplementation* BA_StringImplementation_CreateEmpty(BA_Boolean isWideString);
BA_StringImplementation* BA_StringImplementation_Replace(BA_StringImplementation* target, const BA_StringImplementation* what, const BA_StringImplementation* to);
BA_StringImplementation* BA_StringImplementation_ReplaceCharacter(BA_StringImplementation* target, char what, wchar_t wideWhat, char to, wchar_t wideTo);
BA_StringImplementation* BA_StringImplementation_Join(const BA_DynamicArray* dynamicArray, const BA_StringImplementation* joinString);
BA_StringImplementation* BA_StringImplementation_JoinCharacter(const BA_DynamicArray* dynamicArray, BA_Boolean isWideString, char joinCharacter, wchar_t wideJoinCharacter);
BA_StringImplementation* BA_StringImplementation_Convert(const BA_StringImplementation* string);
// TODO: ReadFile, GetLine, GetLineCharacter
BA_CPLUSPLUS_SUPPORT_GUARD_END()

// May god help your soul if you ever need to debug this

#define BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING_WideString(variable) \
BA_StringImplementation variable ## Implementation;                               \
variable ## Implementation.isWideString = BA_BOOLEAN_TRUE;                        \
variable ## Implementation.wideString = (wchar_t*) variable

#define BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING_String(variable) \
BA_StringImplementation variable ## Implementation;                           \
variable ## Implementation.isWideString = BA_BOOLEAN_FALSE;                   \
variable ## Implementation.string = (char*) variable

#define BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, variable) BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING_ ## name(variable)

#define BA_STRINGIMPLEMENTATION_TYPE_WideString wchar_t
#define BA_STRINGIMPLEMENTATION_TYPE_String char
#define BA_STRINGIMPLEMENTATION_TYPE(name) BA_STRINGIMPLEMENTATION_TYPE_ ## name

#define BA_STRINGIMPLEMENTATION_DEREFERENCE_WideString(specifiedString) specifiedString.wideString
#define BA_STRINGIMPLEMENTATION_DEREFERENCE_String(specifiedString) specifiedString.string
#define BA_STRINGIMPLEMENTATION_DEREFERENCE(name, specifiedString) BA_STRINGIMPLEMENTATION_DEREFERENCE_ ## name(specifiedString)

#define BA_STRINGIMPLEMENTATION_FUNCTION_HEADER(returnType, name, function, ...) returnType BA_ ## name ## _ ## function(__VA_ARGS__)

#define BA_STRINGIMPLEMENTATION_GET_FUNCTION(function) BA_StringImplementation_ ## function

#define BA_STRINGIMPLEMENTATION_CHARACTER_WideString(variable) '\0', variable
#define BA_STRINGIMPLEMENTATION_CHARACTER_String(variable) variable, L'\0'
#define BA_STRINGIMPLEMENTATION_CHARACTER(name, variable) BA_STRINGIMPLEMENTATION_CHARACTER_ ## name(variable)

#define BA_STRINGIMPLEMENTATION_CREATE_BASE_CONTAINS_HEADER(name, function) BA_STRINGIMPLEMENTATION_FUNCTION_HEADER(BA_Boolean, name, function, const BA_STRINGIMPLEMENTATION_TYPE(name)* string, const BA_STRINGIMPLEMENTATION_TYPE(name)* compare, BA_Boolean caseless)

#define BA_STRINGIMPLEMENTATION_CREATE_BASE_CONTAINS(name, function) \
BA_STRINGIMPLEMENTATION_CREATE_BASE_CONTAINS_HEADER(name, function) { \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, string); \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, compare); \
    return BA_STRINGIMPLEMENTATION_GET_FUNCTION(function)(&stringImplementation, &compareImplementation, caseless); \
}

#define BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_HEADER(name, function, ...) \
BA_STRINGIMPLEMENTATION_FUNCTION_HEADER(BA_STRINGIMPLEMENTATION_TYPE(name)*, name, function, __VA_ARGS__) { \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, target);         \
    BA_StringImplementation* result = BA_StringImplementation_Copy(&targetImplementation); \
    if (result == NULL)                                                               \
        return NULL;

#define BA_STRINGIMPLEMENTATION_CREATE_BASE_FOOTER(name) \
    BA_STRINGIMPLEMENTATION_TYPE(name)* returnValue = BA_STRINGIMPLEMENTATION_DEREFERENCE(name, (*result)); \
    free(result);                                        \
    return returnValue;                                  \
}

#define BA_STRINGIMPLEMENTATION_CREATE_BASE_FREE_ORIGINAL_FOOTER(name, variable) \
    free(variable);                                                              \
BA_STRINGIMPLEMENTATION_CREATE_BASE_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_FOOTER(name) BA_STRINGIMPLEMENTATION_CREATE_BASE_FREE_ORIGINAL_FOOTER(name, target)

#define BA_STRINGIMPLEMENTATION_BOOLEAN_WideString BA_BOOLEAN_TRUE
#define BA_STRINGIMPLEMENTATION_BOOLEAN_String BA_BOOLEAN_FALSE
#define BA_STRINGIMPLEMENTATION_BOOLEAN(name) BA_STRINGIMPLEMENTATION_BOOLEAN_ ## name

#define BA_STRINGIMPLEMENTATION_OPPOSITE_TYPE_WideString BA_STRINGIMPLEMENTATION_TYPE(String)
#define BA_STRINGIMPLEMENTATION_OPPOSITE_TYPE_String BA_STRINGIMPLEMENTATION_TYPE(WideString)
#define BA_STRINGIMPLEMENTATION_OPPOSITE_TYPE(name) BA_STRINGIMPLEMENTATION_OPPOSITE_TYPE_ ## name

#define BA_STRINGIMPLEMENTATION_CREATE_OPPOSITE_IMPLEMENTATION_STRING_WideString(variable) BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(String, variable)
#define BA_STRINGIMPLEMENTATION_CREATE_OPPOSITE_IMPLEMENTATION_STRING_String(variable) BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(WideString, variable)
#define BA_STRINGIMPLEMENTATION_CREATE_OPPOSITE_IMPLEMENTATION_STRING(name, variable) BA_STRINGIMPLEMENTATION_CREATE_OPPOSITE_IMPLEMENTATION_STRING_ ## name(variable)

#define BA_STRINGIMPLEMENTATION_CREATE_CONTAINS(name) BA_STRINGIMPLEMENTATION_CREATE_BASE_CONTAINS(name, Contains)
#define BA_STRINGIMPLEMENTATION_CREATE_EQUALS(name) BA_STRINGIMPLEMENTATION_CREATE_BASE_CONTAINS(name, Equals)
#define BA_STRINGIMPLEMENTATION_CREATE_STARTS_WITH(name) BA_STRINGIMPLEMENTATION_CREATE_BASE_CONTAINS(name, StartsWith)
#define BA_STRINGIMPLEMENTATION_CREATE_ENDS_WITH(name) BA_STRINGIMPLEMENTATION_CREATE_BASE_CONTAINS(name, EndsWith)

#define BA_STRINGIMPLEMENTATION_CREATE_CONTAINS_CHARACTER(name) \
BA_STRINGIMPLEMENTATION_FUNCTION_HEADER(BA_Boolean, name, ContainsCharacter, const BA_STRINGIMPLEMENTATION_TYPE(name)* string, BA_STRINGIMPLEMENTATION_TYPE(name) compare, BA_Boolean caseless) { \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, string); \
    return BA_StringImplementation_ContainsCharacter(&stringImplementation, BA_STRINGIMPLEMENTATION_CHARACTER(name, compare), caseless); \
}

#define BA_STRINGIMPLEMENTATION_CREATE_COPY(name) \
BA_STRINGIMPLEMENTATION_FUNCTION_HEADER(BA_STRINGIMPLEMENTATION_TYPE(name)*, name, Copy, const BA_STRINGIMPLEMENTATION_TYPE(name)* string) { \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, string); \
    BA_StringImplementation* result = BA_StringImplementation_Copy(&stringImplementation); \
    if (result == NULL)                           \
        return NULL;                              \
BA_STRINGIMPLEMENTATION_CREATE_BASE_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_APPEND(name) \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_HEADER(name, Append, BA_STRINGIMPLEMENTATION_TYPE(name)* target, const BA_STRINGIMPLEMENTATION_TYPE(name)* stringToAppend) \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, stringToAppend); \
    result = BA_StringImplementation_Append(result, &stringToAppendImplementation); \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_PREPEND(name) \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_HEADER(name, Prepend, BA_STRINGIMPLEMENTATION_TYPE(name)* target, const BA_STRINGIMPLEMENTATION_TYPE(name)* stringToPrepend) \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, stringToPrepend); \
    result = BA_StringImplementation_Prepend(result, &stringToPrependImplementation); \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_TO_LOWER(name) \
BA_STRINGIMPLEMENTATION_FUNCTION_HEADER(BA_STRINGIMPLEMENTATION_TYPE(name)*, name, ToLower, BA_STRINGIMPLEMENTATION_TYPE(name)* string) { \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, string); \
    BA_StringImplementation_ToLower(&stringImplementation); \
    return BA_STRINGIMPLEMENTATION_DEREFERENCE(name, stringImplementation);  \
}

#define BA_STRINGIMPLEMENTATION_CREATE_TO_UPPER(name) \
BA_STRINGIMPLEMENTATION_FUNCTION_HEADER(BA_STRINGIMPLEMENTATION_TYPE(name)*, name, ToUpper, BA_STRINGIMPLEMENTATION_TYPE(name)* string) { \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, string); \
    BA_StringImplementation_ToUpper(&stringImplementation); \
    return BA_STRINGIMPLEMENTATION_DEREFERENCE(name, stringImplementation);  \
}

#define BA_STRINGIMPLEMENTATION_CREATE_APPEND_CHARACTER(name) \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_HEADER(name, AppendCharacter, BA_STRINGIMPLEMENTATION_TYPE(name)* target, BA_STRINGIMPLEMENTATION_TYPE(name) character) \
    result = BA_StringImplementation_AppendCharacter(result, BA_STRINGIMPLEMENTATION_CHARACTER(name, character)); \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_PREPEND_CHARACTER(name) \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_HEADER(name, PrependCharacter, BA_STRINGIMPLEMENTATION_TYPE(name)* target, BA_STRINGIMPLEMENTATION_TYPE(name) character) \
    result = BA_StringImplementation_PrependCharacter(result, BA_STRINGIMPLEMENTATION_CHARACTER(name, character)); \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_FORMAT(name) \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_HEADER(name, Format, BA_STRINGIMPLEMENTATION_TYPE(name)* target, ...) \
    va_list arguments;                              \
    va_start(arguments, target);                    \
    result = BA_StringImplementation_FormatPremadeList(result, arguments); \
    va_end(arguments);                              \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_FORMAT_PREMADE_LIST(name) \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_HEADER(name, FormatPremadeList, BA_STRINGIMPLEMENTATION_TYPE(name)* target, va_list arguments) \
    result = BA_StringImplementation_FormatPremadeList(result, arguments); \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_SPLIT(name) \
BA_STRINGIMPLEMENTATION_FUNCTION_HEADER(BA_DynamicArray*, name, Split, const BA_STRINGIMPLEMENTATION_TYPE(name)* target, const BA_STRINGIMPLEMENTATION_TYPE(name)* splitBy) { \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, target); \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, splitBy); \
    return BA_StringImplementation_Split(&targetImplementation, &splitByImplementation); \
}

#define BA_STRINGIMPLEMENTATION_CREATE_SPLIT_CHARACTER(name) \
BA_STRINGIMPLEMENTATION_FUNCTION_HEADER(BA_DynamicArray*, name, SplitCharacter, const BA_STRINGIMPLEMENTATION_TYPE(name)* target, BA_STRINGIMPLEMENTATION_TYPE(name) splitBy) { \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, target); \
    return BA_StringImplementation_SplitCharacter(&targetImplementation, BA_STRINGIMPLEMENTATION_CHARACTER(name, splitBy)); \
}

#define BA_STRINGIMPLEMENTATION_CREATE_FORMAT_SAFE(name) \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_HEADER(name, FormatSafe, BA_STRINGIMPLEMENTATION_TYPE(name)* target, int amountOfFormatters, ...) \
    va_list arguments;                                   \
    va_start(arguments, amountOfFormatters);             \
    result = BA_StringImplementation_FormatSafePremadeList(result, amountOfFormatters, arguments); \
    va_end(arguments);                                   \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_FORMAT_SAFE_PREMADE_LIST(name) \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_HEADER(name, FormatSafePremadeList, BA_STRINGIMPLEMENTATION_TYPE(name)* target, int amountOfFormatters, va_list arguments) \
    result = BA_StringImplementation_FormatSafePremadeList(result, amountOfFormatters, arguments); \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_CREATE_EMPTY(name) \
BA_STRINGIMPLEMENTATION_FUNCTION_HEADER(BA_STRINGIMPLEMENTATION_TYPE(name)*, name, CreateEmpty, void) { \
    BA_StringImplementation* result = BA_StringImplementation_CreateEmpty(BA_STRINGIMPLEMENTATION_BOOLEAN(name)); \
    if (result == NULL)                                   \
        return NULL;                                      \
BA_STRINGIMPLEMENTATION_CREATE_BASE_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_REPLACE(name) \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_HEADER(name, Replace, BA_STRINGIMPLEMENTATION_TYPE(name)* target, const BA_STRINGIMPLEMENTATION_TYPE(name)* what, const BA_STRINGIMPLEMENTATION_TYPE(name)* to) \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, what); \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, to); \
    result = BA_StringImplementation_Replace(result, &whatImplementation, &toImplementation); \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_REPLACE_CHARACTER(name) \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_HEADER(name, ReplaceCharacter, BA_STRINGIMPLEMENTATION_TYPE(name)* target, BA_STRINGIMPLEMENTATION_TYPE(name) what, BA_STRINGIMPLEMENTATION_TYPE(name) to) \
    result = BA_StringImplementation_ReplaceCharacter(result, BA_STRINGIMPLEMENTATION_CHARACTER(name, what), BA_STRINGIMPLEMENTATION_CHARACTER(name, to)); \
BA_STRINGIMPLEMENTATION_CREATE_BASE_MODIFY_TARGET_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_JOIN(name) \
BA_STRINGIMPLEMENTATION_FUNCTION_HEADER(BA_STRINGIMPLEMENTATION_TYPE(name)*, name, Join, const BA_DynamicArray* dynamicArray, const BA_STRINGIMPLEMENTATION_TYPE(name)* joinString) { \
    BA_STRINGIMPLEMENTATION_CREATE_IMPLEMENTATION_STRING(name, joinString); \
    BA_StringImplementation* result = BA_StringImplementation_Join(dynamicArray, &joinStringImplementation); \
BA_STRINGIMPLEMENTATION_CREATE_BASE_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_JOIN_CHARACTER(name) \
BA_STRINGIMPLEMENTATION_FUNCTION_HEADER(BA_STRINGIMPLEMENTATION_TYPE(name)*, name, JoinCharacter, const BA_DynamicArray* dynamicArray, BA_STRINGIMPLEMENTATION_TYPE(name) joinCharacter) { \
    BA_StringImplementation* result = BA_StringImplementation_JoinCharacter(dynamicArray, BA_STRINGIMPLEMENTATION_BOOLEAN(name), BA_STRINGIMPLEMENTATION_CHARACTER(name, joinCharacter)); \
BA_STRINGIMPLEMENTATION_CREATE_BASE_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE_CONVERT(name) \
BA_STRINGIMPLEMENTATION_FUNCTION_HEADER(BA_STRINGIMPLEMENTATION_TYPE(name)*, name, Convert, const BA_STRINGIMPLEMENTATION_OPPOSITE_TYPE(name)* target) { \
    BA_STRINGIMPLEMENTATION_CREATE_OPPOSITE_IMPLEMENTATION_STRING(name, target); \
    BA_StringImplementation* result = BA_StringImplementation_Convert(&targetImplementation); \
BA_STRINGIMPLEMENTATION_CREATE_BASE_FOOTER(name)

#define BA_STRINGIMPLEMENTATION_CREATE(name) \
BA_STRINGIMPLEMENTATION_CREATE_CONTAINS(name) \
BA_STRINGIMPLEMENTATION_CREATE_EQUALS(name)  \
BA_STRINGIMPLEMENTATION_CREATE_STARTS_WITH(name) \
BA_STRINGIMPLEMENTATION_CREATE_ENDS_WITH(name) \
BA_STRINGIMPLEMENTATION_CREATE_CONTAINS_CHARACTER(name) \
BA_STRINGIMPLEMENTATION_CREATE_COPY(name)    \
BA_STRINGIMPLEMENTATION_CREATE_APPEND(name)  \
BA_STRINGIMPLEMENTATION_CREATE_PREPEND(name) \
BA_STRINGIMPLEMENTATION_CREATE_TO_LOWER(name) \
BA_STRINGIMPLEMENTATION_CREATE_TO_UPPER(name) \
BA_STRINGIMPLEMENTATION_CREATE_APPEND_CHARACTER(name) \
BA_STRINGIMPLEMENTATION_CREATE_PREPEND_CHARACTER(name) \
BA_STRINGIMPLEMENTATION_CREATE_FORMAT(name)  \
BA_STRINGIMPLEMENTATION_CREATE_FORMAT_PREMADE_LIST(name) \
BA_STRINGIMPLEMENTATION_CREATE_SPLIT(name)   \
BA_STRINGIMPLEMENTATION_CREATE_SPLIT_CHARACTER(name) \
BA_STRINGIMPLEMENTATION_CREATE_FORMAT_SAFE(name) \
BA_STRINGIMPLEMENTATION_CREATE_FORMAT_SAFE_PREMADE_LIST(name) \
BA_STRINGIMPLEMENTATION_CREATE_CREATE_EMPTY(name) \
BA_STRINGIMPLEMENTATION_CREATE_REPLACE(name) \
BA_STRINGIMPLEMENTATION_CREATE_REPLACE_CHARACTER(name) \
BA_STRINGIMPLEMENTATION_CREATE_JOIN(name)    \
BA_STRINGIMPLEMENTATION_CREATE_JOIN_CHARACTER(name) \
BA_STRINGIMPLEMENTATION_CREATE_CONVERT(name)
