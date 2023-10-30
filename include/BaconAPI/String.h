// Purpose: String wrapper
// Created on: 7/4/2023 @ 7:17 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "Internal/Boolean.h"
#include "Storage/DynamicArray.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef enum {
    BA_STRING_SAFE_FORMAT_TYPE_STRING,
    BA_STRING_SAFE_FORMAT_TYPE_INTEGER,
    BA_STRING_SAFE_FORMAT_TYPE_DOUBLE,
    BA_STRING_SAFE_FORMAT_TYPE_CHARACTER,
    BA_STRING_SAFE_FORMAT_TYPE_LONG,
    BA_STRING_SAFE_FORMAT_TYPE_LONG_LONG,
    BA_STRING_SAFE_FORMAT_TYPE_SHORT,
    BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED,
    BA_STRING_SAFE_FORMAT_TYPE_LONG_DOUBLE,
    BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_CHARACTER,
    BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_LONG,
    BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_LONG_LONG,

    BA_STRING_SAFE_FORMAT_TYPE_SIGNED = BA_STRING_SAFE_FORMAT_TYPE_INTEGER,
    BA_STRING_SAFE_FORMAT_TYPE_SIGNED_INTEGER = BA_STRING_SAFE_FORMAT_TYPE_INTEGER,
    BA_STRING_SAFE_FORMAT_TYPE_SIGNED_CHARACTER = BA_STRING_SAFE_FORMAT_TYPE_CHARACTER,
    BA_STRING_SAFE_FORMAT_TYPE_LONG_INTEGER = BA_STRING_SAFE_FORMAT_TYPE_LONG,
    BA_STRING_SAFE_FORMAT_TYPE_SIGNED_LONG = BA_STRING_SAFE_FORMAT_TYPE_LONG,
    BA_STRING_SAFE_FORMAT_TYPE_SIGNED_LONG_INTEGER = BA_STRING_SAFE_FORMAT_TYPE_LONG,
    BA_STRING_SAFE_FORMAT_TYPE_LONG_LONG_INTEGER = BA_STRING_SAFE_FORMAT_TYPE_LONG_LONG,
    BA_STRING_SAFE_FORMAT_TYPE_SIGNED_LONG_LONG = BA_STRING_SAFE_FORMAT_TYPE_LONG_LONG,
    BA_STRING_SAFE_FORMAT_TYPE_SIGNED_LONG_LONG_INTEGER = BA_STRING_SAFE_FORMAT_TYPE_LONG_LONG,
    BA_STRING_SAFE_FORMAT_TYPE_SHORT_INTEGER = BA_STRING_SAFE_FORMAT_TYPE_SHORT,
    BA_STRING_SAFE_FORMAT_TYPE_SHORT_SIGNED_SHORT = BA_STRING_SAFE_FORMAT_TYPE_SHORT,
    BA_STRING_SAFE_FORMAT_TYPE_SHORT_SIGNED_SHORT_INTEGER = BA_STRING_SAFE_FORMAT_TYPE_SHORT,
    BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_INTEGER = BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED,
    BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_LONG_INTEGER = BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_LONG,
    BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_LONG_LONG_INTEGER = BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_LONG_LONG
} BA_String_SafeFormatTypes;

BA_Boolean BA_String_Contains(const char* string, const char* compare, BA_Boolean caseless);
BA_Boolean BA_String_Equals(const char* string, const char* compare, BA_Boolean caseless);
BA_Boolean BA_String_StartsWith(const char* string, const char* compare, BA_Boolean caseless);
BA_Boolean BA_String_EndsWith(const char* string, const char* compare, BA_Boolean caseless);
BA_Boolean BA_String_ContainsCharacter(const char* string, char compare, BA_Boolean caseless);

/**
 * @note Returns NULL if it fails to allocate memory
 */
char* BA_String_Copy(const char* duplicateFrom);

/**
 * @return The string target
 * @note Returns NULL if it fails to allocate memory
 */
char* BA_String_Append(char** target, const char* stringToAppend);

/**
 * @return The string target
 * @note Returns NULL if it fails to allocate memory
 */
char* BA_String_Prepend(char** target, const char* stringToPrepend);

char* BA_String_ToLower(char* string);
char* BA_String_ToUpper(char* string);

/**
 * @return The string target
 * @note Returns NULL if it fails to allocate memory
 */
char* BA_String_AppendCharacter(char** target, char character);

/**
 * @return The string target
 * @note Returns NULL if it fails to allocate memory
 */
char* BA_String_PrependCharacter(char** target, char character);

/** 
 * @return The string target
 * @note Returns NULL if it fails to allocate memory
 * @warning Passing user controlled input as target will open up a uncontrolled format string attack.
 */
char* BA_String_Format(char** target, ...);

/** 
 * @return The string target
 * @note Returns NULL if it fails to allocate memory
 * @warning Passing user controlled input as target will open up a uncontrolled format string attack.
 */
char* BA_String_FormatPremadeList(char** target, va_list arguments);

/**
 * @return A char* DynamicArray
 * @note Make sure to free all the elements, including the DynamicArray's internalArray, and the DynamicArray itself, once done using
 */
BA_DynamicArray* BA_String_Split(const char* target, const char* splitBy);

/**
 * @return A char* DynamicArray
 * @note Make sure to free all the elements, including the DynamicArray's internalArray, and the DynamicArray itself, once done using
 */
BA_DynamicArray* BA_String_SplitCharacter(const char* target, char splitBy);

/**
 * Reads all the file contents into a buffer, this buffer can get quite big
 * @param lengthLimit Limit how long the buffer will be, zero will disable it
 * @param lineLength Tells you how long the resulting buffer is. We do nothing to it if it equals to NULL
 * @return The contents of the file, NULL if it failed to allocate memory
 */
char* BA_String_ReadFile(FILE* file, size_t lengthLimit, size_t* lineLength);

/**
 * Gets the next line in the file
 * @param line The output string
 * @param splitString Split the file contents by this string, defaults to newline if NULL
 * @return The length of the current line, -1 if it's end of file, -2 if it failed to allocate buffer
 * @note This moves the file pointer
 * @warning Make sure to run free on the returned buffer
 */
ssize_t BA_String_GetLine(FILE* file, char** line, const char* splitString);

/**
 * Gets the next line in the file
 * @param line The output string
 * @param splitCharacter Split the file contents by this character
 * @return The length of the current line, -1 if it's end of file, -2 if it failed to allocate buffer
 * @note This moves the file pointer
 * @warning Make sure to run free on the returned buffer
 */
ssize_t BA_String_GetLineCharacter(FILE* file, char** line, char splitCharacter);

/**
 * Safer version of BA_String_Format, intended to be used when target is controlled by user input.
 * Use %s when you want to substitute an argument, no matter the type
 * @param ... Each format type
 * @return The string target
 * @note Returns NULL if it fails to allocate memory
 * @warning Undefined behavior if amountOfFormatters is greater than the amount of formatters you've actually passed
 * @example
 * @code
 * char* exampleString = BA_String_Copy("Hello, %s\n");
 * BA_String_FormatSafe(&exampleString, 1, BA_STRING_FORMAT_SAFE_ARGUMENT_STRING("World!"));
 * @endcode
 * You should be using the helper macros for this, but this is essentially how it works.
 * The code takes amountOfFormatters and times it by 2.
 * The reason why is because the helper macros gives two pieces of information at once: `BA_String_SafeFormatTypes, value`.
 * This is so we know what the values type is.
 * It works something like this:
 * @code
 * char* exampleString = BA_String_Copy("Hello, %s\n");
 * BA_String_FormatSafe(&exampleString, 1, BA_STRING_SAFE_FORMAT_TYPE_STRING, "World!");
 * @endcode
 */
char* BA_String_FormatSafe(char** target, int amountOfFormatters, ...);

char* BA_String_FormatSafePremadeList(char** target, int amountOfFormatters, va_list arguments);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BA_STRING_FORMAT_SAFE_ARGUMENT_STRING(value) BA_STRING_SAFE_FORMAT_TYPE_STRING, value
#define BA_STRING_FORMAT_SAFE_ARGUMENT_INTEGER(value) BA_STRING_SAFE_FORMAT_TYPE_INTEGER, value

/**
 * Requires a decimal point, even if the number after is just zero
 */
#define BA_STRING_FORMAT_SAFE_ARGUMENT_DOUBLE(value) BA_STRING_SAFE_FORMAT_TYPE_DOUBLE, value
#define BA_STRING_FORMAT_SAFE_ARGUMENT_CHARACTER(value) BA_STRING_SAFE_FORMAT_TYPE_CHARACTER, value
#define BA_STRING_FORMAT_SAFE_ARGUMENT_LONG(value) BA_STRING_SAFE_FORMAT_TYPE_LONG, value
#define BA_STRING_FORMAT_SAFE_ARGUMENT_LONG_LONG(value) BA_STRING_SAFE_FORMAT_TYPE_LONG_LONG, value
#define BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED(value) BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED, value

/**
 * Requires a decimal point, and the suffix: l
 */
#define BA_STRING_FORMAT_SAFE_ARGUMENT_LONG_DOUBLE(value) BA_STRING_SAFE_FORMAT_TYPE_LONG_DOUBLE, value
#define BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED_CHARACTER(value) BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_CHARACTER, value
#define BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED_LONG(value) BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_LONG, value
#define BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED_LONG_LONG(value) BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_LONG_LONG, value

#define BA_STRING_FORMAT_SAFE_ARGUMENT_SIGNED(value) BA_STRING_FORMAT_SAFE_ARGUMENT_INTEGER(value)
#define BA_STRING_FORMAT_SAFE_ARGUMENT_SIGNED_INTEGER(value) BA_STRING_FORMAT_SAFE_ARGUMENT_INTEGER(value)
#define BA_STRING_FORMAT_SAFE_ARGUMENT_SIGNED_CHARACTER(value) BA_STRING_FORMAT_SAFE_ARGUMENT_CHARACTER(value)
#define BA_STRING_FORMAT_SAFE_ARGUMENT_LONG_INTEGER(value) BA_STRING_FORMAT_SAFE_ARGUMENT_LONG(value)
#define BA_STRING_FORMAT_SAFE_ARGUMENT_SIGNED_LONG(value) BA_STRING_FORMAT_SAFE_ARGUMENT_LONG(value)
#define BA_STRING_FORMAT_SAFE_ARGUMENT_SIGNED_LONG_INTEGER(value) BA_STRING_FORMAT_SAFE_ARGUMENT_LONG(value)
#define BA_STRING_FORMAT_SAFE_ARGUMENT_LONG_LONG_INTEGER(value) BA_STRING_FORMAT_SAFE_ARGUMENT_LONG_LONG(value)
#define BA_STRING_FORMAT_SAFE_ARGUMENT_SIGNED_LONG_LONG(value) BA_STRING_FORMAT_SAFE_ARGUMENT_LONG_LONG(value)
#define BA_STRING_FORMAT_SAFE_ARGUMENT_SIGNED_LONG_LONG_INTEGER(value) BA_STRING_FORMAT_SAFE_ARGUMENT_LONG_LONG(value)
#define BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED_INTEGER(value) BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED(value)
#define BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED_LONG_INTEGER(value) BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED_LONG(value)
#define BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED_LONG_LONG_INTEGER(value) BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED_LONG_LONG(value)
