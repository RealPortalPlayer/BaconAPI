// Copyright (c) 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconAPI/Internal/OperatingSystem.h"

#if BA_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "BaconAPI/String.h"
#include "BaconAPI/Debugging/Assert.h"
#include "BaconAPI/Storage/DynamicDictionary.h"
#include "BaconAPI/Internal/OperatingSystem.h"

#if BA_OPERATINGSYSTEM_WINDOWS
#   define strtok_r strtok_s // I hate Microshit so much. Stop trying to be unique and quirky
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static BA_DynamicDictionary baStringCustomFormatters;
static BA_Boolean baStringInitializedCustomFormatters = BA_BOOLEAN_FALSE;

// FIXME: Some of these functions return NULL on allocation failure. Some people don't want to crash their program on
//        OOM. This would inconvenience them. No easy way to fix without masking memory issues. Such is life

#define BA_STRING_GET_STRING_COMPARE(stringLength, compareLength) \
char* heapString = calloc(stringLength + 1, 1);                   \
char* heapCompare = calloc(compareLength + 1, 1);                 \
memcpy(heapString, string, stringLength);                         \
memcpy(heapCompare, compare, compareLength)

char* BA_String_Copy(const char* duplicateFrom) {
    char* string = calloc(sizeof(char) * (strlen(duplicateFrom) + 1), 1);

    if (string == NULL)
        return NULL;
    
    return strcpy(string, duplicateFrom);
}

char* BA_String_Append(char* target, const char* stringToAppend) {
    size_t length = sizeof(char) * (strlen(target) + strlen(stringToAppend) + 1);
    char* clonedTarget = calloc(length, 1);

    if (clonedTarget == NULL)
        return NULL;
    
    strcpy(clonedTarget, target);
    strcat(clonedTarget, stringToAppend);
    free(target);
    return clonedTarget;
}

char* BA_String_Prepend(char* target, const char* stringToPrepend) {
    char* copiedStringToPrepend = BA_String_Copy(stringToPrepend);

    if (copiedStringToPrepend == NULL)
        return NULL;
    
    copiedStringToPrepend = BA_String_Append(copiedStringToPrepend, target);
    
    free(target);
    return copiedStringToPrepend;
}

BA_Boolean BA_String_StartsWith(const char* string, const char* compare, BA_Boolean caseless) {
    if (string == compare)
        return BA_BOOLEAN_TRUE;

    size_t compareLength = strlen(compare);

    if (strlen(string) < compareLength)
        return BA_BOOLEAN_FALSE;

    if (!caseless)
        return strncmp(string, compare, compareLength) == 0;
    
    BA_STRING_GET_STRING_COMPARE(compareLength, compareLength);

    BA_Boolean result = strncmp(BA_String_ToLower(heapString), BA_String_ToLower(heapCompare), compareLength) == 0;

    free(heapString);
    free(heapCompare);
    return result;
}

BA_Boolean BA_String_EndsWith(const char* string, const char* compare, BA_Boolean caseless) {
    if (string == compare)
        return BA_BOOLEAN_TRUE;
    
    size_t stringLength = strlen(string);
    size_t compareLength = strlen(compare);

    if (stringLength < compareLength)
        return BA_BOOLEAN_FALSE;
    
    if (!caseless)
        return strncmp(string + (stringLength - compareLength), compare, compareLength) == 0;

    BA_STRING_GET_STRING_COMPARE(stringLength, compareLength);

    BA_Boolean result = strncmp(BA_String_ToLower(heapString + (stringLength - compareLength)), BA_String_ToLower(heapCompare), compareLength) == 0;

    free(heapString);
    free(heapCompare);
    return result;
}

char* BA_String_ToLower(char* string) {
    for (size_t i = 0; i < strlen(string); i++)
        string[i] = (char) tolower(string[i]);
    
    return string;
}

char* BA_String_ToUpper(char* string) {
    for (size_t i = 0; i < strlen(string); i++)
        string[i] = (char) toupper(string[i]);
    
    return string;
}

BA_Boolean BA_String_Contains(const char* string, const char* compare, BA_Boolean caseless) {
    if (string == compare)
        return BA_BOOLEAN_TRUE;

    size_t stringLength = strlen(string);
    size_t compareLength = strlen(compare);
    
    if (stringLength < compareLength)
        return BA_BOOLEAN_FALSE;

    BA_STRING_GET_STRING_COMPARE(stringLength, compareLength);

    if (caseless) {
        BA_String_ToLower(heapString);
        BA_String_ToLower(heapCompare);
    }
    
    for (int i = 0; i < stringLength; i++) {
        if (strncmp(heapString + i, heapCompare, compareLength) != 0)
            continue;

        free(heapString);
        free(heapCompare);
        return BA_BOOLEAN_TRUE;
    }

    free(heapString);
    free(heapCompare);
    return BA_BOOLEAN_FALSE;
}

BA_Boolean BA_String_ContainsCharacter(const char* string, char compare, BA_Boolean caseless) {
    char temporaryString[2] = {compare, '\0'};

    return BA_String_Contains(string, temporaryString, caseless);
}

BA_Boolean BA_String_Equals(const char* string, const char* compare, BA_Boolean caseless) {
    if (!caseless)
        return strcmp(string, compare) == 0;

    size_t stringLength = strlen(string);

    if (stringLength != strlen(compare))
        return BA_BOOLEAN_FALSE;
    
    BA_STRING_GET_STRING_COMPARE(stringLength, stringLength);

    BA_Boolean result = strncmp(BA_String_ToLower(heapString), BA_String_ToLower(heapCompare), stringLength) == 0;

    free(heapString);
    free(heapCompare);
    return result;
}

char* BA_String_AppendCharacter(char* target, char character) {
    char temporaryString[2] = {character, '\0'};
    
    return BA_String_Append(target, temporaryString);
}

char* BA_String_PrependCharacter(char* target, char character) {
    char temporaryString[2] = {character, '\0'};
    
    return BA_String_Prepend(target, temporaryString);
}

char* BA_String_Format(char* target, ...) {
    va_list arguments;
    
    va_start(arguments, target);
    
    target = BA_String_FormatPremadeList(target, arguments);
    
    va_end(arguments);
    return target;
}

char* BA_String_FormatPremadeList(char* target, va_list arguments) {
    va_list argumentsCopy;

    va_copy(argumentsCopy, arguments);

    size_t newLength = vsnprintf(NULL, 0, target, arguments) + 1;
    char* newBuffer = calloc(sizeof(char) * newLength, 1);

    if (newBuffer == NULL) {
        va_end(argumentsCopy);
        return NULL;
    }
    
    vsnprintf(newBuffer, newLength, target, argumentsCopy);
    va_end(argumentsCopy);
    free(target);
    return newBuffer;
}

BA_DynamicArray* BA_String_Split(const char* target, const char* splitBy) {
    BA_DynamicArray* dynamicArray = malloc(sizeof(BA_DynamicArray));
    size_t targetLength = strlen(target);
    char* heapTarget = calloc(targetLength + 1, 1);

    BA_DynamicArray_Create(dynamicArray, 100);

    if (dynamicArray == NULL) {
        free(heapTarget);
        return NULL;
    }
    
    memcpy(heapTarget, target, targetLength);

    heapTarget[targetLength] = '\0';

    char* stackTargetPointer = &(heapTarget[0]);
    char* token = strtok_r(heapTarget, splitBy, &stackTargetPointer);

    while (token != NULL) {
        BA_DynamicArray_AddElementToLast(dynamicArray, BA_String_Copy(token));
        
        token = strtok_r(stackTargetPointer, splitBy, &stackTargetPointer);
    }

    free(heapTarget);
    return dynamicArray;
}

BA_DynamicArray* BA_String_SplitCharacter(const char* target, char splitBy) {
    char temporaryString[2] = {splitBy, '\0'};
    
    return BA_String_Split(target, temporaryString);
}

char* BA_String_ReadFile(FILE* file, size_t lengthLimit, size_t* lineLength) {
    fseek(file, 0, SEEK_END);
    
    size_t numberOfBytes = ftell(file);

    fseek(file, 0, SEEK_SET);

    if (lengthLimit != 0 && numberOfBytes > lengthLimit)
        numberOfBytes = lengthLimit;
    
    char* buffer = calloc(sizeof(char) * numberOfBytes, 1);
    
    if (buffer == NULL)
        return NULL;

    if (lineLength != NULL)
        *lineLength = numberOfBytes;

    buffer[0] = '\0';

    fread(buffer, sizeof(char), numberOfBytes, file);
    fseek(file, 0, SEEK_SET);

    buffer[numberOfBytes] = '\0';
    return buffer;
}

intmax_t BA_String_GetLine(FILE* file, char** line, const char* splitString) {
    const char* currentSplitString = splitString != NULL ? splitString : "\n";
    intmax_t length = 0;
    char* buffer = NULL;
    intmax_t splitStringLength = (int) strlen(currentSplitString);

    if (line != NULL) {
        buffer = BA_String_CreateEmpty();
                
        if (buffer == NULL)
            return -2;
    }
    
    while (!feof(file)) {
        char* contents = calloc(splitStringLength + 1, 1);

        contents[0] = '\0';
        contents[splitStringLength] = '\0';
        
        fread(contents, sizeof(char), splitStringLength, file);

        if (BA_String_Equals(contents, currentSplitString, BA_BOOLEAN_FALSE)) {
            if (line != NULL)
                *line = buffer;

            free(contents);
            return length;
        }
        
        length += splitStringLength;

        if (line == NULL) {
            free(contents);
            continue;
        }
        
        buffer = BA_String_Append(buffer, contents);
        
        free(contents);
    }

    if (line != NULL && buffer[0] != '\0') {
        *line = buffer;
        return length;
    }
    
    return -1;
}

intmax_t BA_String_GetLineCharacter(FILE* file, char** line, char splitCharacter) {
    const char temporaryString[2] = {splitCharacter, '\0'};
    
    return BA_String_GetLine(file, line, temporaryString);
}

char* BA_String_FormatSafe(char* target, int amountOfFormatters, ...) {
    va_list arguments;

    va_start(arguments, amountOfFormatters);
    
    target = BA_String_FormatSafePremadeList(target, amountOfFormatters, arguments);

    va_end(arguments);
    return target;
}

char* BA_String_FormatSafePremadeList(char* target, int amountOfFormatters, va_list arguments) {
    if (!baStringInitializedCustomFormatters) {
        if (!BA_DynamicDictionary_Create(&baStringCustomFormatters, 10))
            return NULL;

        baStringInitializedCustomFormatters = BA_BOOLEAN_TRUE;
    }
    
    size_t originalTargetSize = strlen(target);

    if (originalTargetSize == 0 || amountOfFormatters <= 0)
        return target;

    char* newBuffer = BA_String_CreateEmpty();

    if (newBuffer == NULL)
        return NULL;
    
    BA_Boolean percentageFound = BA_BOOLEAN_FALSE;
    int usedArguments = 0;
    int lastSuccessfulIdentifier = 0;
    
    for (int i = 0; i < originalTargetSize; i++) {
        char character = target[i];

        if (character == '%') {
            if (!percentageFound) {
                percentageFound = BA_BOOLEAN_TRUE;
                continue;
            }

            newBuffer = BA_String_Append(newBuffer, "%%");

            percentageFound = BA_BOOLEAN_FALSE;
            continue;
        }

        if (percentageFound) {
            percentageFound = BA_BOOLEAN_FALSE;

            if (character == 's') {
                if (usedArguments >= amountOfFormatters) {
                    newBuffer = BA_String_Append(newBuffer, "%s");
                    continue;
                }

                usedArguments++;

#define BA_STRING_CONVERT_AND_APPEND(type, formatSpecifier) \
do {                                                        \
    type value = va_arg(arguments, type);                   \
    size_t bufferSize = snprintf(NULL, 0, formatSpecifier, value); \
    char* buffer = calloc(bufferSize, 1);                   \
    snprintf(buffer, bufferSize + 1, formatSpecifier, value); \
    newBuffer = BA_String_Append(newBuffer, buffer);        \
    free(buffer);                                           \
} while (0)

                BA_String_SafeFormatTypes identifier = va_arg(arguments, BA_String_SafeFormatTypes);
                
                switch (identifier) {
                    case BA_STRING_SAFE_FORMAT_TYPE_STRING: newBuffer = BA_String_Append(newBuffer, va_arg(arguments, char*)); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_INTEGER: BA_STRING_CONVERT_AND_APPEND(int, "%d"); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_DOUBLE: BA_STRING_CONVERT_AND_APPEND(double, "%lf"); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_CHARACTER: newBuffer = BA_String_AppendCharacter(newBuffer, (char) va_arg(arguments, int)); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_LONG: BA_STRING_CONVERT_AND_APPEND(long, "%li"); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_LONG_LONG: BA_STRING_CONVERT_AND_APPEND(long long, "%lli"); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_SHORT: BA_STRING_CONVERT_AND_APPEND(int, "%hi"); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED: BA_STRING_CONVERT_AND_APPEND(unsigned, "%u"); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_LONG_DOUBLE: BA_STRING_CONVERT_AND_APPEND(long double, "%Lf"); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_CHARACTER: BA_STRING_CONVERT_AND_APPEND(unsigned int, "%c"); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_LONG: BA_STRING_CONVERT_AND_APPEND(unsigned long, "%lu"); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_UNSIGNED_LONG_LONG: BA_STRING_CONVERT_AND_APPEND(unsigned long long, "%llu"); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_POINTER: BA_STRING_CONVERT_AND_APPEND(void*, "%p"); break;
                    default:
                    {
                        BA_String_CustomSafeFormatAction actionFunction = (BA_String_CustomSafeFormatAction) BA_DYNAMICDICTIONARY_GET_VALUE(BA_String_CustomSafeFormatAction, &baStringCustomFormatters, &identifier, sizeof(int));

                        if (actionFunction != NULL) {
                            void* argument = va_arg(arguments, void*);
                            
                            newBuffer = actionFunction(newBuffer, &argument);
                            break;
                        }

                        BA_ASSERT_ALWAYS("Specified type is unrecognized:\nIdentifier: %i\nPrevious identifier: %i\n", identifier, lastSuccessfulIdentifier);
                    }
                }
                
#undef BA_STRING_CONVERT_AND_APPEND

                lastSuccessfulIdentifier = identifier;
                continue;
            }

            newBuffer = BA_String_AppendCharacter(newBuffer, '%');
        }

        newBuffer = BA_String_AppendCharacter(newBuffer, character);
    }

    if (percentageFound)
        newBuffer = BA_String_AppendCharacter(newBuffer, '%');

    free(target);
    return newBuffer;
}

char* BA_String_CreateEmpty(void) {
    return calloc(sizeof(char), 1);
}

BA_Boolean BA_String_AddCustomSafeFormatter(int identifier, BA_String_CustomSafeFormatAction actionFunction) {
    if (!baStringInitializedCustomFormatters) {
        if (!BA_DynamicDictionary_Create(&baStringCustomFormatters, 10))
            return BA_BOOLEAN_FALSE;
        
        baStringInitializedCustomFormatters = BA_BOOLEAN_TRUE;
    }
    
    int* identifierPointer = malloc(sizeof(int));

    if (identifierPointer == NULL)
        return BA_BOOLEAN_FALSE;
    
    *identifierPointer = identifier;
    return BA_DynamicDictionary_AddElementToLast(&baStringCustomFormatters, identifierPointer, actionFunction);
}

char* BA_String_Replace(char* target, const char* what, const char* to) {
    size_t targetLength = strlen(target);
    size_t whatLength = strlen(what);
    size_t toLength = strlen(to);

    if (targetLength < whatLength)
        return target;
    
    for (int i = 0; i < targetLength; i++) {
        if (strncmp(target + i, what, whatLength) != 0)
            continue;

        size_t savedStringLength = targetLength - (i + whatLength);
        char* temporaryString = calloc(savedStringLength + 1, 1);
        
        strncpy(temporaryString, target + i + whatLength, savedStringLength);
        
        if (whatLength != toLength) {
            targetLength += toLength - whatLength;
            target = realloc(target, sizeof(char) * targetLength + 1);
            target[targetLength] = '\0';
        }
        
        strncpy(target + i, to, toLength);
        strncpy(target + i + toLength, temporaryString, savedStringLength);
        free(temporaryString);
    }
    
    return target;
}

char* BA_String_ReplaceCharacter(char* target, char what, char to) {
    const char temporaryWhat[2] = {what, '\0'};
    const char temporaryTo[2] = {to, '\0'};
    
    return BA_String_Replace(target, temporaryWhat, temporaryTo);
}

char* BA_String_Join(const BA_DynamicArray* dynamicArray, const char* joinString) {
    char* finalString = BA_String_CreateEmpty();
    
    for (int i = 0; i < dynamicArray->used - 1; i++)
        finalString = BA_String_Append(BA_String_Append(finalString, BA_DYNAMICARRAY_GET_ELEMENT_POINTER(char, dynamicArray, i)), joinString);
    
    return BA_String_Append(finalString, BA_DYNAMICARRAY_GET_LAST_ELEMENT_POINTER(char, dynamicArray));
}

char* BA_String_JoinCharacter(const BA_DynamicArray* dynamicArray, char joinCharacter) {
    char temporaryString[2] = {joinCharacter, '\0'};
    
    return BA_String_Join(dynamicArray, temporaryString);
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
