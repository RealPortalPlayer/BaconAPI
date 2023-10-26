// Purpose: Stores a variable amount of elements.
// Created on: 8/8/22 @ 3:50 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <stddef.h>

#include "BaconAPI/Internal/CPlusPlusSupport.h"
#include "BaconAPI/Internal/Boolean.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    void** internalArray;
    int used;
    size_t size;
    int calledReallocate;
    BA_Boolean frozen;
} BA_DynamicArray;

/**
 * @return The index if the element was found, -1 if not
 */
int BA_DynamicArray_GetIndexForElement(BA_DynamicArray* array, void* element, size_t elementSize);

BA_Boolean BA_DynamicArray_Create(BA_DynamicArray* array, size_t size);
BA_Boolean BA_DynamicArray_AddElementToStart(BA_DynamicArray* array, void* element);
BA_Boolean BA_DynamicArray_AddElementToLast(BA_DynamicArray* array, void* element);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BA_Boolean BA_DynamicArray_RemoveFirstElement(BA_DynamicArray* array);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BA_Boolean BA_DynamicArray_RemoveLastElement(BA_DynamicArray* array);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BA_Boolean BA_DynamicArray_RemoveElementAt(BA_DynamicArray* array, unsigned int index);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BA_Boolean BA_DynamicArray_RemoveMatchedElement(BA_DynamicArray* array, void* element, size_t elementSize, BA_Boolean repeat);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BA_DYNAMICARRAY_GET_ELEMENT_POINTER(type, array, index) ((type*) (array)->internalArray[(index)])
#define BA_DYNAMICARRAY_GET_ELEMENT(type, array, index) BA_DYNAMICARRAY_GET_ELEMENT_POINTER(type, &array, index)
#define BA_DYNAMICARRAY_GET_LAST_ELEMENT_POINTER(type, array) BA_DYNAMICARRAY_GET_ELEMENT_POINTER(type, array, array->used - 1)
#define BA_DYNAMICARRAY_GET_LAST_ELEMENT(type, array) BA_DYNAMICARRAY_GET_ELEMENT(type, array, array.used - 1)
