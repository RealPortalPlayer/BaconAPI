// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/String.h>
#include <string.h>

#define CASE_COMPARE(function, string, upper, lower, dummy) \
BA_ASSERT(!function(string, lower, BA_BOOLEAN_FALSE), "Invalid case sensitive match\n"); \
BA_ASSERT(function(string, upper, BA_BOOLEAN_FALSE), "Invalid case sensitive match\n"); \
BA_ASSERT(function(string, lower, BA_BOOLEAN_TRUE), "Invalid case insensitive match\n"); \
BA_ASSERT(function(string, upper, BA_BOOLEAN_TRUE), "Invalid case insensitive match\n"); \
BA_ASSERT(!function(string, dummy, BA_BOOLEAN_FALSE), "Found dummy string\n")
#define MAIN_TEST(function, string, secondaryString, expectedOutput) \
do {                                                                 \
    char* result = BA_String_Copy(string);                           \
    BA_ASSERT(result != NULL, "Failed to allocate memory for string\n"); \
    BA_ASSERT((result = function(result, secondaryString)) != NULL, "Failed to allocate memory for string\n"); \
    BA_ASSERT(strcmp(result, expectedOutput) == 0, "String did not match expected outcome\n"); \
    free(result);                                                    \
} while (BA_BOOLEAN_FALSE)
#define SECONDARY_TEST(function, string, expectedOutput) \
do {                                                     \
    char* result = BA_String_Copy(string);               \
    BA_ASSERT(result != NULL, "Failed to allocate memory for string\n"); \
    BA_ASSERT((result = function(result)) != NULL, "Failed to allocate memory for string\n"); \
    BA_ASSERT(strcmp(result, expectedOutput) == 0, "String did not match expected outcome\n"); \
    free(result);                                         \
} while (BA_BOOLEAN_FALSE)
#define THIRD_TEST(function, string, secondaryString, thirdString, expectedOutput) \
do {                                                           \
    char* result = BA_String_Copy(string);                     \
    BA_ASSERT(result != NULL, "Failed to allocate memory for string\n"); \
    BA_ASSERT((result = function(result, secondaryString, thirdString)) != NULL, "Failed to allocate memory for string\n"); \
    BA_ASSERT(strcmp(result, expectedOutput) == 0, "String did not match expected outcome\n"); \
    free(result);                                              \
} while (BA_BOOLEAN_FALSE)

#define FORMAT_SAFE_ARGUMENT_BOOLEAN(boolean) 1, boolean

char* FormatSafeWrapper(char* target, ...) {
    va_list arguments;

    va_start(arguments, target);
    
    target = BA_String_FormatSafePremadeList(target, 1, arguments);

    va_end(arguments);
    return target;
}

char* BooleanSafeFormat(char* buffer, void** argument) {
    return BA_String_Append(buffer, *(int*) argument ? "Yes" : "No");
}

int main(int argc, char** argv) {
    BA_ArgumentHandler_Initialize(argc, argv);
    CASE_COMPARE(BA_String_Contains, "Hello, World!", "Hello", "hello", "Goodbye");
    CASE_COMPARE(BA_String_Equals, "Hello, World!", "Hello, World!", "hello, world!", "Goodbye, Moon!");
    CASE_COMPARE(BA_String_StartsWith, "Hello, World!", "Hello", "hello", "Goodbye");
    CASE_COMPARE(BA_String_EndsWith, "Hello, World!", "World!", "world!", "Moon!");
    CASE_COMPARE(BA_String_ContainsCharacter, "Hello, World!", 'H', 'h', 'G');
    MAIN_TEST(BA_String_Append, "Hello,", " World!", "Hello, World!");
    MAIN_TEST(BA_String_Prepend, " Moon!", "Goodbye,", "Goodbye, Moon!");
    SECONDARY_TEST(BA_String_ToLower, "Hello, World!", "hello, world!");
    SECONDARY_TEST(BA_String_ToUpper, "Goodbye, Moon!", "GOODBYE, MOON!");
    MAIN_TEST(BA_String_AppendCharacter, "Hello, World", '!', "Hello, World!");
    MAIN_TEST(BA_String_PrependCharacter, "oodbye, Moon!", 'G', "Goodbye, Moon!");
    MAIN_TEST(BA_String_Format, "Hello, %s", "World!", "Hello, World!");
    // NOTE: BA_String_Format is a wrapper around BA_String_FormatPremadeList
    // TODO: BA_String_Split, BA_String_SplitCharacter
    // TODO: Good way to test BA_String_ReadFile, BA_String_GetLine, BA_String_GetLineCharacter
    MAIN_TEST(FormatSafeWrapper, "%s Moon! % %s %i %% %", BA_STRING_FORMAT_SAFE_ARGUMENT_STRING("Goodbye,"), "Goodbye, Moon! % %s %i %% %");
    // NOTE: BA_String_FormatSafe is a wrapper around BA_String_FormatSafePremadeList
    
    {
        char* result = BA_String_CreateEmpty();

        BA_ASSERT(result != NULL, "Failed to allocate memory for empty string\n");
        free(result);
    }

    BA_ASSERT(BA_String_AddCustomSafeFormatter(1, &BooleanSafeFormat), "Failed to add custom safe formatter\n");
    MAIN_TEST(FormatSafeWrapper, "Said hello to world? %s", FORMAT_SAFE_ARGUMENT_BOOLEAN(BA_BOOLEAN_TRUE), "Said hello to world? Yes");
    do {
        char* result = BA_String_Copy("Hello, Moon!");
        BA_ASSERT(result != NULL, "Failed to allocate memory for string\n");
        result = BA_String_Replace(result, "Hello", "Goodbye");
        BA_ASSERT(result != NULL, "Failed to allocate memory for string\n");
        BA_ASSERT(strcmp(result, "Goodbye, Moon!") == 0, "String did not match expected outcome\n");
        free(result);
    } while (BA_BOOLEAN_FALSE);
    THIRD_TEST(BA_String_ReplaceCharacter, "Gello, World!", 'G', 'H', "Hello, World!");
    // TODO: BA_String_Join, BA_String_JoinCharacter
}
