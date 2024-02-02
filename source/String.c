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

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static BA_DynamicDictionary baStringCustomFormatters;
static BA_Boolean baStringInitializedCustomFormatters = BA_BOOLEAN_FALSE;

#define BA_STRING_GET_STACK_STRING_COMPARE(stringLength, compareLength) \
char stackString[stringLength + 1];                                     \
char stackCompare[compareLength + 1];                                   \
memcpy(stackString, string, stringLength);                              \
memcpy(stackCompare, compare, compareLength);                           \
stackString[stringLength + 1] = '\0';                                   \
stackCompare[compareLength + 1] = '\0'

char* BA_String_Copy(const char* duplicateFrom) {
    char* string = malloc(sizeof(char) * (strlen(duplicateFrom) + 1));

    if (string == NULL)
        return NULL;
    
    strcpy(string, duplicateFrom);
    return string;
}

char* BA_String_Append(char** target, const char* stringToAppend) {
    char* reallocatedString = realloc(*target, sizeof(char) * (strlen(*target) + strlen(stringToAppend) + 1));

    if (reallocatedString == NULL)
        return NULL;
    
    *target = reallocatedString;
    
    strcat(*target, stringToAppend);
    return *target;
}

char* BA_String_Prepend(char** target, const char* stringToPrepend) {
    char* copiedStringToPrepend = BA_String_Copy(stringToPrepend);

    if (copiedStringToPrepend == NULL)
        return NULL;
    
    BA_String_Append(&copiedStringToPrepend, *target);
    free(*target);
    
    *target = copiedStringToPrepend;
    return *target;
}

BA_Boolean BA_String_StartsWith(const char* string, const char* compare, BA_Boolean caseless) {
    if (string == compare)
        return BA_BOOLEAN_TRUE;

    size_t compareLength = strlen(compare);

    if (strlen(string) < compareLength)
        return BA_BOOLEAN_FALSE;

    if (!caseless)
        return strncmp(string, compare, compareLength) == 0;
    
    BA_STRING_GET_STACK_STRING_COMPARE(compareLength, compareLength);
    return strncmp(BA_String_ToLower(stackString), BA_String_ToLower(stackCompare), compareLength) == 0;
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

    BA_STRING_GET_STACK_STRING_COMPARE(stringLength, compareLength);
    return strncmp(BA_String_ToLower(stackString + (stringLength - compareLength)), BA_String_ToLower(stackCompare), compareLength) == 0;
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

    BA_STRING_GET_STACK_STRING_COMPARE(stringLength, compareLength);

    if (caseless) {
        memcpy(stackString, BA_String_ToLower(stackString), stringLength);
        memcpy(stackCompare, BA_String_ToLower(stackCompare), compareLength);
    }
    
    for (int i = 0; i < stringLength; i++) {
        if (strncmp(stackString + i, stackCompare, compareLength) != 0)
            continue;
        
        return BA_BOOLEAN_TRUE;
    }

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
    
    BA_STRING_GET_STACK_STRING_COMPARE(stringLength, stringLength);
    return strncmp(BA_String_ToLower(stackString), BA_String_ToLower(stackCompare), stringLength) == 0;
}

char* BA_String_AppendCharacter(char** target, char character) {
    char temporaryString[2] = {character, '\0'};
    
    return BA_String_Append(target, temporaryString);
}

char* BA_String_PrependCharacter(char** target, char character) {
    char temporaryString[2] = {character, '\0'};
    
    return BA_String_Prepend(target, temporaryString);
}

char* BA_String_Format(char** target, ...) {
    va_list arguments;
    
    va_start(arguments, target);
    
    char* returnValue = BA_String_FormatPremadeList(target, arguments);
    
    va_end(arguments);
    return returnValue;
}

char* BA_String_FormatPremadeList(char** target, va_list arguments) {
    va_list argumentsCopy;

    va_copy(argumentsCopy, arguments);

    size_t newLength = vsnprintf(NULL, 0, *target, arguments) + 1;
    char* newBuffer = malloc(sizeof(char) * newLength);

    if (newBuffer == NULL) {
        va_end(argumentsCopy);
        return NULL;
    }
    
    vsnprintf(newBuffer, newLength, *target, argumentsCopy);
    va_end(argumentsCopy);

    *target = newBuffer;
    return *target;
}

BA_DynamicArray* BA_String_Split(const char* target, const char* splitBy) {
    BA_DynamicArray* dynamicArray = malloc(sizeof(BA_DynamicArray));
    size_t targetLength = strlen(target);
    size_t splitByLength = strlen(splitBy);
    const char* target2 = target;

    if (dynamicArray == NULL)
        return NULL;

    BA_DynamicArray_Create(dynamicArray, 100);
    
    if (targetLength < splitByLength)
        return dynamicArray;
    
    for (int i = 0, j = 0; i <= targetLength; i++) {
        if (strncmp(target + i, splitBy, splitByLength) != 0) {
            j++;

            if (i != targetLength)
                continue;
        }
        
        char* string = malloc(sizeof(char) * (j != 0 ? j : 1));
        
        if (string == NULL) {
            for (int k = 0; k < dynamicArray->used; k++)
                free(dynamicArray->internalArray[k]);

            free(dynamicArray->internalArray);
            return NULL;
        };

        string[j] = '\0';
        
        memcpy(string, target2, j);
        BA_DynamicArray_AddElementToLast(dynamicArray, string);
        
        target2 += j + splitByLength;
        j = 0;
    }
    
    return dynamicArray;
}

BA_DynamicArray* BA_String_SplitCharacter(const char* target, char splitBy) {
    char temporaryString[2] = {splitBy, '\0'};
    
    return BA_String_Split(target, temporaryString);
}

char* BA_String_ReadFile(FILE* file, size_t lengthLimit, size_t* lineLength) {
    size_t numberOfBytes;

    fseek(file, 0, SEEK_END);
    
    numberOfBytes = ftell(file);

    fseek(file, 0, SEEK_SET);

    if (lengthLimit != 0 && numberOfBytes > lengthLimit)
        numberOfBytes = lengthLimit;
    
    char* buffer = malloc(sizeof(char) * numberOfBytes);
    
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

ssize_t BA_String_GetLine(FILE* file, char** line, const char* splitString) {
    const char* currentSplitString = splitString != NULL ? splitString : "\n";
    ssize_t length = 0;
    char* buffer = NULL;
    ssize_t splitStringLength = (int) strlen(currentSplitString);

    if (line != NULL) {
        buffer = BA_String_CreateEmpty();
                
        if (buffer == NULL)
            return -2;
    }
    
    while (!feof(file)) {
        char contents[splitStringLength + 1];
        
        contents[0] = '\0';
        contents[splitStringLength] = '\0';
        
        fread(contents, sizeof(char), splitStringLength, file);

        if (BA_String_Equals(contents, currentSplitString, BA_BOOLEAN_FALSE)) {
            if (line != NULL)
                *line = buffer;
            
            return length;
        }
        
        length += splitStringLength;

        if (line == NULL)
            continue;
        
        BA_String_Append(&buffer, BA_String_Copy(contents));
    }

    if (line != NULL && buffer[0] != '\0') {
        *line = buffer;
        return length;
    }
    
    return -1;
}

ssize_t BA_String_GetLineCharacter(FILE* file, char** line, char splitCharacter) {
    const char temporaryString[2] = {splitCharacter, '\0'};
    
    return BA_String_GetLine(file, line, temporaryString);
}

char* BA_String_FormatSafe(char** target, int amountOfFormatters, ...) {
    va_list arguments;

    va_start(arguments, amountOfFormatters);
    
    char* returnValue = BA_String_FormatSafePremadeList(target, amountOfFormatters, arguments);

    va_end(arguments);
    return returnValue;
}

char* BA_String_FormatSafePremadeList(char** target, int amountOfFormatters, va_list arguments) {
    if (!baStringInitializedCustomFormatters) {
        if (!BA_DynamicDictionary_Create(&baStringCustomFormatters, 10))
            return NULL;

        baStringInitializedCustomFormatters = BA_BOOLEAN_TRUE;
    }
    
    size_t originalTargetSize = strlen(*target);

    if (originalTargetSize == 0 || amountOfFormatters <= 0)
        return *target;

    char* newBuffer = BA_String_CreateEmpty();

    if (newBuffer == NULL)
        return NULL;
    
    BA_Boolean percentageFound = BA_BOOLEAN_FALSE;
    int usedArguments = 0;
    int lastSuccessfulIdentifier = 0;
    
    for (int i = 0; i < originalTargetSize; i++) {
        char character = (*target)[i];

        if (character == '%') {
            if (!percentageFound) {
                percentageFound = BA_BOOLEAN_TRUE;
                continue;
            }

            BA_String_Append(&newBuffer, "%%");

            percentageFound = BA_BOOLEAN_FALSE;
            continue;
        }

        if (percentageFound) {
            percentageFound = BA_BOOLEAN_FALSE;

            if (character == 's') {
                if (usedArguments >= amountOfFormatters) {
                    BA_String_Append(&newBuffer, "%s");
                    continue;
                }

                usedArguments++;

#define BA_STRING_CONVERT_AND_APPEND(type, formatSpecifier) \
do {                                                        \
    type value = va_arg(arguments, type);                   \
    size_t bufferSize = snprintf(NULL, 0, formatSpecifier, value); \
    char buffer[bufferSize];                                \
    snprintf(buffer, bufferSize + 1, formatSpecifier, value); \
    BA_String_Append(&newBuffer, buffer);                   \
} while (0)

                BA_String_SafeFormatTypes identifier = va_arg(arguments, BA_String_SafeFormatTypes);
                
                switch (identifier) {
                    case BA_STRING_SAFE_FORMAT_TYPE_STRING: BA_String_Append(&newBuffer, va_arg(arguments, char*)); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_INTEGER: BA_STRING_CONVERT_AND_APPEND(int, "%d"); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_DOUBLE: BA_STRING_CONVERT_AND_APPEND(double, "%lf"); break;
                    case BA_STRING_SAFE_FORMAT_TYPE_CHARACTER: BA_String_AppendCharacter(&newBuffer, (char) va_arg(arguments, int)); break;
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
                            
                            actionFunction(&newBuffer, &argument);
                            break;
                        }

                        BA_ASSERT_ALWAYS("Specified type is unrecognized:\nIdentifier: %i\nPrevious identifier: %i\n", identifier, lastSuccessfulIdentifier);
                    }
                }
                
#undef BA_STRING_CONVERT_AND_APPEND

                lastSuccessfulIdentifier = identifier;
                continue;
            }

            BA_String_AppendCharacter(&newBuffer, '%');
        }

        BA_String_AppendCharacter(&newBuffer, character);
    }

    if (percentageFound)
        BA_String_AppendCharacter(&newBuffer, '%');

    *target = newBuffer;
    return *target;
}

char* BA_String_CreateEmpty(void) {
    char* string = malloc(sizeof(char));

    if (string == NULL)
        return NULL;
    
    string[0] = '\0';
    return string;
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

    BA_DynamicDictionary_AddElementToLast(&baStringCustomFormatters, identifierPointer, actionFunction);
    return BA_BOOLEAN_TRUE;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
