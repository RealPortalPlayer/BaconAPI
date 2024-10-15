// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Storage/DynamicArray.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/Debugging/StaticAssert.h>

#define ASSERT_USED(expected) BA_ASSERT(array.used == expected, "Used desync\n")
#define ASSERT_SIZE(expected) BA_ASSERT(array.size == expected, "Size desync\n")
#define ASSERT_REALLOCATE(expected) BA_ASSERT(array.calledReallocate == expected, "Unexpected reallocate count\n")

#define ASSERT_FROZEN(call) \
BA_ASSERT(!call, "Modified frozen array\n"); \
ASSERT_USED(2);             \
ASSERT_SIZE(11);            \
ASSERT_REALLOCATE(1)

void Test(void) {
    BA_DynamicArray array;
    int number1 = 0;
    int number2 = 1;
    int number3 = 2;
    
    BA_ASSERT(BA_DynamicArray_Create(&array, 10), "Failed to create array\n");
    BA_ASSERT(BA_DynamicArray_GetIndexForElement(&array, &number1, sizeof(int)) == -1, "Found index when item doesn't exist\n");
    BA_ASSERT(BA_DynamicArray_AddElementToStart(&array, &number1), "Failed to add item\n");
    BA_ASSERT(BA_DynamicArray_GetIndexForElement(&array, &number1, sizeof(int)) == 0, "Invalid index\n");
    ASSERT_USED(1);
    BA_ASSERT(BA_DynamicArray_AddElementToLast(&array, &number2), "Failed to add item\n");
    BA_ASSERT(array.internalArray[0] == &number1, "First item is not correct\n");
    BA_ASSERT(BA_DYNAMICARRAY_GET_LAST_ELEMENT(int, array) == &number2, "Last item is not correct\n");
    ASSERT_USED(2);
    BA_ASSERT(BA_DynamicArray_RemoveFirstElement(&array), "Failed to remove first item\n");
    BA_ASSERT(array.internalArray[0] == &number2, "Last item did not shift over\n");
    ASSERT_USED(1);
    BA_DynamicArray_AddElementToStart(&array, &number1);
    BA_ASSERT(BA_DynamicArray_RemoveLastElement(&array), "Failed to remove the last item\n");
    BA_ASSERT(array.internalArray[0] == &number1, "First item got destroyed\n");
    ASSERT_USED(1);
    BA_DynamicArray_AddElementToLast(&array, &number2);
    BA_ASSERT(BA_DynamicArray_RemoveElementAt(&array, 1), "Failed to remove element at index 1\n");
    BA_ASSERT(array.internalArray[0] == &number1, "First item got destroyed\n");
    ASSERT_USED(1);
    BA_DynamicArray_AddElementToLast(&array, &number2);
    BA_ASSERT(!BA_DynamicArray_RemoveMatchedElement(&array, &number3, sizeof(int), BA_BOOLEAN_FALSE), "Removed an element that wasn't in the array\n");
    ASSERT_USED(2);
    BA_ASSERT(BA_DynamicArray_RemoveMatchedElement(&array, &number1, sizeof(int), BA_BOOLEAN_FALSE), "Failed to remove matched elements\n");
    BA_ASSERT(array.internalArray[0] == &number2, "Last item did not shift over\n");
    ASSERT_USED(1);
    BA_ASSERT(BA_DynamicArray_Shrink(&array), "Failed to shrink array\n");
    ASSERT_SIZE(1);
    BA_ASSERT(BA_DynamicArray_AddElementToStart(&array, &number1), "Failed to expand the array\n");
    ASSERT_USED(2);
    ASSERT_SIZE(11);
    ASSERT_REALLOCATE(1);

    array.frozen = BA_BOOLEAN_TRUE;

    ASSERT_FROZEN(BA_DynamicArray_AddElementToStart(&array, &number1));
    ASSERT_FROZEN(BA_DynamicArray_AddElementToLast(&array, &number1));
    ASSERT_FROZEN(BA_DynamicArray_RemoveFirstElement(&array));
    ASSERT_FROZEN(BA_DynamicArray_RemoveLastElement(&array));
    ASSERT_FROZEN(BA_DynamicArray_RemoveElementAt(&array, 0));
    ASSERT_FROZEN(BA_DynamicArray_RemoveMatchedElement(&array, &number1, sizeof(int), BA_BOOLEAN_FALSE));
    ASSERT_FROZEN(BA_DynamicArray_Shrink(&array));
    free(array.internalArray);
}
