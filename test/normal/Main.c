// Copyright (c) 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/Storage/DynamicArray.h>
#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/Storage/DynamicDictionary.h>
#include <BaconAPI/ANSI.h>
#include <string.h>
#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/String.h>
#include <BaconAPI/Number.h>
#include <BaconAPI/Debugging/StaticAssert.h>

#include "../TestHeader.h"

#define FAKE_ARGUMENTS_AMOUNT 7
#define VALUES_LENGTH 14
#define VALUES_SPLIT_LENGTH 2

void BooleanFormat(char** buffer, void** argument) {
    BA_String_Append(buffer, *((int*) argument) ? "true" : "false");
}

int main(void) {
    char** fakeArguments = malloc(sizeof(char*) * FAKE_ARGUMENTS_AMOUNT + 2);
    char values[] = "qwertyui--asdf";
    int secondIndex = 0;

    BA_STATIC_ASSERT(sizeof(values) == VALUES_LENGTH + 1, "Values length does not match");
    BA_STATIC_ASSERT(VALUES_LENGTH / VALUES_SPLIT_LENGTH == FAKE_ARGUMENTS_AMOUNT, "String is not dividable into equal parts");
    
    printf("--- Initializing fake arguments. Ignore any assert triggers as this isn't part of the API test\n");
    
    for (int i = 1; i < FAKE_ARGUMENTS_AMOUNT + 1; i++) {
        fakeArguments[i] = malloc(sizeof(char) * VALUES_SPLIT_LENGTH + 1);
        
        ASSERT(fakeArguments[i] != NULL);
        memcpy(fakeArguments[i], values + secondIndex, VALUES_SPLIT_LENGTH);
        
        fakeArguments[i][VALUES_SPLIT_LENGTH] = '\0';
        secondIndex += VALUES_SPLIT_LENGTH;
    }
    
    fakeArguments[FAKE_ARGUMENTS_AMOUNT + 1] = NULL;

    BA_ArgumentHandler_Initialize(8, fakeArguments);
    printf("--- Done initializing, testing API\n");
    {
        BA_DynamicArray array;
        int element = 10;
        int element2 = 20;
        int element3 = 30;

        printf("--- DynamicArray\n");
        ASSERT(BA_DynamicArray_Create(&array, 10));
        ASSERT(BA_DynamicArray_GetIndexForElement(&array, &element, sizeof(int)) == -1);
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element));
        ASSERT(BA_DynamicArray_GetIndexForElement(&array, &element, sizeof(int)) == 0);
        ASSERT(array.used == 1);
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_GetIndexForElement(&array, &element2, sizeof(int)) == 0);
        ASSERT(BA_DynamicArray_GetIndexForElement(&array, &element, sizeof(int)) == 1);
        ASSERT(BA_DynamicArray_AddElementToLast(&array, &element3));
        ASSERT(BA_DynamicArray_GetIndexForElement(&array, &element3, sizeof(int)) == 2);
        ASSERT(array.used == 3);
        ASSERT(BA_DynamicArray_RemoveFirstElement(&array));
        ASSERT(BA_DynamicArray_GetIndexForElement(&array, &element2, sizeof(int)) == -1);
        ASSERT(array.used == 2);
        ASSERT(BA_DynamicArray_RemoveLastElement(&array));
        ASSERT(BA_DynamicArray_GetIndexForElement(&array, &element3, sizeof(int)) == -1);
        ASSERT(array.used == 1);
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_RemoveElementAt(&array, 1));
        ASSERT(BA_DynamicArray_GetIndexForElement(&array, &element, sizeof(int)) == -1);
        ASSERT(array.used == 1);
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element));
        ASSERT(BA_DynamicArray_RemoveMatchedElement(&array, &element2, sizeof(int), BA_BOOLEAN_FALSE));
        ASSERT(BA_DynamicArray_GetIndexForElement(&array, &element2, sizeof(int)) == -1);
        ASSERT(array.used == 1);
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_RemoveMatchedElement(&array, &element2, sizeof(int), BA_BOOLEAN_TRUE));
        ASSERT(BA_DynamicArray_GetIndexForElement(&array, &element2, sizeof(int)) == -1);
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DYNAMICARRAY_GET_ELEMENT(int, array, 0) == &element2);
        ASSERT(BA_DYNAMICARRAY_GET_LAST_ELEMENT(int, array) == &element);
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(BA_DynamicArray_AddElementToStart(&array, &element2));
        ASSERT(array.used == 11);
        ASSERT(array.size == 20);
        ASSERT(array.calledReallocate == 1);
        ASSERT(BA_DynamicArray_Shrink(&array));
        ASSERT(array.size == array.used);

        array.frozen = BA_BOOLEAN_TRUE;

        ASSERT(!BA_DynamicArray_AddElementToStart(&array, &element));
        ASSERT(!BA_DynamicArray_AddElementToLast(&array, &element));
        ASSERT(!BA_DynamicArray_RemoveFirstElement(&array));
        ASSERT(!BA_DynamicArray_RemoveLastElement(&array));
        ASSERT(!BA_DynamicArray_RemoveElementAt(&array, 0));
        ASSERT(!BA_DynamicArray_RemoveMatchedElement(&array, &element2, sizeof(int), BA_BOOLEAN_FALSE));
        ASSERT(!BA_DynamicArray_Shrink(&array));
        free(array.internalArray);
    }

    {
        BA_DynamicDictionary dictionary;
        int element = 10;
        int elementValue = 20;
        int element2 = 30;
        int element2Value = 40;

        printf("--- DynamicDictionary\n");
        ASSERT(BA_DynamicDictionary_Create(&dictionary, 10));
        ASSERT(BA_DynamicDictionary_GetElementIndexFromKey(&dictionary, &element, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromValue(&dictionary, &elementValue, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementKeyViaValue(&dictionary, &elementValue, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_GetElementValueViaKey(&dictionary, &elementValue, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &element, &elementValue));
        ASSERT(dictionary.keys.size == dictionary.values.size);
        ASSERT(dictionary.keys.used == dictionary.values.used);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromKey(&dictionary, &element, sizeof(int)) == 0);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromValue(&dictionary, &elementValue, sizeof(int)) == 0);
        ASSERT(BA_DynamicDictionary_GetElementKeyViaValue(&dictionary, &elementValue, sizeof(int)) == &element);
        ASSERT(BA_DynamicDictionary_GetElementValueViaKey(&dictionary, &element, sizeof(int)) == &elementValue);
        ASSERT(BA_DynamicDictionary_AddElementToLast(&dictionary, &element2, &element2Value));
        ASSERT(dictionary.keys.size == dictionary.values.size);
        ASSERT(dictionary.keys.used == dictionary.values.used);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromKey(&dictionary, &element2, sizeof(int)) == 1);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromValue(&dictionary, &element2Value, sizeof(int)) == 1);
        ASSERT(BA_DynamicDictionary_GetElementKeyViaValue(&dictionary, &element2Value, sizeof(int)) == &element2);
        ASSERT(BA_DynamicDictionary_GetElementValueViaKey(&dictionary, &element2, sizeof(int)) == &element2Value);
        ASSERT(BA_DynamicDictionary_RemoveFirstElement(&dictionary));
        ASSERT(dictionary.keys.size == dictionary.values.size);
        ASSERT(dictionary.keys.used == dictionary.values.used);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromKey(&dictionary, &element, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromValue(&dictionary, &elementValue, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementKeyViaValue(&dictionary, &elementValue, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_GetElementValueViaKey(&dictionary, &elementValue, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &element, &elementValue));
        ASSERT(BA_DynamicDictionary_RemoveLastElement(&dictionary));
        ASSERT(dictionary.keys.size == dictionary.values.size);
        ASSERT(dictionary.keys.used == dictionary.values.used);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromKey(&dictionary, &element2, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromValue(&dictionary, &element2Value, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementKeyViaValue(&dictionary, &element2Value, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_GetElementValueViaKey(&dictionary, &element2, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &element2, &element2Value));
        ASSERT(BA_DynamicDictionary_RemoveElementViaKey(&dictionary, &element, sizeof(int), BA_BOOLEAN_FALSE));
        ASSERT(dictionary.keys.size == dictionary.values.size);
        ASSERT(dictionary.keys.used == dictionary.values.used);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromKey(&dictionary, &element, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromValue(&dictionary, &elementValue, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementKeyViaValue(&dictionary, &elementValue, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_GetElementValueViaKey(&dictionary, &elementValue, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &element, &elementValue));
        ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &element, &elementValue));
        ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &element, &elementValue));
        ASSERT(BA_DynamicDictionary_RemoveElementViaKey(&dictionary, &element, sizeof(int), BA_BOOLEAN_TRUE));
        ASSERT(dictionary.keys.size == dictionary.values.size);
        ASSERT(dictionary.keys.used == dictionary.values.used);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromKey(&dictionary, &element, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromValue(&dictionary, &elementValue, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementKeyViaValue(&dictionary, &elementValue, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_GetElementValueViaKey(&dictionary, &elementValue, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &element, &elementValue));
        ASSERT(BA_DynamicDictionary_RemoveElementViaValue(&dictionary, &element2Value, sizeof(int), BA_BOOLEAN_FALSE));
        ASSERT(dictionary.keys.size == dictionary.values.size);
        ASSERT(dictionary.keys.used == dictionary.values.used);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromKey(&dictionary, &element2, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromValue(&dictionary, &element2Value, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementKeyViaValue(&dictionary, &element2Value, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_GetElementValueViaKey(&dictionary, &element2, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &element2, &element2Value));
        ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &element2, &element2Value));
        ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &element2, &element2Value));
        ASSERT(BA_DynamicDictionary_RemoveElementViaValue(&dictionary, &element2Value, sizeof(int), BA_BOOLEAN_TRUE));
        ASSERT(dictionary.keys.size == dictionary.values.size);
        ASSERT(dictionary.keys.used == dictionary.values.used);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromKey(&dictionary, &element2, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementIndexFromValue(&dictionary, &element2Value, sizeof(int)) == -1);
        ASSERT(BA_DynamicDictionary_GetElementKeyViaValue(&dictionary, &element2Value, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_GetElementValueViaKey(&dictionary, &element2, sizeof(int)) == NULL);
        ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &element2, &element2Value));
        ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &element2, &element2Value));
        ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &element2, &element2Value));
        
        BA_DynamicDictionary found;

        ASSERT(BA_DynamicDictionary_Create(&found, 10));
        BA_DynamicDictionary_GetElementsKeyViaValue(&dictionary, &found, &element2Value, sizeof(int));
        ASSERT(found.keys.used == 3);
        ASSERT(found.keys.used == found.values.used);

        for (int i = 0; i < found.keys.used - 1; i++)
            ASSERT(found.keys.internalArray[i] == found.keys.internalArray[i + 1]);
        
        ASSERT(BA_DynamicDictionary_Shrink(&dictionary));
        ASSERT(dictionary.keys.size == dictionary.values.size);
        ASSERT(dictionary.keys.used == dictionary.values.used);
        ASSERT(BA_DYNAMICDICTIONARY_GET_KEY(int, &dictionary, &element2Value, sizeof(int)) == &element2);
        ASSERT(BA_DYNAMICDICTIONARY_GET_VALUE(int, &dictionary, &element2, sizeof(int)) == &element2Value);
        
        dictionary.frozen = BA_BOOLEAN_TRUE;

        ASSERT(!BA_DynamicDictionary_AddElementToStart(&dictionary, &element, &elementValue));
        ASSERT(dictionary.keys.frozen == dictionary.values.frozen);
        
        dictionary.keys.frozen = BA_BOOLEAN_FALSE;
        dictionary.values.frozen = BA_BOOLEAN_FALSE;
        
        ASSERT(!BA_DynamicDictionary_AddElementToLast(&dictionary, &element2, &element2Value));
        ASSERT(dictionary.keys.frozen == dictionary.values.frozen);

        dictionary.keys.frozen = BA_BOOLEAN_FALSE;
        dictionary.values.frozen = BA_BOOLEAN_FALSE;

        ASSERT(!BA_DynamicDictionary_RemoveFirstElement(&dictionary));
        ASSERT(dictionary.keys.frozen == dictionary.values.frozen);

        dictionary.keys.frozen = BA_BOOLEAN_FALSE;
        dictionary.values.frozen = BA_BOOLEAN_FALSE;

        ASSERT(!BA_DynamicDictionary_RemoveLastElement(&dictionary));
        ASSERT(dictionary.keys.frozen == dictionary.values.frozen);

        dictionary.keys.frozen = BA_BOOLEAN_FALSE;
        dictionary.values.frozen = BA_BOOLEAN_FALSE;

        ASSERT(!BA_DynamicDictionary_RemoveElementViaKey(&dictionary, &element, sizeof(int), BA_BOOLEAN_FALSE));
        ASSERT(dictionary.keys.frozen == dictionary.values.frozen);

        dictionary.keys.frozen = BA_BOOLEAN_FALSE;
        dictionary.values.frozen = BA_BOOLEAN_FALSE;

        ASSERT(!BA_DynamicDictionary_RemoveElementViaValue(&dictionary, &element2Value, sizeof(int), BA_BOOLEAN_FALSE));
        ASSERT(dictionary.keys.frozen == dictionary.values.frozen);

        dictionary.keys.frozen = BA_BOOLEAN_FALSE;
        dictionary.values.frozen = BA_BOOLEAN_FALSE;

        ASSERT(!BA_DynamicDictionary_Shrink(&dictionary));
        ASSERT(dictionary.keys.frozen == dictionary.values.frozen);
        free(found.keys.internalArray);
        free(found.values.internalArray);
    }

    {
        printf("--- ANSI\n");
        ASSERT(strcmp(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_BOLD), "\033[1m") == 0);
        ASSERT(strcmp(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_UNDERLINE), "\033[4m") == 0);
        ASSERT(strcmp(BA_ANSI_ConvertCodeToString(-1), "") == 0);
        ASSERT(strcmp(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_SIZE), "") == 0);
        ASSERT(strcmp(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_SIZE + 1), "") == 0);
    }

    {
        printf("--- ArgumentHandler\n");
        ASSERT(BA_ArgumentHandler_GetCount() == 8);
        ASSERT(BA_ArgumentHandler_GetVector() == fakeArguments);
        ASSERT(BA_ArgumentHandler_GetIndex("qw", BA_BOOLEAN_FALSE) == 1);
        ASSERT(BA_ArgumentHandler_GetIndex("er", BA_BOOLEAN_FALSE) == 2);
        ASSERT(BA_ArgumentHandler_GetIndex("as", BA_BOOLEAN_FALSE) == -1);
        ASSERT(BA_ArgumentHandler_GetIndex("as", BA_BOOLEAN_TRUE) == 6);
        ASSERT(strcmp(BA_ArgumentHandler_GetValue("qw", BA_BOOLEAN_FALSE), "er") == 0);
        ASSERT(BA_ArgumentHandler_GetValue("as", BA_BOOLEAN_FALSE) == NULL);
        ASSERT(strcmp(BA_ArgumentHandler_GetValue("as", BA_BOOLEAN_TRUE), "df") == 0);
    }

    {
        printf("--- String\n");
        
        char* string = BA_String_Copy("Hello, World!");
        
        ASSERT(strcmp(string, "Hello, World!") == 0);
        ASSERT(BA_String_Contains(string, ", Wor", BA_BOOLEAN_FALSE));
        ASSERT(!BA_String_Contains(string, ", wor", BA_BOOLEAN_FALSE));
        ASSERT(BA_String_Contains(string, ", wor", BA_BOOLEAN_TRUE));
        ASSERT(BA_String_Contains(string, ", Wor", BA_BOOLEAN_TRUE));
        ASSERT(!BA_String_Contains(string, "bye, Wor", BA_BOOLEAN_FALSE));
        ASSERT(!BA_String_Contains(string, "bye, wor", BA_BOOLEAN_TRUE));
        ASSERT(BA_String_Equals(string, "Hello, World!", BA_BOOLEAN_FALSE));
        ASSERT(!BA_String_Equals(string, "hello, world!", BA_BOOLEAN_FALSE));
        ASSERT(BA_String_Equals(string, "hello, world!", BA_BOOLEAN_TRUE));
        ASSERT(BA_String_Equals(string, "Hello, World!", BA_BOOLEAN_TRUE));
        ASSERT(!BA_String_Equals(string, "Bye, World", BA_BOOLEAN_FALSE));
        ASSERT(!BA_String_Equals(string, "bye, world", BA_BOOLEAN_TRUE));
        ASSERT(BA_String_StartsWith(string, "Hello,", BA_BOOLEAN_FALSE));
        ASSERT(!BA_String_StartsWith(string, "hello,", BA_BOOLEAN_FALSE));
        ASSERT(BA_String_StartsWith(string, "hello,", BA_BOOLEAN_TRUE));
        ASSERT(BA_String_StartsWith(string, "Hello,", BA_BOOLEAN_TRUE));
        ASSERT(BA_String_EndsWith(string, "World!", BA_BOOLEAN_FALSE));
        ASSERT(!BA_String_EndsWith(string, "world!", BA_BOOLEAN_FALSE));
        ASSERT(BA_String_EndsWith(string, "world!", BA_BOOLEAN_TRUE));
        ASSERT(BA_String_EndsWith(string, "World!", BA_BOOLEAN_TRUE));
        ASSERT(strcmp(BA_String_Append(&string, " How are you?"), "Hello, World! How are you?") == 0);
        ASSERT(strcmp(BA_String_Prepend(&string, "Good morning! "), "Good morning! Hello, World! How are you?") == 0);
        ASSERT(strcmp(BA_String_ToLower(string), "good morning! hello, world! how are you?") == 0);
        ASSERT(strcmp(BA_String_ToUpper(string), "GOOD MORNING! HELLO, WORLD! HOW ARE YOU?") == 0);
        ASSERT(strcmp(BA_String_Append(&string, " %s"), "GOOD MORNING! HELLO, WORLD! HOW ARE YOU? %s") == 0);
        ASSERT(strcmp(BA_String_Format(&string, "I'm doing fine!"), "GOOD MORNING! HELLO, WORLD! HOW ARE YOU? I'm doing fine!") == 0);
    
        BA_DynamicArray* array = BA_String_Split(string, " ");

        ASSERT(array != NULL);
        ASSERT(array->used == 10);
        ASSERT(strcmp(array->internalArray[0], "GOOD") == 0);
        ASSERT(strcmp(array->internalArray[1], "MORNING!") == 0);
        ASSERT(strcmp(array->internalArray[2], "HELLO,") == 0);
        ASSERT(strcmp(array->internalArray[3], "WORLD!") == 0);
        ASSERT(strcmp(array->internalArray[4], "HOW") == 0);
        ASSERT(strcmp(array->internalArray[5], "ARE") == 0);
        ASSERT(strcmp(array->internalArray[6], "YOU?") == 0);
        ASSERT(strcmp(array->internalArray[7], "I'm") == 0);
        ASSERT(strcmp(array->internalArray[8], "doing") == 0);
        ASSERT(strcmp(array->internalArray[9], "fine!") == 0);
        free(array->internalArray);
        free(array);

        FILE* file = fopen("test.txt", "r");

        if (file != NULL) {
            {
                size_t length;
                char* contents;

                ASSERT((contents = BA_String_ReadFile(file, 10, &length)) != NULL);
                ASSERT(strlen(contents) == length);
                ASSERT(length <= 10);
                printf("### test.txt -> %s\n", contents);
            }

            {
                char* line;
                ssize_t length;
                
                while ((length = BA_String_GetLine(file, &line, NULL)) != -1) {
                    ASSERT(length != -2);
                    ASSERT(line != NULL);
                    printf("### line -> %s\n", line);
                    free(line);
                }
            }
        } else
            printf("!!! Unable to find test.txt, skipping BA_String_ReadFile, BA_String_GetLine\n");

        ASSERT(strcmp(BA_String_Append(&string, " %s %s %s %s %s %s %s %s %s %s %s %s %s %s"), "GOOD MORNING! HELLO, WORLD! HOW ARE YOU? I'm doing fine! %s %s %s %s %s %s %s %s %s %s %s %s %s %s") == 0);
        ASSERT(strcmp(BA_String_FormatSafe(&string, 11,
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_STRING("That's good to hear!"),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_INTEGER(10),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_DOUBLE(12.0),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_CHARACTER('h'),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_LONG(100000),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_LONG_LONG(10000000000000),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED(100u),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_LONG_DOUBLE(200.0l),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED_CHARACTER('h'),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED_LONG(100000),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED_LONG_LONG(10000000000000)),
                      "GOOD MORNING! HELLO, WORLD! HOW ARE YOU? I'm doing fine! That's good to hear! 10 12.000000 h 100000 10000000000000 100 200.000000 h 100000 10000000000000 %s %s %s") == 0);
        ASSERT(BA_String_AddCustomSafeFormatter(1, BooleanFormat));
        ASSERT(strcmp(BA_String_FormatSafe(&string, 2, 1, 1, 1, 0), "GOOD MORNING! HELLO, WORLD! HOW ARE YOU? I'm doing fine! That's good to hear! 10 12.000000 h 100000 10000000000000 100 200.000000 h 100000 10000000000000 true false %s") == 0);
        ASSERT(strcmp(BA_String_Replace(&string, "MORNING", "EVENING"), "GOOD EVENING! HELLO, WORLD! HOW ARE YOU? I'm doing fine! That's good to hear! 10 12.000000 h 100000 10000000000000 100 200.000000 h 100000 10000000000000 true false %s") == 0);
        ASSERT(strcmp(BA_String_Replace(&string, "100000", "200000"), "GOOD EVENING! HELLO, WORLD! HOW ARE YOU? I'm doing fine! That's good to hear! 10 12.000000 h 200000 20000000000000 100 200.000000 h 200000 20000000000000 true false %s") == 0);
        ASSERT(strcmp(BA_String_Replace(&string, "EVENING", "EVE"), "GOOD EVE! HELLO, WORLD! HOW ARE YOU? I'm doing fine! That's good to hear! 10 12.000000 h 200000 20000000000000 100 200.000000 h 200000 20000000000000 true false %s") == 0);
        ASSERT(strcmp(BA_String_Replace(&string, "EVE", "EVENING"), "GOOD EVENING! HELLO, WORLD! HOW ARE YOU? I'm doing fine! That's good to hear! 10 12.000000 h 200000 20000000000000 100 200.000000 h 200000 20000000000000 true false %s") == 0);
        ASSERT(strcmp(BA_String_Replace(&string, "this should do nothing", "test"), "GOOD EVENING! HELLO, WORLD! HOW ARE YOU? I'm doing fine! That's good to hear! 10 12.000000 h 200000 20000000000000 100 200.000000 h 200000 20000000000000 true false %s") == 0);
        ASSERT(strcmp(BA_String_ReplaceCharacter(&string, 'd', 'g'), "GOOD EVENING! HELLO, WORLD! HOW ARE YOU? I'm going fine! That's goog to hear! 10 12.000000 h 200000 20000000000000 100 200.000000 h 200000 20000000000000 true false %s") == 0);
        ASSERT(strcmp(BA_String_ReplaceCharacter(&string, 'x', 'z'), "GOOD EVENING! HELLO, WORLD! HOW ARE YOU? I'm going fine! That's goog to hear! 10 12.000000 h 200000 20000000000000 100 200.000000 h 200000 20000000000000 true false %s") == 0);
    
        BA_DynamicArray* splitString = BA_String_Split("Hello, World!", " ");

        ASSERT(strcmp(BA_String_Join(splitString, "  "), "Hello,  World!") == 0);
        ASSERT(strcmp(BA_String_JoinCharacter(splitString, ' '), "Hello, World!") == 0);
        ASSERT(strcmp(BA_String_JoinCharacter(splitString, '_'), "Hello,_World!") == 0);
    };

    {
        printf("--- Number\n");
        
#define NUMBER_FUNCTION(to, string, result, error, endPointerResult) \
do {                                                                 \
    char* endPointer;                                                \
    BA_Boolean isError;                                              \
    ASSERT(BA_Number_StringTo ## to(string, &endPointer, &isError, NULL, 0) == result); \
    ASSERT(isError == error);                                        \
    ASSERT(strcmp(endPointer, endPointerResult) == 0);               \
} while (BA_BOOLEAN_FALSE)
        
#define NUMBER_FUNCTION_TEST(to) \
do {                             \
        NUMBER_FUNCTION(to, "1", 1, BA_BOOLEAN_FALSE, ""); \
        NUMBER_FUNCTION(to, "test", 0, BA_BOOLEAN_TRUE, "test"); \
        NUMBER_FUNCTION(to, "1test", 0, BA_BOOLEAN_TRUE, "test"); \
} while (0)
        
        NUMBER_FUNCTION_TEST(Character);
        NUMBER_FUNCTION_TEST(UnsignedCharacter);
        NUMBER_FUNCTION_TEST(Short);
        NUMBER_FUNCTION_TEST(UnsignedShort);
        NUMBER_FUNCTION_TEST(Integer);
        NUMBER_FUNCTION_TEST(Unsigned);
    }
    
    printf("+++ Success\n");
}