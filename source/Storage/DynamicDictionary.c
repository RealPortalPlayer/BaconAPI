// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>

#include "BaconAPI/Storage/DynamicDictionary.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static BA_Boolean BA_DynamicDictionary_UpdateFrozenState(BA_DynamicDictionary* dictionary) {
    dictionary->keys.frozen = dictionary->frozen;
    dictionary->values.frozen = dictionary->frozen;
    return dictionary->frozen;
}

BA_Boolean BA_DynamicDictionary_Create(BA_DynamicDictionary* dictionary, size_t size) {
    BA_Boolean returnValue = BA_DynamicArray_Create(&dictionary->keys, size) &&
                             BA_DynamicArray_Create(&dictionary->values, size);

    dictionary->frozen = BA_BOOLEAN_FALSE;
    return returnValue;
}

BA_Boolean BA_DynamicDictionary_AddElementToStart(BA_DynamicDictionary* dictionary, void* key, void* value) {
    return !BA_DynamicDictionary_UpdateFrozenState(dictionary) &&
           BA_DynamicArray_AddElementToStart(&dictionary->keys, key) &&
           BA_DynamicArray_AddElementToStart(&dictionary->values, value);
}

BA_Boolean BA_DynamicDictionary_AddElementToLast(BA_DynamicDictionary* dictionary, void* key, void* value) {
    return !BA_DynamicDictionary_UpdateFrozenState(dictionary) &&
           BA_DynamicArray_AddElementToLast(&dictionary->keys, key) &&
           BA_DynamicArray_AddElementToLast(&dictionary->values, value);
}

BA_Boolean BA_DynamicDictionary_RemoveFirstElement(BA_DynamicDictionary* dictionary) {
    return !BA_DynamicDictionary_UpdateFrozenState(dictionary) &&
           BA_DynamicArray_RemoveFirstElement(&dictionary->keys) &&
           BA_DynamicArray_RemoveFirstElement(&dictionary->values);
}

BA_Boolean BA_DynamicDictionary_RemoveLastElement(BA_DynamicDictionary* dictionary) {
    return !BA_DynamicDictionary_UpdateFrozenState(dictionary) &&
           BA_DynamicArray_RemoveLastElement(&dictionary->keys) &&
           BA_DynamicArray_RemoveLastElement(&dictionary->values);
}

BA_Boolean BA_DynamicDictionary_RemoveElementAt(BA_DynamicDictionary* dictionary, unsigned index) {
    return !BA_DynamicDictionary_UpdateFrozenState(dictionary) &&
           BA_DynamicArray_RemoveElementAt(&dictionary->keys, index) &&
           BA_DynamicArray_RemoveElementAt(&dictionary->values, index);
}

BA_Boolean BA_DynamicDictionary_RemoveElementViaKey(BA_DynamicDictionary* dictionary, const void* key, size_t elementSize, BA_Boolean repeat) {
    if (BA_DynamicDictionary_UpdateFrozenState(dictionary))
        return BA_BOOLEAN_FALSE;

    BA_Boolean removedOne = BA_BOOLEAN_FALSE;

    for (int i = 0; i < dictionary->keys.used; i++) {
        int elementIndex = BA_DynamicDictionary_GetElementIndexFromKey(dictionary, key, elementSize);

        if (elementIndex == -1) {
            if (!repeat)
                return BA_BOOLEAN_FALSE;

            continue;
        }

        BA_DynamicDictionary_RemoveElementAt(dictionary, elementIndex);

        if (!repeat)
            return BA_BOOLEAN_TRUE;
        
        i--;
        removedOne = BA_BOOLEAN_TRUE;
    }
    
    return removedOne;
}

BA_Boolean BA_DynamicDictionary_RemoveElementViaValue(BA_DynamicDictionary* dictionary, const void* value, size_t elementSize, BA_Boolean repeat) {
    if (BA_DynamicDictionary_UpdateFrozenState(dictionary))
        return BA_BOOLEAN_FALSE;

    BA_Boolean removedOne = BA_BOOLEAN_FALSE;

    for (int i = 0; i < dictionary->keys.used; i++) {
        int elementIndex = BA_DynamicDictionary_GetElementIndexFromValue(dictionary, value, elementSize);

        if (elementIndex == -1) {
            if (!repeat)
                return BA_BOOLEAN_FALSE;

            continue;
        }

        BA_DynamicDictionary_RemoveElementAt(dictionary, elementIndex);

        if (!repeat)
            return BA_BOOLEAN_TRUE;

        i--;
        removedOne = BA_BOOLEAN_TRUE;
    }

    return removedOne;
}

int BA_DynamicDictionary_GetElementIndexFromKey(const BA_DynamicDictionary* dictionary, const void* key, size_t elementSize) {
    for (int index = 0; index < dictionary->keys.used; index++) {
        if (dictionary->keys.internalArray[index] == NULL || memcmp(dictionary->keys.internalArray[index], key, elementSize) != 0)
            continue;

        return index;
    }

    return -1;
}

int BA_DynamicDictionary_GetElementIndexFromValue(const BA_DynamicDictionary* dictionary, const void* value, size_t elementSize) {
    for (int index = 0; index < dictionary->keys.used; index++) {
        if (dictionary->values.internalArray[index] == NULL || memcmp(dictionary->values.internalArray[index], value, elementSize) != 0)
            continue;

        return index;
    }

    return -1;
}

void* BA_DynamicDictionary_GetElementKeyViaValue(const BA_DynamicDictionary* dictionary, const void* value, size_t elementSize) {
    int index = BA_DynamicDictionary_GetElementIndexFromValue(dictionary, value, elementSize);

    return index != -1 ? dictionary->keys.internalArray[index] : NULL;
}

void* BA_DynamicDictionary_GetElementValueViaKey(const BA_DynamicDictionary* dictionary, const void* key, size_t elementSize) {
    int index = BA_DynamicDictionary_GetElementIndexFromKey(dictionary, key, elementSize);

    return index != -1 ? dictionary->values.internalArray[index] : NULL;
}

void BA_DynamicDictionary_GetElementsValueViaKey(const BA_DynamicDictionary* dictionary, BA_DynamicDictionary* results, const void* key, size_t elementSize) {
    for (int index = 0; index < dictionary->keys.used; index++) {
        if (dictionary->keys.internalArray[index] == NULL || memcmp(dictionary->keys.internalArray[index], key, elementSize) != 0)
            continue;

        BA_DynamicDictionary_AddElementToLast(results, key, dictionary->values.internalArray[index]);
    }
}

void BA_DynamicDictionary_GetElementsKeyViaValue(const BA_DynamicDictionary* dictionary, BA_DynamicDictionary* results, const void* value, size_t elementSize) {
    for (int index = 0; index < dictionary->keys.used; index++) {
        if (dictionary->values.internalArray[index] == NULL || memcmp(dictionary->values.internalArray[index], value, elementSize) != 0)
            continue;

        BA_DynamicDictionary_AddElementToLast(results, dictionary->keys.internalArray[index], value);
    }
}

BA_Boolean BA_DynamicDictionary_Shrink(BA_DynamicDictionary* dictionary) {
    return dictionary->keys.size != dictionary->keys.used &&
           !BA_DynamicDictionary_UpdateFrozenState(dictionary) &&       
           BA_DynamicArray_Shrink(&dictionary->keys) &&
           BA_DynamicArray_Shrink(&dictionary->values);
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
