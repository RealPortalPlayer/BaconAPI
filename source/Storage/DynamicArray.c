// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdlib.h>
#include <string.h>

#include "BaconAPI/Storage/DynamicArray.h"
#include "BaconAPI/Logger.h"
#include "BaconAPI/Debugging/Assert.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static BA_Boolean BA_DynamicArray_ReallocateArray(BA_DynamicArray* array) {
    if (array->size != (size_t) array->used)
        return BA_BOOLEAN_TRUE;

    BA_LOGGER_TRACE("Ran out of free space, expanding array\nThis is expensive, so you should try avoiding it\n");
    BA_ASSERT((int) array->size >= array->used, "Invalid array state\n");

    array->size *= 2;
    array->calledReallocate++;
    
    void** newArray = (void**) realloc(array->internalArray, sizeof(void*) * array->size);

    if (newArray == NULL)
        return BA_BOOLEAN_FALSE;
    
    array->internalArray = newArray;
    return BA_BOOLEAN_TRUE;
}

int BA_DynamicArray_GetIndexForElement(BA_DynamicArray* array, void* element, size_t elementSize) {
    for (int i = 0; i < array->used; i++) {
        if (memcmp(array->internalArray[i], element, elementSize) != 0)
            continue;
        
        return i;
    }

    return -1;
}

BA_Boolean BA_DynamicArray_Create(BA_DynamicArray* array, size_t size) {
    if (size <= 0)
        return BA_BOOLEAN_FALSE;
    
    array->internalArray = malloc(sizeof(void*) * size);

    if (array->internalArray == NULL)
        return BA_BOOLEAN_FALSE;

    array->used = 0;
    array->size = size;
    array->frozen = BA_BOOLEAN_FALSE;
    array->calledReallocate = 0;
    return BA_BOOLEAN_TRUE;
}

BA_Boolean BA_DynamicArray_AddElementToStart(BA_DynamicArray* array, void* element) {
    if (array->frozen)
        return BA_BOOLEAN_FALSE;

    BA_DynamicArray_ReallocateArray(array);

    for (int id = array->used; id >= 0; id--)
        array->internalArray[id + 1] = array->internalArray[id];

    array->internalArray[0] = element;
    array->used++;
    return BA_BOOLEAN_TRUE;
}

BA_Boolean BA_DynamicArray_AddElementToLast(BA_DynamicArray* array, void* element) {
    if (array->frozen)
        return BA_BOOLEAN_FALSE;

    BA_DynamicArray_ReallocateArray(array);

    array->internalArray[array->used++] = element;
    return BA_BOOLEAN_TRUE;
}

BA_Boolean BA_DynamicArray_RemoveFirstElement(BA_DynamicArray* array) {
    return array->used != 0 && !array->frozen && BA_DynamicArray_RemoveElementAt(array, 0);
}

BA_Boolean BA_DynamicArray_RemoveLastElement(BA_DynamicArray* array) {
    if (array->used == 0 || array->frozen)
        return BA_BOOLEAN_FALSE;

    array->internalArray[--array->used] = NULL;
    return BA_BOOLEAN_TRUE;
}

BA_Boolean BA_DynamicArray_RemoveElementAt(BA_DynamicArray* array, unsigned index) {
    if ((int) index >= array->used || array->frozen)
        return BA_BOOLEAN_FALSE;

    BA_DynamicArray_ReallocateArray(array);

    for (unsigned int id = index; (int) id < array->used; id++)
        array->internalArray[id] = array->internalArray[id + 1];

    array->used--;
    return BA_BOOLEAN_TRUE;
}

BA_Boolean BA_DynamicArray_RemoveMatchedElement(BA_DynamicArray* array, void* element, size_t elementSize, BA_Boolean repeat) {
    if (array->frozen)
        return BA_BOOLEAN_FALSE;
    
    BA_Boolean removedOne = BA_BOOLEAN_FALSE;

    for (int i = 0; i < array->used; i++) {
        int elementIndex = BA_DynamicArray_GetIndexForElement(array, element, elementSize);

        if (elementIndex == -1) {
            if (!repeat)
                return BA_BOOLEAN_FALSE;

            continue;
        }

        BA_DynamicArray_RemoveElementAt(array, elementIndex);

        if (!repeat)
            return BA_BOOLEAN_TRUE;

        i--;
        removedOne = BA_BOOLEAN_TRUE;
    }

    return removedOne;
}

BA_Boolean BA_DynamicArray_Shrink(BA_DynamicArray* array) {
    if (array->size == array->used || array->frozen)
        return BA_BOOLEAN_FALSE;
    
    array->size = array->used;
    
    void** newArray = realloc(array->internalArray, array->size);

    if (newArray == NULL)
        return BA_BOOLEAN_FALSE;
    
    array->internalArray = newArray;
    return BA_BOOLEAN_TRUE;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
