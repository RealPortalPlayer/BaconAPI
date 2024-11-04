// Purpose: String testing helper
// Created on: 9/19/24 @ 9:12 AM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <wchar.h>
#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/String.h>
#include <BaconAPI/WideString.h>

void* BooleanSafeFormat(void* buffer, BA_Boolean fromWideCharacter, void** argument) {
    BA_Boolean boolean = *(int*) argument;

    return fromWideCharacter ? (void*) BA_WideString_Append(buffer, (const wchar_t*) (boolean ? L"Yes" : L"No")) : (void*) BA_String_Append(buffer, boolean ? "Yes" : "No");
}

#define STRING_HELPER_ARGUMENT_BOOLEAN(boolean) 1, boolean

#define STRING_HELPER_TYPE_WideString wchar_t
#define STRING_HELPER_TYPE_String char
#define STRING_HELPER_TYPE(name) STRING_HELPER_TYPE_ ## name

#define STRING_HELPER_GET_FUNCTION(name, function) BA_ ## name ## _ ## function

#define STRING_HELPER_DO_TEST(name, expected, message, functionName, ...) BA_ASSERT(STRING_HELPER_GET_FUNCTION(name, functionName)(__VA_ARGS__) == expected, message)

#define STRING_HELPER_PARSE_STRING_WideString(string) L ## string
#define STRING_HELPER_PARSE_STRING_String(string) string
#define STRING_HELPER_PARSE_STRING(name, string) STRING_HELPER_PARSE_STRING_ ## name(string)

#define STRING_HELPER_HEADER() do {

#define STRING_HELPER_FOOTER() } while (BA_BOOLEAN_FALSE)

#define STRING_HELPER_BASE(name, expected, message, functionName, ...) \
STRING_HELPER_HEADER()                                                 \
    STRING_HELPER_DO_TEST(name, expected, message, functionName, __VA_ARGS__); \
STRING_HELPER_FOOTER()

#define STRING_HELPER_COPY_STRING_HEADER(name, string) \
STRING_HELPER_HEADER()                                 \
    STRING_HELPER_TYPE(name)* results = STRING_HELPER_GET_FUNCTION(name, Copy)(string); \
    BA_ASSERT(results != NULL, "Failed to allocate memory for string\n");

#define STRING_HELPER_COPY_STRING_FOOTER(name, string1, expectedString, expected) \
    STRING_HELPER_DO_TEST(name, expected, "String did not match outcome\n", Equals, results, expectedString, BA_BOOLEAN_FALSE); \
    free(results);                                                                \
STRING_HELPER_FOOTER()

#define STRING_HELPER_COPY_STRING_BASE(name, string1, expected, functionName, ...) \
STRING_HELPER_COPY_STRING_HEADER(name, STRING_HELPER_PARSE_STRING(name, string1))  \
    results = STRING_HELPER_GET_FUNCTION(name, functionName)(__VA_ARGS__);         \
STRING_HELPER_COPY_STRING_FOOTER(name, STRING_HELPER_PARSE_STRING(name, string1), STRING_HELPER_PARSE_STRING(name, expected), BA_BOOLEAN_TRUE)

#define STRING_HELPER_SPLIT_HEADER(name, string1, splitBy, functionName) \
STRING_HELPER_HEADER()                                        \
    BA_DynamicArray* results = STRING_HELPER_GET_FUNCTION(name, functionName)(STRING_HELPER_PARSE_STRING(name, string1), STRING_HELPER_PARSE_STRING(name, splitBy))

#define STRING_HELPER_SPLIT_FOOTER() \
    free(results->internalArray[0]); \
    free(results->internalArray[1]); \
    free(results->internalArray);    \
    free(results);                   \
STRING_HELPER_FOOTER()

#define STRING_HELPER_SPLIT_BASE(name, string1, splitBy, functionName, expected1, expected2) \
STRING_HELPER_SPLIT_HEADER(name, string1, splitBy, functionName);                            \
    BA_ASSERT(STRING_HELPER_GET_FUNCTION(name, Equals)(results->internalArray[0], STRING_HELPER_PARSE_STRING(name, expected1), BA_BOOLEAN_FALSE), "Failed to split string\n"); \
    BA_ASSERT(STRING_HELPER_GET_FUNCTION(name, Equals)(results->internalArray[1], STRING_HELPER_PARSE_STRING(name, expected2), BA_BOOLEAN_FALSE), "Failed to split string\n"); \
STRING_HELPER_SPLIT_FOOTER()

#define STRING_HELPER_JOIN_BASE(name, string1, splitBy, splitFunctionName, functionName) \
STRING_HELPER_SPLIT_HEADER(name, string1, splitBy, splitFunctionName);                   \
    STRING_HELPER_TYPE(name)* joined = STRING_HELPER_GET_FUNCTION(name, functionName)(results, STRING_HELPER_PARSE_STRING(name, splitBy)); \
    BA_ASSERT(STRING_HELPER_GET_FUNCTION(name, Equals)(STRING_HELPER_PARSE_STRING(name, string1), joined, BA_BOOLEAN_FALSE), "Failed to join string\n"); \
    free(joined);                                                                         \
STRING_HELPER_SPLIT_FOOTER()

#define STRING_HELPER_OPPOSITE_PARSE_STRING_WideString(string) STRING_HELPER_PARSE_STRING(String, string)
#define STRING_HELPER_OPPOSITE_PARSE_STRING_String(string) STRING_HELPER_PARSE_STRING(WideString, string)
#define STRING_HELPER_OPPOSITE_PARSE_STRING(name, string) STRING_HELPER_OPPOSITE_PARSE_STRING_ ## name(string)

#define STRING_HELPER_FORMATTER_CODE_WideString "%ls"
#define STRING_HELPER_FORMATTER_CODE_String "%s"
#define STRING_HELPER_FORMATTER_CODE(name) STRING_HELPER_FORMATTER_CODE_ ## name

#define STRING_HELPER_TEST_CONTAINS(name, string1, string2, caseless, expected, message) STRING_HELPER_BASE(name, expected, message, Contains, STRING_HELPER_PARSE_STRING(name, string1), STRING_HELPER_PARSE_STRING(name, string2), caseless)
#define STRING_HELPER_TEST_EQUALS(name, string1, string2, caseless, expected, message) STRING_HELPER_BASE(name, expected, message, Equals, STRING_HELPER_PARSE_STRING(name, string1), STRING_HELPER_PARSE_STRING(name, string2), caseless)
#define STRING_HELPER_TEST_STARTS_WITH(name, string1, string2, caseless, expected, message) STRING_HELPER_BASE(name, expected, message, StartsWith, STRING_HELPER_PARSE_STRING(name, string1), STRING_HELPER_PARSE_STRING(name, string2), caseless)
#define STRING_HELPER_TEST_ENDS_WITH(name, string1, string2, caseless, expected, message) STRING_HELPER_BASE(name, expected, message, EndsWith, STRING_HELPER_PARSE_STRING(name, string1), STRING_HELPER_PARSE_STRING(name, string2), caseless)
#define STRING_HELPER_TEST_CONTAINS_CHARACTER(name, string1, string2, caseless, expected, message) STRING_HELPER_BASE(name, expected, message, ContainsCharacter, STRING_HELPER_PARSE_STRING(name, string1), STRING_HELPER_PARSE_STRING(name, string2), caseless)
#define STRING_HELPER_TEST_APPEND(name, string1, string2, expected) STRING_HELPER_COPY_STRING_BASE(name, string1, expected, Append, results, STRING_HELPER_PARSE_STRING(name, string2))
#define STRING_HELPER_TEST_PREPEND(name, string1, string2, expected) STRING_HELPER_COPY_STRING_BASE(name, string1, expected, Prepend, results, STRING_HELPER_PARSE_STRING(name, string2))
#define STRING_HELPER_TEST_TO_LOWER(name, string1, expected) STRING_HELPER_COPY_STRING_BASE(name, string1, expected, ToLower, results)
#define STRING_HELPER_TEST_TO_UPPER(name, string1, expected) STRING_HELPER_COPY_STRING_BASE(name, string1, expected, ToUpper, results)
#define STRING_HELPER_TEST_APPEND_CHARACTER(name, string1, character, expected) STRING_HELPER_COPY_STRING_BASE(name, string1, expected, AppendCharacter, results, STRING_HELPER_PARSE_STRING(name, character))
#define STRING_HELPER_TEST_PREPEND_CHARACTER(name, string1, character, expected) STRING_HELPER_COPY_STRING_BASE(name, string1, expected, PrependCharacter, results, STRING_HELPER_PARSE_STRING(name, character))
#define STRING_HELPER_TEST_FORMAT(name, string1, string2, expected) STRING_HELPER_COPY_STRING_BASE(name, string1, expected, Format, results, STRING_HELPER_PARSE_STRING(name, string2))
#define STRING_HELPER_TEST_SPLIT(name, string1, splitBy, expected1, expected2) STRING_HELPER_SPLIT_BASE(name, string1, splitBy, Split, expected1, expected2)
#define STRING_HELPER_TEST_SPLIT_CHARACTER(name, string1, splitByCharacter, expected1, expected2) STRING_HELPER_SPLIT_BASE(name, string1, splitByCharacter, SplitCharacter, expected1, expected2)
#define STRING_HELPER_TEST_FORMAT_SAFE(name, string1, string2, expected) STRING_HELPER_COPY_STRING_BASE(name, string1, expected, FormatSafe, results, 1, string2)
#define STRING_HELPER_TEST_REPLACE(name, string1, string2, string3, expected) STRING_HELPER_COPY_STRING_BASE(name, string1, expected, Replace, results, STRING_HELPER_PARSE_STRING(name, string2), STRING_HELPER_PARSE_STRING(name, string3))
#define STRING_HELPER_TEST_REPLACE_CHARACTER(name, string1, character1, character2, expected) STRING_HELPER_COPY_STRING_BASE(name, string1, expected, ReplaceCharacter, results, STRING_HELPER_PARSE_STRING(name, character1), STRING_HELPER_PARSE_STRING(name, character2))
#define STRING_HELPER_JOIN(name, string1, splitBy) STRING_HELPER_JOIN_BASE(name, string1, splitBy, Split, Join)
#define STRING_HELPER_JOIN_CHARACTER(name, string1, splitByCharacter) STRING_HELPER_JOIN_BASE(name, string1, splitByCharacter, SplitCharacter, JoinCharacter)
#define STRING_HELPER_TEST_CONVERT(name, string1) \
STRING_HELPER_HEADER()                            \
    STRING_HELPER_TYPE(name)* results = STRING_HELPER_GET_FUNCTION(name, Convert)(STRING_HELPER_OPPOSITE_PARSE_STRING(name, string1)); \
    BA_ASSERT(STRING_HELPER_GET_FUNCTION(name, Equals)(results, STRING_HELPER_PARSE_STRING(name, string1), BA_BOOLEAN_FALSE), "Failed to convert string\n"); \
    free(results);                                \
STRING_HELPER_FOOTER()

#define STRING_HELPER_TEST_GROUP_CONTAINS(name, function, upper, lower, dummy) \
STRING_HELPER_TEST_ ## function(name, "Hello, World!", lower, BA_BOOLEAN_FALSE, BA_BOOLEAN_FALSE, "Invalid case sensitive match\n"); \
STRING_HELPER_TEST_ ## function(name, "Hello, World!", upper, BA_BOOLEAN_FALSE, BA_BOOLEAN_TRUE, "Invalid case sensitive match\n"); \
STRING_HELPER_TEST_ ## function(name, "Hello, World!", lower, BA_BOOLEAN_TRUE, BA_BOOLEAN_TRUE, "Invalid case insensitive match\n"); \
STRING_HELPER_TEST_ ## function(name, "Hello, World!", upper, BA_BOOLEAN_TRUE, BA_BOOLEAN_TRUE, "Invalid case insensitive match\n"); \
STRING_HELPER_TEST_ ## function(name, "Hello, World!", dummy, BA_BOOLEAN_FALSE, BA_BOOLEAN_FALSE, "Found dummy string\n")

#define STRING_HELPER_CREATE(name) \
STRING_HELPER_TEST_GROUP_CONTAINS(name, CONTAINS, "Hello", "hello", "Goodbye"); \
STRING_HELPER_TEST_GROUP_CONTAINS(name, EQUALS, "Hello, World!", "hello, world!", "Goodbye, Moon!"); \
STRING_HELPER_TEST_GROUP_CONTAINS(name, STARTS_WITH, "Hello", "hello", "Goodbye"); \
STRING_HELPER_TEST_GROUP_CONTAINS(name, ENDS_WITH, "World!", "world!", "Moon!"); \
STRING_HELPER_TEST_GROUP_CONTAINS(name, CONTAINS_CHARACTER, 'H', 'h', 'G'); \
STRING_HELPER_DO_TEST(name, NULL, "Did not return NULL\n", Copy, NULL); \
STRING_HELPER_TYPE(name)* result = STRING_HELPER_GET_FUNCTION(name, Copy)(STRING_HELPER_PARSE_STRING(name, "Goodbye, Moon!")); \
BA_ASSERT(result != NULL, "Failed to allocate memory for string\n"); \
BA_ASSERT(STRING_HELPER_GET_FUNCTION(name, Equals)(result, STRING_HELPER_PARSE_STRING(name, "Goodbye, Moon!"), BA_BOOLEAN_FALSE), "String did not copy correctly\n"); \
free(result);                      \
STRING_HELPER_TEST_APPEND(name, "Hello, ", "World!", "Hello, World!"); \
STRING_HELPER_TEST_PREPEND(name, "Moon!", "Goodbye, ", "Goodbye, Moon!"); \
STRING_HELPER_TEST_TO_LOWER(name, "Hello, World!", "hello, world!"); \
STRING_HELPER_TEST_TO_UPPER(name, "Goodbye, Moon!", "GOODBYE, MOON!"); \
STRING_HELPER_TEST_APPEND_CHARACTER(name, "Hello, World", '!', "Hello, World!"); \
STRING_HELPER_TEST_PREPEND_CHARACTER(name, "oodbye, Moon!", 'G', "Goodbye, Moon!"); \
STRING_HELPER_TEST_FORMAT(name, "Hello, " STRING_HELPER_FORMATTER_CODE(name), "World!", "Hello, World!"); \
STRING_HELPER_TEST_SPLIT(name, "Goodbye, Moon!", " ", "Goodbye,", "Moon!"); \
STRING_HELPER_TEST_SPLIT_CHARACTER(name, "Hello, World!", ' ', "Hello,", "World!"); \
STRING_HELPER_TEST_FORMAT_SAFE(name, "%s Moon! % %s %i %% %", BA_STRINGSAFEFORMAT_ARGUMENT_STRING("Goodbye,"), "Goodbye, Moon! % %s %i %% %"); \
BA_ASSERT(BA_StringSafeFormat_AddCustomSafeFormatter(1, &BooleanSafeFormat), "Failed to add custom safe formatter\n"); \
STRING_HELPER_TEST_FORMAT_SAFE(name, "Said hello to world? %s", STRING_HELPER_ARGUMENT_BOOLEAN(BA_BOOLEAN_TRUE), "Said hello to world? Yes"); \
STRING_HELPER_TYPE(name)* empty = STRING_HELPER_GET_FUNCTION(name, CreateEmpty)(); \
BA_ASSERT(empty != NULL, "Failed to create an empty string\n"); \
free(empty);                       \
STRING_HELPER_TEST_REPLACE(name, "Hello, Moon!", "Hello", "Goodbye", "Goodbye, Moon!"); \
STRING_HELPER_TEST_REPLACE_CHARACTER(name, "Hello, World?", '?', '!', "Hello, World!"); \
STRING_HELPER_JOIN(name, "Goodbye, Moon!", " "); \
STRING_HELPER_JOIN_CHARACTER(name, "Goodbye, Moon!", ' '); \
STRING_HELPER_TEST_CONVERT(name, "Goodbye, Moon!")
