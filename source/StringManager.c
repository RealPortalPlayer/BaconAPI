// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <wchar.h>
#include <stddef.h>
#include <stdlib.h>

#include "BaconAPI/StringManager.h"
#include "BaconAPI/Storage/DynamicArray.h"
#include "BaconAPI/WideString.h"
#include "BaconAPI/String.h"

#define BA_STRINGMANAGER_CHECK_INITIALIZED() \
do {                                         \
    if (!baStringManagerInitialized) {       \
        baStringManagerInitialized = BA_BOOLEAN_TRUE; \
        BA_DynamicArray_Create(&baStringManagerArray, 100); \
    }                                        \
} while (BA_BOOLEAN_FALSE)

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    union {
        char* string;
        wchar_t* wideString;
    };
    BA_Boolean isWideString;
} BA_StringManager_Implementation;

static BA_DynamicArray baStringManagerArray;
static BA_Boolean baStringManagerInitialized = BA_BOOLEAN_FALSE;

void* BA_StringManager_Get(int index) {
    BA_STRINGMANAGER_CHECK_INITIALIZED();

    if (baStringManagerArray.used <= index)
        return NULL;

    BA_StringManager_Implementation* implementation = BA_DYNAMICARRAY_GET_ELEMENT(BA_StringManager_Implementation, baStringManagerArray, index);

    if (implementation->isWideString)
        return implementation->wideString;

    return implementation->string;
}

BA_Boolean BA_StringManager_IsWideString(int index) {
    BA_STRINGMANAGER_CHECK_INITIALIZED();
    return baStringManagerArray.used > index ? BA_DYNAMICARRAY_GET_ELEMENT(BA_StringManager_Implementation, baStringManagerArray, index)->isWideString : BA_BOOLEAN_FALSE;
}

int BA_StringManager_Allocate(const void* originalString, BA_Boolean isWideString) {
    BA_STRINGMANAGER_CHECK_INITIALIZED();

    BA_StringManager_Implementation* implementation = malloc(sizeof(BA_StringManager_Implementation));

    if (implementation == NULL)
        return -1;

    if (isWideString)
        implementation->wideString = BA_WideString_Copy(originalString);
    else
        implementation->string = BA_String_Copy(originalString);

    implementation->isWideString = isWideString;

    BA_DynamicArray_AddElementToLast(&baStringManagerArray, implementation);
    return baStringManagerArray.used - 1;
}
void BA_StringManager_Deallocate(int index) {
    BA_STRINGMANAGER_CHECK_INITIALIZED();

    if (baStringManagerArray.used <= index)
        return;

    if (index < 0) {
        for (int i = 0; i < baStringManagerArray.used; i++) {
            BA_StringManager_Implementation* implementation = BA_DYNAMICARRAY_GET_ELEMENT(BA_StringManager_Implementation, baStringManagerArray, i);

            if (implementation->isWideString)
                free(implementation->wideString);
            else
                free(implementation->string);

            free(implementation);
        }

        free(baStringManagerArray.internalArray);

        baStringManagerInitialized = BA_BOOLEAN_FALSE;
        return;
    }

    BA_StringManager_Implementation* element = BA_DYNAMICARRAY_GET_ELEMENT(BA_StringManager_Implementation, baStringManagerArray, index);

    BA_DynamicArray_RemoveElementAt(&baStringManagerArray, index);

    if (element->isWideString)
        free(element->wideString);
    else
        free(element->string);

    free(element);
}

void BA_StringManager_Replace(int index, const void* newString, BA_Boolean isWideString) {
    BA_STRINGMANAGER_CHECK_INITIALIZED();

    if (baStringManagerArray.used <= index)
        return;

    BA_StringManager_Implementation* implementation = BA_DYNAMICARRAY_GET_ELEMENT(BA_StringManager_Implementation, baStringManagerArray, index);

    if (implementation->isWideString)
        free(implementation->wideString);
    else
        free(implementation->string);

    implementation->isWideString = isWideString;

    if (implementation->isWideString)
        implementation->wideString = BA_WideString_Copy(newString);
    else
        implementation->string = BA_String_Copy(newString);
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
