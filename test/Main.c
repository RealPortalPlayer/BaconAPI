// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdio.h>
#include <BaconAPI/Storage/DynamicArray.h>
#include <BaconAPI/Internal/Boolean.h>
#include <stdlib.h>
#include <BaconAPI/Storage/DynamicDictionary.h>
#include <BaconAPI/ANSI.h>
#include <string.h>
#include <BaconAPI/BuiltInArguments.h>
#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/String.h>

#define ASSERT(check) \
do {                  \
    printf("%s", #check); \
    if (!(check)) {   \
        printf(": FAIL\n"); \
        abort();      \
    }                 \
    printf("\n");     \
} while (0)

int main(void) {
    char** fakeArguments = malloc(sizeof(char*) * 10);
    char values[] = "qwertyui--asdf";
    int secondIndex = 0;
    
    fakeArguments[1] = malloc(sizeof(char) * strlen(BA_BUILTINARGUMENTS_DISABLE_ANSI_COLORING));

    printf("--- Initializing fake arguments. Ignore any assert triggers as this isn't part of the API test\n");
    ASSERT(fakeArguments[1] != NULL);
    strcpy(fakeArguments[1], BA_BUILTINARGUMENTS_DISABLE_ANSI_COLORING);
    
    for (int i = 2; i < 9; i++) {
        fakeArguments[i] = malloc(sizeof(char) * 3);

        ASSERT(fakeArguments[i] != NULL);
        memcpy(fakeArguments[i], values + secondIndex, 2);
        
        secondIndex += 2;
    }
    
    fakeArguments[9] = NULL;

    BA_ArgumentHandler_Initialize(9, fakeArguments);
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
        ASSERT(BA_ArgumentHandler_GetCount() == 9);
        ASSERT(BA_ArgumentHandler_GetVector() == fakeArguments);
        ASSERT(BA_ArgumentHandler_GetIndex("qw", BA_BOOLEAN_FALSE) == 2);
        ASSERT(BA_ArgumentHandler_GetIndex("er", BA_BOOLEAN_FALSE) == 3);
        ASSERT(BA_ArgumentHandler_GetIndex("as", BA_BOOLEAN_FALSE) == -1);
        ASSERT(BA_ArgumentHandler_GetIndex("as", BA_BOOLEAN_TRUE) == 7);
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
                    printf("### line -> %s\n", line);
                    free(line);
                }
            }
        } else
            printf("!!! Unable to find test.txt, skipping BA_String_ReadFile, BA_String_GetLine\n");

        ASSERT(strcmp(BA_String_Append(&string, " %s %s %s %s %s %s %s %s"), "GOOD MORNING! HELLO, WORLD! HOW ARE YOU? I'm doing fine! %s %s %s %s %s %s %s %s") == 0);
        ASSERT(strcmp(BA_String_FormatSafe(&string, 7,
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_STRING("That's good to hear!"),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_INTEGER(10),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_DOUBLE(12.0),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_CHARACTER('h'),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_LONG(10000000000000),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_LONG_LONG(1000000000000000000),
                                           BA_STRING_FORMAT_SAFE_ARGUMENT_UNSIGNED(100)),
                      "GOOD MORNING! HELLO, WORLD! HOW ARE YOU? I'm doing fine! That's good to hear! 10 12.000000 h 10000000000000 1000000000000000000 100 %s") == 0);
    }

    printf("+++ Success\n");
}