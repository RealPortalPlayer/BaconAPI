// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <wctype.h>
#include <ctype.h>
#include <stdio.h>

#include "StringImplementation.h"
#include "BaconAPI/WideString.h"
#include "BaconAPI/String.h"
#include "BaconAPI/Math/Bitwise.h"
#include "BaconAPI/StringSafeFormat.h"
#include "BaconAPI/Debugging/Assert.h"
#include "BaconAPI/OperatingSystem.h"

#if BA_OPERATINGSYSTEM_WINDOWS
#   define strtok_r strtok_s // I hate Microshit so much. Stop trying to be unique and quirky
#endif

// FIXME: Some of these functions return NULL on allocation failure. Some people don't want to crash their program on
//        OOM. This would inconvenience them. No easy way to fix without masking memory issues. Such is life

// HACK: CLion hates this file
// ReSharper disable CppDFAUnreachableCode
// ReSharper disable CppDFAUnusedValue
// ReSharper disable CppDFAConstantFunctionResult
// ReSharper disable CppDFANullDereference

// NOTE: CLion sucks at detecting memory leaks in this file. Use ASAN instead
// ReSharper disable CppDFAMemoryLeak

#undef BA_STRINGIMPLEMENTATION_CREATE
#define BA_STRINGIMPLEMENTATION_CREATE(variable, size, makeItWideString) \
do {                                                                     \
    variable = malloc(sizeof(BA_StringImplementation));                  \
    if (variable != NULL) {                                              \
        variable->isWideString = makeItWideString;                       \
        if (makeItWideString) {                                          \
            variable->wideString = calloc(1, sizeof(wchar_t) * (size + 1)); \
            if (variable->wideString == NULL) {                          \
                free(variable);                                          \
                variable = NULL;                                         \
            }                                                            \
        } else {                                                         \
            variable->string = calloc(1, sizeof(char) * (size + 1));     \
            if (variable->string == NULL) {                              \
                free(variable);                                          \
                variable = NULL;                                         \
            }                                                            \
        }                                                                \
    }                                                                    \
} while (BA_BOOLEAN_FALSE)

#define BA_STRINGIMPLEMENTATION_GET_LENGTH(specialString) \
if (specialString->isWideString)                          \
    specialString ## Length = wcslen(specialString->wideString); \
else                                                      \
    specialString ## Length = strlen(specialString->string)

#define BA_STRINGIMPLEMENTATION_GET_STRING_COMPARE(stringLength, compareLength) \
heapString = NULL;                                                              \
heapCompare = NULL;                                                             \
BA_STRINGIMPLEMENTATION_CREATE(heapString, stringLength, string->isWideString); \
if (heapString != NULL) {                                                       \
    BA_STRINGIMPLEMENTATION_CREATE(heapCompare, stringLength, string->isWideString); \
    if (heapCompare != NULL) {                                                  \
        allocated = BA_BOOLEAN_TRUE;                                            \
        if (string->isWideString) {                                             \
            wcsncpy(heapString->wideString, string->wideString, stringLength);  \
            wcsncpy(heapCompare->wideString, compare->wideString, compareLength); \
        } else {                                                                \
            strncpy(heapString->string, string->string, stringLength);          \
            strncpy(heapCompare->string, compare->string, stringLength);        \
        }                                                                       \
    }                                                                           \
} (void) NULL

#define BA_STRINGIMPLEMENTATION_FREE(specifiedString) \
if (specifiedString->isWideString)                    \
    free(specifiedString->wideString);                \
else                                                  \
    free(specifiedString->string);                    \
free(specifiedString)

#define BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(specifiedString, returnValue) \
if (specifiedString == NULL)                                                \
    return returnValue;                                                     \
if (specifiedString->isWideString) {                                        \
    if (specifiedString->wideString == NULL)                                \
        return returnValue;                                                 \
} else {                                                                    \
    if (specifiedString->string == NULL)                                    \
        return returnValue;                                                 \
} (void) NULL

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static BA_DynamicDictionary baStringImplementationDynamicDictionary;
static BA_Boolean baStringImplementationInitialized = BA_BOOLEAN_FALSE;

static void BA_StringImplementation_Initialize(void) {
    if (baStringImplementationInitialized)
        return;
    
    baStringImplementationInitialized = BA_DynamicDictionary_Create(&baStringImplementationDynamicDictionary, 10);
}


BA_Boolean BA_StringImplementation_Contains(const BA_StringImplementation* string, const BA_StringImplementation* compare, BA_Boolean caseless) {
    if (string == compare)
        return BA_BOOLEAN_TRUE;

    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(string, BA_BOOLEAN_FALSE);
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(compare, BA_BOOLEAN_FALSE);

    size_t stringLength;
    size_t compareLength;
    BA_Boolean allocated = BA_BOOLEAN_FALSE;
    
    BA_STRINGIMPLEMENTATION_GET_LENGTH(string);
    BA_STRINGIMPLEMENTATION_GET_LENGTH(compare);

    if (stringLength < compareLength)
        return BA_BOOLEAN_FALSE;

    BA_StringImplementation* heapString = (BA_StringImplementation*) string;
    BA_StringImplementation* heapCompare = (BA_StringImplementation*) compare;

    if (!caseless)
        goto check;
    
    heapString = NULL;
    heapCompare = NULL;

    BA_STRINGIMPLEMENTATION_GET_STRING_COMPARE(stringLength, compareLength);

    if (heapCompare == NULL)
        return BA_BOOLEAN_FALSE;

    heapString = BA_StringImplementation_ToLower(heapString);
    heapCompare = BA_StringImplementation_ToLower(heapCompare);

    check:
    for (int i = 0; i < stringLength; i++) {
        if (string->isWideString) {
            if (wcsncmp(heapString->wideString + i, heapCompare->wideString, compareLength) != 0)
                continue;
        } else {
            if (strncmp(heapString->string + i, heapCompare->string, compareLength) != 0)
                continue;
        }

        if (allocated) {
            BA_STRINGIMPLEMENTATION_FREE(heapString);
            BA_STRINGIMPLEMENTATION_FREE(heapCompare);
        }
        
        return BA_BOOLEAN_TRUE;
    }

    if (allocated) {
        BA_STRINGIMPLEMENTATION_FREE(heapString);
        BA_STRINGIMPLEMENTATION_FREE(heapCompare);
    }
    
    return BA_BOOLEAN_FALSE;
}

BA_Boolean BA_StringImplementation_Equals(const BA_StringImplementation* string, const BA_StringImplementation* compare, BA_Boolean caseless) {
    BA_Boolean result;

    if (string == compare)
        return BA_BOOLEAN_TRUE;
    
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(string, BA_BOOLEAN_FALSE);
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(compare, BA_BOOLEAN_FALSE);

    BA_StringImplementation* heapString = (BA_StringImplementation*) string;
    BA_StringImplementation* heapCompare = (BA_StringImplementation*) compare;
    BA_Boolean allocated = BA_BOOLEAN_FALSE;
    size_t stringLength;

    BA_STRINGIMPLEMENTATION_GET_LENGTH(string);

    if (!caseless)
        goto check;

    {
        size_t compareLength;

        BA_STRINGIMPLEMENTATION_GET_LENGTH(compare);

        if (stringLength != compareLength)
            return BA_BOOLEAN_FALSE;
    }

    BA_STRINGIMPLEMENTATION_GET_STRING_COMPARE(stringLength, stringLength);

    if (heapCompare == NULL)
        return BA_BOOLEAN_FALSE;
    
    heapString = BA_StringImplementation_ToLower(heapString);
    heapCompare = BA_StringImplementation_ToLower(heapCompare);

    check:
    result = (string->isWideString ? wcsncmp(heapString->wideString, heapCompare->wideString, stringLength) : strncmp(heapString->string, heapCompare->string, stringLength)) == 0;

    if (allocated) {
        BA_STRINGIMPLEMENTATION_FREE(heapString);
        BA_STRINGIMPLEMENTATION_FREE(heapCompare);
    }
    
    return result;
}

BA_Boolean BA_StringImplementation_StartsWith(const BA_StringImplementation* string, const BA_StringImplementation* compare, BA_Boolean caseless) {
    BA_Boolean result;

    if (string == compare)
        return BA_BOOLEAN_TRUE;

    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(string, BA_BOOLEAN_FALSE);
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(compare, BA_BOOLEAN_FALSE);

    size_t compareLength;
    BA_StringImplementation* heapString = (BA_StringImplementation*) string;
    BA_StringImplementation* heapCompare = (BA_StringImplementation*) compare;
    BA_Boolean allocated = BA_BOOLEAN_FALSE;
    
    BA_STRINGIMPLEMENTATION_GET_LENGTH(compare);

    {
        size_t stringLength;

        BA_STRINGIMPLEMENTATION_GET_LENGTH(string);

        if (stringLength < compareLength)
            return BA_BOOLEAN_FALSE;

        if (!caseless)
            goto check;
    }

    BA_STRINGIMPLEMENTATION_GET_STRING_COMPARE(compareLength, compareLength);

    if (heapCompare == NULL)
        return BA_BOOLEAN_FALSE;
    
    heapString = BA_StringImplementation_ToLower(heapString);
    heapCompare = BA_StringImplementation_ToLower(heapCompare);

    check:
    result = (string->isWideString ? wcsncmp(heapString->wideString, heapCompare->wideString, compareLength) : strncmp(heapString->string, heapCompare->string, compareLength)) == 0;

    if (allocated) {
        BA_STRINGIMPLEMENTATION_FREE(heapString);
        BA_STRINGIMPLEMENTATION_FREE(heapCompare);
    }
    
    return result;
}

BA_Boolean BA_StringImplementation_EndsWith(const BA_StringImplementation* string, const BA_StringImplementation* compare, BA_Boolean caseless) {
    BA_Boolean result;

    if (string == compare)
        return BA_BOOLEAN_TRUE;

    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(string, BA_BOOLEAN_FALSE);
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(compare, BA_BOOLEAN_FALSE);

    size_t stringLength;
    size_t compareLength;
    BA_StringImplementation* heapString = (BA_StringImplementation*) string;
    BA_StringImplementation* heapCompare = (BA_StringImplementation*) compare;
    BA_Boolean allocated = BA_BOOLEAN_FALSE;
    
    BA_STRINGIMPLEMENTATION_GET_LENGTH(string);
    BA_STRINGIMPLEMENTATION_GET_LENGTH(compare);

    if (stringLength < compareLength)
        return BA_BOOLEAN_FALSE;

    if (!caseless)
        goto check;
        
    BA_STRINGIMPLEMENTATION_GET_STRING_COMPARE(stringLength, compareLength);

    if (heapCompare == NULL)
        return BA_BOOLEAN_FALSE;

    heapString = BA_StringImplementation_ToLower(heapString);
    heapCompare = BA_StringImplementation_ToLower(heapCompare);

    check:
    result = (string->isWideString ? wcsncmp(heapString->wideString + (stringLength - compareLength), heapCompare->wideString, compareLength) : strncmp(heapString->string + (stringLength - compareLength), heapCompare->string, compareLength)) == 0;

    if (allocated) {
        BA_STRINGIMPLEMENTATION_FREE(heapString);
        BA_STRINGIMPLEMENTATION_FREE(heapCompare);
    }

    return result;
}

#define BA_STRINGIMPLEMENTATION_GET_TEMPORARY_STRING(willBeWideString, stringName, wideVariableName, stringVariableName) \
BA_StringImplementation stringName;                                                                                      \
wchar_t temporary ## wideVariableName[2];                                                                                \
char temporary ## stringVariableName[2];                                                                                 \
if (willBeWideString) {                                                                                                  \
    temporary ## wideVariableName[0] = wideVariableName;                                                                 \
    temporary ## wideVariableName[1] = L'\0';                                                                            \
    stringName.wideString = temporary ## wideVariableName;                                                               \
} else {                                                                                                                 \
    temporary ## stringVariableName[0] = stringVariableName;                                                             \
    temporary ## stringVariableName[1] = '\0';                                                                           \
    stringName.string = temporary ## stringVariableName;                                                                 \
}                                                                                                                        \
stringName.isWideString = willBeWideString

BA_Boolean BA_StringImplementation_ContainsCharacter(const BA_StringImplementation* string, char compare, wchar_t wideCompare, BA_Boolean caseless) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(string, BA_BOOLEAN_FALSE);
    BA_STRINGIMPLEMENTATION_GET_TEMPORARY_STRING(string->isWideString, temporaryString, wideCompare, compare);
    return BA_StringImplementation_Contains(string, &temporaryString, caseless);
}

BA_StringImplementation* BA_StringImplementation_Copy(const BA_StringImplementation* duplicateFrom) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(duplicateFrom, NULL);

    BA_StringImplementation* duplicatedString = malloc(sizeof(BA_StringImplementation));
    
    if (duplicatedString == NULL)
        return NULL;

    if (duplicateFrom->isWideString)
        duplicatedString->wideString = wcsdup(duplicateFrom->wideString);
    else
        duplicatedString->string = strdup(duplicateFrom->string);

    duplicatedString->isWideString = duplicateFrom->isWideString;
    return duplicatedString;
}

BA_StringImplementation* BA_StringImplementation_Append(BA_StringImplementation* target, const BA_StringImplementation* stringToAppend) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(target, NULL);
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(stringToAppend, NULL);
    
    size_t targetLength;
    size_t stringToAppendLength;

    BA_STRINGIMPLEMENTATION_GET_LENGTH(target);
    BA_STRINGIMPLEMENTATION_GET_LENGTH(stringToAppend);
    
    BA_StringImplementation* clonedTarget;

    BA_STRINGIMPLEMENTATION_CREATE(clonedTarget, targetLength + stringToAppendLength, target->isWideString);

    if (clonedTarget == NULL)
        return NULL;
    
    if (target->isWideString) {
        wcscpy(clonedTarget->wideString, target->wideString);
        wcscat(clonedTarget->wideString, stringToAppend->wideString);
    } else {
        strcpy(clonedTarget->string, target->string);
        strcat(clonedTarget->string, stringToAppend->string);
    }

    BA_STRINGIMPLEMENTATION_FREE(target);
    return clonedTarget;
}

BA_StringImplementation* BA_StringImplementation_Prepend(BA_StringImplementation* target, const BA_StringImplementation* stringToPrepend) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(target, NULL);
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(stringToPrepend, NULL);
    
    BA_StringImplementation* copiedStringToPrepend = BA_StringImplementation_Copy(stringToPrepend);

    if (copiedStringToPrepend == NULL)
        return NULL;

    copiedStringToPrepend = BA_StringImplementation_Append(copiedStringToPrepend, target);

    BA_STRINGIMPLEMENTATION_FREE(target);
    // ReSharper disable once CppDFAMemoryLeak
    return copiedStringToPrepend;
}

BA_StringImplementation* BA_StringImplementation_ToLower(BA_StringImplementation* string) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(string, NULL);
    
    size_t stringLength;

    BA_STRINGIMPLEMENTATION_GET_LENGTH(string);

    for (size_t i = 0; i < stringLength; i++) {
        if (string->isWideString)
            string->wideString[i] = (wchar_t) towlower(string->wideString[i]);
        else
            string->string[i] = (char) tolower(string->string[i]);
    }

    return string;
}

BA_StringImplementation* BA_StringImplementation_ToUpper(BA_StringImplementation* string) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(string, NULL);

    size_t stringLength;

    BA_STRINGIMPLEMENTATION_GET_LENGTH(string);

    for (size_t i = 0; i < stringLength; i++) {
        if (string->isWideString)
            string->wideString[i] = (wchar_t) towupper(string->wideString[i]);
        else
            string->string[i] = (char) toupper(string->string[i]);
    }

    return string;
}

BA_StringImplementation* BA_StringImplementation_AppendCharacter(BA_StringImplementation* target, char character, wchar_t wideCharacter) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(target, NULL);
    BA_STRINGIMPLEMENTATION_GET_TEMPORARY_STRING(target->isWideString, temporaryString, wideCharacter, character);
    return BA_StringImplementation_Append(target, &temporaryString);
}

BA_StringImplementation* BA_StringImplementation_PrependCharacter(BA_StringImplementation* target, char character, wchar_t wideCharacter) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(target, NULL);
    BA_STRINGIMPLEMENTATION_GET_TEMPORARY_STRING(target->isWideString, temporaryString, wideCharacter, character);
    return BA_StringImplementation_Prepend(target, &temporaryString);
}

BA_StringImplementation* BA_StringImplementation_FormatPremadeList(BA_StringImplementation* target, va_list arguments) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(target, NULL);

    va_list argumentsCopy;

    va_copy(argumentsCopy, arguments);

    size_t newLength = 1;
    BA_StringImplementation* newBuffer = malloc(sizeof(BA_StringImplementation));

    if (target->isWideString) {
        static FILE* dummyFile = NULL;

        // This is disgusting
        if (dummyFile == NULL) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
            dummyFile = fopen("/dev/null", "wb");
#elif BA_OPERATINGSYSTEM_WINDOWS
            dummyFile = fopen("NUL", "wb");
#endif
        }

        newLength += vfwprintf(dummyFile, target->wideString, arguments);
    } else
        newLength += vsnprintf(NULL, 0, target->string, arguments);
    
    if (newBuffer == NULL) {
        va_end(argumentsCopy);
        return NULL;
    }
    
    newBuffer->isWideString = target->isWideString;
    
    if (target->isWideString) {
        size_t length = wcslen(target->wideString);
        newBuffer->wideString = calloc(1, sizeof(wchar_t) * (newLength + length));
        
        if (newBuffer->wideString == NULL) {
            free(newBuffer);
            return NULL;
        }

        wcsncpy(newBuffer->wideString, target->wideString, length);
    } else {
        size_t length = strlen(target->string);
        
        newBuffer->string = calloc(1, sizeof(char) * (newLength + length));

        if (newBuffer->string == NULL) {
            free(newBuffer);
            return NULL;
        }
        
        strncpy(newBuffer->string, target->string, length);
    }

    target->isWideString ? vswprintf(newBuffer->wideString, newLength, target->wideString, argumentsCopy) : vsnprintf(newBuffer->string, newLength, target->string, argumentsCopy);
    va_end(argumentsCopy);
    BA_STRINGIMPLEMENTATION_FREE(target);
    return newBuffer;
}

BA_DynamicArray* BA_StringImplementation_Split(const BA_StringImplementation* target, const BA_StringImplementation* splitBy) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(target, NULL);
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(splitBy, NULL);
    
    BA_DynamicArray* dynamicArray = malloc(sizeof(BA_DynamicArray));

    if (dynamicArray == NULL)
        return NULL;
    
    BA_StringImplementation* heapTarget = BA_StringImplementation_Copy(target);
    void* originalStringPointer = heapTarget->string; // NOTE: string, and wideString occupy the same space, so this wouldn't matter
    
    BA_DynamicArray_Create(dynamicArray, 100);

    if (dynamicArray == NULL) {
        free(heapTarget);
        return NULL;
    }

    BA_StringImplementation* stackTargetPointer = heapTarget;
    wchar_t* wideToken = NULL;
    char* token = NULL;

    if (target->isWideString)
        wideToken = wcstok(heapTarget->wideString, splitBy->wideString, &stackTargetPointer->wideString);
    else
        token = strtok_r(heapTarget->string, splitBy->string, &stackTargetPointer->string);

    while (target->isWideString ? wideToken != NULL : token != NULL) {
        if (target->isWideString) {
            BA_DynamicArray_AddElementToLast(dynamicArray, BA_WideString_Copy(wideToken));

            wideToken = wcstok(stackTargetPointer->wideString, splitBy->wideString, &stackTargetPointer->wideString);
        } else {
            BA_DynamicArray_AddElementToLast(dynamicArray, BA_String_Copy(token));

            token = strtok_r(stackTargetPointer->string, splitBy->string, &stackTargetPointer->string);
        }
    }

    free(originalStringPointer);
    free(heapTarget);
    return dynamicArray;
}

BA_DynamicArray* BA_StringImplementation_SplitCharacter(const BA_StringImplementation* target, char splitBy, wchar_t wideSplitBy) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(target, NULL);
    BA_STRINGIMPLEMENTATION_GET_TEMPORARY_STRING(target->isWideString, temporaryString, wideSplitBy, splitBy);
    return BA_StringImplementation_Split(target, &temporaryString);
}

BA_StringImplementation* BA_StringImplementation_FormatSafePremadeList(BA_StringImplementation* target, int amountOfFormatters, va_list arguments) {
    BA_StringImplementation_Initialize();

    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(target, NULL);
    
    size_t targetLength;

    BA_STRINGIMPLEMENTATION_GET_LENGTH(target);

    if (targetLength == 0 || amountOfFormatters <= 0)
        return target;

    BA_StringImplementation* newBuffer = BA_StringImplementation_CreateEmpty(target->isWideString);

    if (newBuffer == NULL)
        return NULL;

    BA_Boolean percentageFound = BA_BOOLEAN_FALSE;
    int usedArguments = 0;
    int lastSuccessfulIdentifier = 0;

    for (int i = 0; i < targetLength; i++) {
        if (target->isWideString ? target->wideString[i] == L'%' : target->string[i] == '%') {
            if (!percentageFound) {
                percentageFound = BA_BOOLEAN_TRUE;
                continue;
            }

            if (target->isWideString)
                newBuffer->wideString = BA_WideString_Append(newBuffer->wideString, L"%%");
            else
                newBuffer->string = BA_String_Append(newBuffer->string, "%%");

            percentageFound = BA_BOOLEAN_FALSE;
            continue;
        }

        if (percentageFound) {
            percentageFound = BA_BOOLEAN_FALSE;

            if (target->isWideString ? target->wideString[i] == L's' : target->string[i] == 's') {
                if (usedArguments >= amountOfFormatters) {
                    if (target->isWideString)
                        newBuffer->wideString = BA_WideString_Append(newBuffer->wideString, L"%s");
                    else
                        newBuffer->string = BA_String_Append(newBuffer->string, "%s");

                    continue;
                }

                usedArguments++;

#define BA_STRINGIMPLEMENTATION_CONVERT_AND_APPEND(type, formatSpecifier) \
do {                                                                      \
    type value = va_arg(arguments, type);                                 \
    size_t bufferSize;                                                    \
    if (target->isWideString) {                                           \
        bufferSize = swprintf(NULL, 0, L ## formatSpecifier, value);      \
        wchar_t* buffer = calloc(1, bufferSize);                          \
        swprintf(buffer, bufferSize + 1, L ## formatSpecifier, value);    \
        newBuffer->wideString = BA_WideString_Append(newBuffer->wideString, buffer); \
        free(buffer);                                                     \
    } else {                                                              \
        bufferSize = snprintf(NULL, 0, formatSpecifier, value);           \
        char* buffer = calloc(1, bufferSize);                             \
        snprintf(buffer, bufferSize + 1, formatSpecifier, value);         \
        newBuffer->string = BA_String_Append(newBuffer->string, buffer);  \
        free(buffer);                                                     \
    }                                                                     \
} while (BA_BOOLEAN_FALSE)

                BA_StringSafeFormat_Types identifier = va_arg(arguments, BA_StringSafeFormat_Types);

                switch (identifier) {
                    case BA_STRINGSAFEFORMAT_TYPE_STRING:
                        if (target->isWideString) {
                            wchar_t* converted = BA_WideString_Convert(va_arg(arguments, char*));
                            
                            newBuffer->wideString = BA_WideString_Append(newBuffer->wideString, converted);
                            free(converted);
                        } else
                            newBuffer->string = BA_String_Append(newBuffer->string, va_arg(arguments, char*));

                        break;

                    case BA_STRINGSAFEFORMAT_TYPE_INTEGER: BA_STRINGIMPLEMENTATION_CONVERT_AND_APPEND(int, "%d"); break;
                    case BA_STRINGSAFEFORMAT_TYPE_DOUBLE: BA_STRINGIMPLEMENTATION_CONVERT_AND_APPEND(double, "%lf"); break;
                    case BA_STRINGSAFEFORMAT_TYPE_CHARACTER:
                        if (target->isWideString)
                            newBuffer->wideString = BA_WideString_AppendCharacter(newBuffer->wideString, va_arg(arguments, int));
                        else
                            newBuffer->string = BA_String_AppendCharacter(newBuffer->string, va_arg(arguments, int));

                        break;

                    case BA_STRINGSAFEFORMAT_TYPE_LONG: BA_STRINGIMPLEMENTATION_CONVERT_AND_APPEND(long, "%li"); break;
                    case BA_STRINGSAFEFORMAT_TYPE_LONG_LONG: BA_STRINGIMPLEMENTATION_CONVERT_AND_APPEND(long long, "%lli"); break;
                    case BA_STRINGSAFEFORMAT_TYPE_SHORT: BA_STRINGIMPLEMENTATION_CONVERT_AND_APPEND(int, "%hi"); break;
                    case BA_STRINGSAFEFORMAT_TYPE_UNSIGNED: BA_STRINGIMPLEMENTATION_CONVERT_AND_APPEND(unsigned, "%u"); break;
                    case BA_STRINGSAFEFORMAT_TYPE_LONG_DOUBLE: BA_STRINGIMPLEMENTATION_CONVERT_AND_APPEND(long double, "%Lf"); break;
                    case BA_STRINGSAFEFORMAT_TYPE_UNSIGNED_CHARACTER: BA_STRINGIMPLEMENTATION_CONVERT_AND_APPEND(unsigned int, "%c"); break;
                    case BA_STRINGSAFEFORMAT_TYPE_UNSIGNED_LONG: BA_STRINGIMPLEMENTATION_CONVERT_AND_APPEND(unsigned long, "%lu"); break;
                    case BA_STRINGSAFEFORMAT_TYPE_UNSIGNED_LONG_LONG: BA_STRINGIMPLEMENTATION_CONVERT_AND_APPEND(unsigned long long, "%llu"); break;
                    case BA_STRINGSAFEFORMAT_TYPE_POINTER: BA_STRINGIMPLEMENTATION_CONVERT_AND_APPEND(void*, "%p"); break;
                    default:
                    {
                        if (baStringImplementationInitialized) {
                            BA_StringSafeFormat_CustomSafeFormatAction actionFunction = (BA_StringSafeFormat_CustomSafeFormatAction) BA_DYNAMICDICTIONARY_GET_VALUE(BA_StringSafeFormat_CustomSafeFormatAction, &baStringImplementationDynamicDictionary, &identifier, sizeof(int));

                            if (actionFunction != NULL) {
                                void* argument = va_arg(arguments, void*);

                                if (target->isWideString)
                                    newBuffer->wideString = actionFunction(newBuffer->wideString, BA_BOOLEAN_TRUE, &argument);
                                else
                                    newBuffer->string = actionFunction(newBuffer->string, BA_BOOLEAN_FALSE, &argument);
                                
                                break;
                            }
                        }

                        BA_ASSERT_ALWAYS("Specified type is unrecognized:\nIdentifier: %i\nPrevious identifier: %i\n", identifier, lastSuccessfulIdentifier);
                    }
                }

                lastSuccessfulIdentifier = identifier;
                continue;
            }

            if (target->isWideString)
                newBuffer->wideString = BA_WideString_AppendCharacter(newBuffer->wideString, L'%');
            else
                newBuffer->string = BA_String_AppendCharacter(newBuffer->string, '%');
        }

        if (target->isWideString)
            newBuffer->wideString = BA_WideString_AppendCharacter(newBuffer->wideString, target->wideString[i]);
        else
            newBuffer->string = BA_String_AppendCharacter(newBuffer->string, target->string[i]);
    }

    if (percentageFound) {
        if (target->isWideString)
            newBuffer->wideString = BA_WideString_AppendCharacter(newBuffer->wideString, L'%');
        else
            newBuffer->string = BA_String_AppendCharacter(newBuffer->string, '%');
    }

    BA_STRINGIMPLEMENTATION_FREE(target);
    return newBuffer;
}

BA_StringImplementation* BA_StringImplementation_CreateEmpty(BA_Boolean isWideString) {
    BA_StringImplementation* string;

    BA_STRINGIMPLEMENTATION_CREATE(string, 1, isWideString);
    return string;
}

BA_Boolean BA_StringSafeFormat_AddCustomSafeFormatter(int identifier, BA_StringSafeFormat_CustomSafeFormatAction actionFunction) {
    BA_StringImplementation_Initialize();

    if (!baStringImplementationInitialized)
        return BA_BOOLEAN_FALSE;

    int* identifierPointer = malloc(sizeof(int));

    if (identifierPointer == NULL)
        return BA_BOOLEAN_FALSE;

    *identifierPointer = identifier;
    return BA_DynamicDictionary_AddElementToLast(&baStringImplementationDynamicDictionary, identifierPointer, actionFunction);
}

BA_StringImplementation* BA_StringImplementation_Replace(BA_StringImplementation* target, const BA_StringImplementation* what, const BA_StringImplementation* to) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(target, NULL);
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(what, NULL);
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(to, NULL);

    size_t targetLength;
    size_t whatLength;
    size_t toLength;

    BA_STRINGIMPLEMENTATION_GET_LENGTH(target);
    BA_STRINGIMPLEMENTATION_GET_LENGTH(what);
    BA_STRINGIMPLEMENTATION_GET_LENGTH(to);

    if (targetLength < whatLength)
        return target;

    for (int i = 0; i < targetLength; i++) {
        if (target->isWideString) {
            if (wcsncmp(target->wideString + i, what->wideString, whatLength) != 0)
                continue;
        } else {
            if (strncmp(target->string + i, what->string, whatLength) != 0)
                continue;
        }

        size_t savedStringLength = targetLength - (i + whatLength);
        BA_StringImplementation* temporaryString;

        BA_STRINGIMPLEMENTATION_CREATE(temporaryString, savedStringLength, target->isWideString);

        if (temporaryString == NULL)
            return NULL;
        
        if (target->isWideString)
            wcsncpy(temporaryString->wideString, target->wideString + i + whatLength, savedStringLength);
        else
            strncpy(temporaryString->string, target->string + i + whatLength, savedStringLength);

        if (targetLength != toLength) {
            targetLength += toLength - whatLength;

            if (target->isWideString) {
                target->wideString = realloc(target->wideString, sizeof(wchar_t) * (targetLength + 1));
                target->wideString[targetLength] = L'\0';
            } else {
                target->string = realloc(target->string, sizeof(char) * (targetLength + 1));
                target->string[targetLength] = '\0';
            }
        }

        if (target->isWideString) {
            wcsncpy(target->wideString + i, to->wideString, toLength);
            wcsncpy(target->wideString + i + toLength, temporaryString->wideString, savedStringLength);
        } else {
            strncpy(target->string + i, to->string, toLength);
            strncpy(target->string + i + toLength, temporaryString->string, savedStringLength);
        }

        BA_STRINGIMPLEMENTATION_FREE(temporaryString);
    }

    return target;
}

BA_StringImplementation* BA_StringImplementation_ReplaceCharacter(BA_StringImplementation* target, char what, wchar_t wideWhat, char to, wchar_t wideTo) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(target, NULL);
    BA_STRINGIMPLEMENTATION_GET_TEMPORARY_STRING(target->isWideString, temporaryWhatString, wideWhat, what);
    BA_STRINGIMPLEMENTATION_GET_TEMPORARY_STRING(target->isWideString, temporaryToString, wideTo, to);
    return BA_StringImplementation_Replace(target, &temporaryWhatString, &temporaryToString);
}

BA_StringImplementation* BA_StringImplementation_Join(const BA_DynamicArray* dynamicArray, const BA_StringImplementation* joinString) {
    if (dynamicArray == NULL)
        return NULL;

    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(joinString, NULL);
    
    BA_StringImplementation* finalString = BA_StringImplementation_CreateEmpty(joinString->isWideString);
    
    if (finalString == NULL)
        return NULL;
    
    for (int i = 0; i < dynamicArray->used - 1; i++) {
        if (joinString->isWideString)
            finalString->wideString = BA_WideString_Append(BA_WideString_Append(finalString->wideString, BA_DYNAMICARRAY_GET_ELEMENT_POINTER(wchar_t, dynamicArray, i)), joinString->wideString);
        else
            finalString->string = BA_String_Append(BA_String_Append(finalString->string, BA_DYNAMICARRAY_GET_ELEMENT_POINTER(char, dynamicArray, i)), joinString->string);
    }

    if (joinString->isWideString)
        finalString->wideString = BA_WideString_Append(finalString->wideString, BA_DYNAMICARRAY_GET_LAST_ELEMENT_POINTER(wchar_t, dynamicArray));
    else
        finalString->string = BA_String_Append(finalString->string, BA_DYNAMICARRAY_GET_LAST_ELEMENT_POINTER(char, dynamicArray));

    return finalString;
}

BA_StringImplementation* BA_StringImplementation_JoinCharacter(const BA_DynamicArray* dynamicArray, BA_Boolean isWideString, char joinCharacter, wchar_t wideJoinCharacter) {
    if (dynamicArray == NULL)
        return NULL;

    BA_STRINGIMPLEMENTATION_GET_TEMPORARY_STRING(isWideString, temporaryString, wideJoinCharacter, joinCharacter);
    return BA_StringImplementation_Join(dynamicArray, &temporaryString);
}

BA_StringImplementation* BA_StringImplementation_Convert(const BA_StringImplementation* target) {
    BA_STRINGIMPLEMENTATION_CHECK_IF_NULL(target, NULL);

    BA_StringImplementation* newBuffer;
    size_t size;

    if (target->isWideString)
        size = wcslen(target->wideString);
    else
        size = strlen(target->string);

    BA_STRINGIMPLEMENTATION_CREATE(newBuffer, size, !target->isWideString);
    
    if (newBuffer == NULL)
        return NULL;
    
    {
        size_t returnValue = 0;
    
        if (target->isWideString)
            returnValue = wcstombs(newBuffer->string, target->wideString, size);
        else
            returnValue = mbstowcs(newBuffer->wideString, target->string, size);

        if (returnValue == -1) {
            free(newBuffer);
        }
    }

    return newBuffer;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
