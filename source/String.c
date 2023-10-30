// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
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

BA_CPLUSPLUS_SUPPORT_GUARD_START()
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
    size_t targetLength = strlen(*target);
    size_t stringToPrependLength = strlen(stringToPrepend);
    char* reallocatedString = realloc(*target, sizeof(char) * (targetLength + stringToPrependLength));
    
    if (reallocatedString == NULL)
        return NULL;
    
    *target = reallocatedString;

    for (size_t i = targetLength; i > 0 ; i--)
        (*target)[i + stringToPrependLength - 1] = (*target)[i - 1];

    memcpy(*target, stringToPrepend, stringToPrependLength);
    
    (*target)[targetLength + stringToPrependLength] = 0;
    return *target;
}

BA_Boolean BA_String_StartsWith(const char* string, const char* compare, BA_Boolean caseless) {
    if (string == compare)
        return BA_BOOLEAN_TRUE;

    size_t stringLength = strlen(string);
    size_t compareLength = strlen(compare);
    
    if (stringLength < compareLength)
        return BA_BOOLEAN_FALSE;

    for (size_t i = 0; i < compareLength; i++) {
        char character1 = string[i];
        char character2 = compare[i];

        if (caseless) {
            character1 = (char) tolower(character1);
            character2 = (char) tolower(character2);
        }

        if (i < stringLength && character1 == character2)
            continue;

        return BA_BOOLEAN_FALSE;
    }

    return BA_BOOLEAN_TRUE;
}

BA_Boolean BA_String_EndsWith(const char* string, const char* compare, BA_Boolean caseless) {
    if (string == compare)
        return BA_BOOLEAN_TRUE;

    size_t stringLength = strlen(string);
    size_t compareLength = strlen(compare);
    
    if (stringLength < compareLength)
        return BA_BOOLEAN_FALSE;

    while (stringLength != 0 && compareLength != 0) {
        char character1 = string[stringLength];
        char character2 = compare[compareLength];

        if (caseless) {
            character1 = (char) tolower(character1);
            character2 = (char) tolower(character2);
        }

        if (character1 == character2) {
            stringLength--;
            compareLength--;
            continue;
        }

        return BA_BOOLEAN_FALSE;
    }

    char finalCharacter1 = string[stringLength];
    char finalCharacter2 = compare[compareLength];

    if (caseless) {
        finalCharacter1 = (char) tolower(finalCharacter1);
        finalCharacter2 = (char) tolower(finalCharacter2);
    }

    return finalCharacter1 == finalCharacter2;
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
    
    if (stringLength < strlen(compare))
        return BA_BOOLEAN_FALSE;

    BA_Boolean started = BA_BOOLEAN_FALSE;
    size_t compareIndex = 0;

    for (size_t i = 0; i < stringLength; i++) {
        char character1 = string[i];
        char character2 = compare[compareIndex];

        if (character2 == '\0')
            return BA_BOOLEAN_TRUE;

        if (caseless) {
            character1 = (char) tolower(character1);
            character2 = (char) tolower(character2);
        }

        if (character1 == character2) {
            started = BA_BOOLEAN_TRUE;
            compareIndex++;
            continue;
        }

        if (!started)
            continue;

        return BA_BOOLEAN_FALSE;
    }

    return started;
}

BA_Boolean BA_String_ContainsCharacter(const char* string, char compare, BA_Boolean caseless) {
    char temporaryString[2] = {compare, '\0'};

    return BA_String_Contains(string, temporaryString, caseless);
}

BA_Boolean BA_String_Equals(const char* string, const char* compare, BA_Boolean caseless) {
    if (!caseless)
        return strcmp(string, compare) == 0;

    size_t i = 0;
    size_t compareLength = strlen(compare);
    
    for (; i < strlen(string); i++) {
        if (tolower(string[i]) == tolower(compare[i]))
            continue;

        return BA_BOOLEAN_FALSE;
    }

    return i == compareLength;
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

    size_t newSize = vsnprintf(NULL, 0, *target, arguments);
    char* newBuffer = malloc(sizeof(char) * newSize + 1);

    if (newBuffer == NULL) {
        va_end(argumentsCopy);
        return NULL;
    }
    
    vsnprintf(newBuffer, newSize + 1, *target, argumentsCopy);
    va_end(argumentsCopy);

    *target = newBuffer;
    return *target;
}

BA_DynamicArray* BA_String_Split(const char* target, const char* splitBy) {
    BA_DynamicArray* dynamicArray = malloc(sizeof(BA_DynamicArray));

    if (dynamicArray == NULL)
        return NULL;
    
    BA_DynamicArray_Create(dynamicArray, 100);
    
    char* string = malloc(sizeof(char));
    int matchCount = 0;
    size_t splitByLength = strlen(splitBy);
    size_t targetLength = strlen(target);
    int i = 0;
    BA_Boolean characterMatched = BA_BOOLEAN_FALSE;

    if (string == NULL) {
        free(dynamicArray->internalArray);
        free(dynamicArray);
        return NULL;
    }
    
    string[0] = '\0';

    for (; i < targetLength; i++) {
        if (matchCount == splitByLength) {
            BA_DynamicArray_AddElementToLast(dynamicArray, (void*) string);
            
            matchCount = 0;
            string = malloc(sizeof(char));

            if (string == NULL) {
                free(dynamicArray->internalArray);
                free(dynamicArray);
                return NULL;
            }
            
            string[0] = '\0';
        }
        
        if (target[i] != splitBy[matchCount]) {
            for (int j = 0; j < matchCount; j++)
                BA_String_AppendCharacter(&string, splitBy[j]);
            
            matchCount = 0;
            characterMatched = BA_BOOLEAN_FALSE;

            BA_String_AppendCharacter(&string, target[i]);
            continue;
        }

        characterMatched = BA_BOOLEAN_TRUE;
        matchCount++;
    }

    BA_DynamicArray_AddElementToLast(dynamicArray, string);

    if (i == targetLength && characterMatched) {
        string = malloc(sizeof(char));

        if (string == NULL) {
            free(dynamicArray->internalArray);
            free(dynamicArray);
            return NULL;
        }
        
        string[0] = '\0';

        BA_DynamicArray_AddElementToLast(dynamicArray, (void*) string);
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
        buffer = malloc(sizeof(char));
                
        if (buffer == NULL)
            return -2;

        buffer[0] = '\0';
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

    if (buffer[0] != '\0') {
        if (line != NULL)
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
    size_t originalTargetSize = strlen(*target);

    if (originalTargetSize == 0 || amountOfFormatters <= 0)
        return *target;

    char* newBuffer = malloc(sizeof(char));

    if (newBuffer == NULL)
        return NULL;

    newBuffer[0] = '\0';

    BA_Boolean percentageFound = BA_BOOLEAN_FALSE;
    int usedArguments = 0;
    
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
                
                switch (va_arg(arguments, BA_String_SafeFormatTypes)) {
                    case BA_STRING_SAFE_FORMAT_TYPE_STRING:
                        BA_String_Append(&newBuffer, va_arg(arguments, char*));
                        break;

                    case BA_STRING_SAFE_FORMAT_TYPE_INTEGER:
                        BA_STRING_CONVERT_AND_APPEND(int, "%d");
                        break;

                    case BA_STRING_SAFE_FORMAT_TYPE_DOUBLE:
                        BA_STRING_CONVERT_AND_APPEND(double, "%lf");
                        break;

                    case BA_STRING_SAFE_FORMAT_TYPE_CHARACTER:
                        BA_String_AppendCharacter(&newBuffer, (char) va_arg(arguments, int));
                        break;

                    case BA_STRING_SAFE_FORMAT_TYPE_LONG:
                        BA_STRING_CONVERT_AND_APPEND(long, "%li");
                        break;

                    case BA_STRING_SAFE_FORMAT_TYPE_LONG_LONG:
                        BA_STRING_CONVERT_AND_APPEND(long long, "%lli");
                        break;

                    default:
                        // FIXME: Specify which type
                        BA_ASSERT_ALWAYS("Type not supported in this engine version\n");
                }

#undef BA_STRING_CONVERT_AND_APPEND
                
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
BA_CPLUSPLUS_SUPPORT_GUARD_END()
