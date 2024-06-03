// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Storage/DynamicDictionary.h>
#include <BaconAPI/Debugging/Assert.h>

#define ASSERT_USED(expected) \
BA_ASSERT(dictionary.keys.used == expected, "Key used desync\n"); \
BA_ASSERT(dictionary.values.used == expected, "Value used desync\n")
#define ASSERT_SIZE(expected) \
BA_ASSERT(dictionary.keys.size == expected, "Keys size desync\n"); \
BA_ASSERT(dictionary.values.size == expected, "Values size desync\n")
#define ASSERT_REALLOCATE(expected) \
BA_ASSERT(dictionary.keys.calledReallocate == expected, "Keys unexpected reallocate count\n"); \
BA_ASSERT(dictionary.values.calledReallocate == expected, "Values unexpected reallocate count\n")
#define ASSERT_FROZEN(call) \
BA_ASSERT(!call, "Modified frozen dictionary\n"); \
ASSERT_USED(2);             \
ASSERT_SIZE(11);            \
ASSERT_REALLOCATE(1);       \
BA_ASSERT(dictionary.keys.frozen && dictionary.values.frozen, "Dictionary internal arrays did not freeze\n"); \
dictionary.frozen = BA_BOOLEAN_FALSE; \
BA_DynamicDictionary_AddElementToStart(&dictionary, &key1, &value1); /* To reset frozen */ \
BA_DynamicDictionary_RemoveFirstElement(&dictionary); \
BA_ASSERT(!dictionary.keys.frozen && !dictionary.values.frozen, "Dictionary internal arrays did not unfreeze\n"); \
dictionary.frozen = BA_BOOLEAN_TRUE

int main(int argc, char** argv) {
    BA_ArgumentHandler_Initialize(argc, argv);

    BA_DynamicDictionary dictionary;
    int key1 = 0;
    int value1 = 1;
    int key2 = 2;
    int value2 = 3;
    int key3 = 4;
    int value3 = 5;

    BA_ASSERT(BA_DynamicDictionary_Create(&dictionary, 10), "Failed to create dictionary\n");
    BA_ASSERT(BA_DynamicDictionary_GetElementIndexFromKey(&dictionary, &key1, sizeof(int)) == -1, "Found index when key doesn't exist\n");
    BA_ASSERT(BA_DynamicDictionary_GetElementIndexFromValue(&dictionary, &value1, sizeof(int)) == -1, "Found index when value doesn't exist\n");
    BA_ASSERT(BA_DynamicDictionary_GetElementKeyViaValue(&dictionary, &value1, sizeof(int)) == NULL, "Found key when value doesn't exist\n");
    BA_ASSERT(BA_DynamicDictionary_GetElementValueViaKey(&dictionary, &key1, sizeof(int)) == NULL, "Found value when key doesn't exist\n");
    BA_ASSERT(BA_DynamicDictionary_AddElementToStart(&dictionary, &key1, &value1), "Failed to add item\n");
    BA_ASSERT(BA_DynamicDictionary_GetElementIndexFromKey(&dictionary, &key1, sizeof(int)) == 0, "Invalid index\n");
    BA_ASSERT(BA_DynamicDictionary_GetElementIndexFromValue(&dictionary, &value1, sizeof(int)) == 0, "Invalid index\n");
    BA_ASSERT(BA_DynamicDictionary_GetElementKeyViaValue(&dictionary, &value1, sizeof(int)) == &key1, "Failed to find key\n");
    BA_ASSERT(BA_DynamicDictionary_GetElementValueViaKey(&dictionary, &key1, sizeof(int)) == &value1, "Failed to find value\n");
    ASSERT_USED(1);
    BA_ASSERT(BA_DynamicDictionary_AddElementToLast(&dictionary, &key2, &value2), "Failed to add item\n");
    BA_ASSERT(dictionary.keys.internalArray[0] == &key1 && dictionary.values.internalArray[0] == &value1, "First item is not correct\n");
    BA_ASSERT(BA_DYNAMICARRAY_GET_LAST_ELEMENT(int, dictionary.keys) == &key2 && BA_DYNAMICARRAY_GET_LAST_ELEMENT(int, dictionary.values) == &value2, "Last item is not correct\n");
    ASSERT_USED(2);
    BA_ASSERT(BA_DynamicDictionary_RemoveFirstElement(&dictionary), "Failed to remove first item\n");
    BA_ASSERT(dictionary.keys.internalArray[0] == &key2 && dictionary.values.internalArray[0] == &value2, "Last item did not shift over\n");
    ASSERT_USED(1);
    BA_DynamicDictionary_AddElementToStart(&dictionary, &key1, &value1);
    BA_ASSERT(BA_DynamicDictionary_RemoveLastElement(&dictionary), "Failed to remove the last item\n");
    BA_ASSERT(dictionary.keys.internalArray[0] == &key1 && dictionary.values.internalArray[0] == &value1, "First item got destroyed\n");
    ASSERT_USED(1);
    BA_DynamicDictionary_AddElementToLast(&dictionary, &key2, &value2);
    BA_ASSERT(BA_DynamicDictionary_RemoveElementAt(&dictionary, 1), "Failed to remove element at index 1\n");
    BA_ASSERT(dictionary.keys.internalArray[0] == &key1 && dictionary.values.internalArray[0] == &value1, "First item got destroyed\n");
    ASSERT_USED(1);
    BA_DynamicDictionary_AddElementToLast(&dictionary, &key2, &value2);
    BA_ASSERT(!BA_DynamicDictionary_RemoveElementViaKey(&dictionary, &key3, sizeof(int), BA_BOOLEAN_FALSE), "Removed an element that wasn't in the dictionary\n");
    BA_ASSERT(!BA_DynamicDictionary_RemoveElementViaValue(&dictionary, &value3, sizeof(int), BA_BOOLEAN_FALSE), "Removed an element that wasn't in the dictionary\n");
    ASSERT_USED(2);
    BA_ASSERT(BA_DynamicDictionary_RemoveElementViaKey(&dictionary, &key1, sizeof(int), BA_BOOLEAN_FALSE), "Failed to remove matched key\n");
    BA_ASSERT(dictionary.keys.internalArray[0] == &key2 && dictionary.values.internalArray[0] == &value2, "Last item did not shift over\n");
    ASSERT_USED(1);
    BA_DynamicDictionary_AddElementToStart(&dictionary, &key1, &value1);
    BA_ASSERT(BA_DynamicDictionary_RemoveElementViaValue(&dictionary, &value2, sizeof(int), BA_BOOLEAN_FALSE), "Failed to remove matched value\n");
    BA_ASSERT(dictionary.keys.internalArray[0] == &key1 && dictionary.values.internalArray[0] == &value1, "First item got destroyed\n");
    ASSERT_USED(1);
    // TODO: BA_DynamicDictionary_GetElementsKeyViaValue, BA_DynamicDictionary_GetElementsValueViaKey
    BA_ASSERT(BA_DynamicDictionary_Shrink(&dictionary), "Failed to shrink dictionary\n");
    ASSERT_USED(1);
    BA_ASSERT(BA_DynamicDictionary_AddElementToLast(&dictionary, &key2, &value2), "Failed to expand the array\n");
    ASSERT_USED(2);
    ASSERT_SIZE(11);
    ASSERT_REALLOCATE(1);

    dictionary.frozen = BA_BOOLEAN_TRUE;

    ASSERT_FROZEN(BA_DynamicDictionary_AddElementToStart(&dictionary, &key1, &value1));
    ASSERT_FROZEN(BA_DynamicDictionary_AddElementToLast(&dictionary, &key1, &value1));
    ASSERT_FROZEN(BA_DynamicDictionary_RemoveFirstElement(&dictionary));
    ASSERT_FROZEN(BA_DynamicDictionary_RemoveLastElement(&dictionary));
    ASSERT_FROZEN(BA_DynamicDictionary_RemoveElementAt(&dictionary, 0));
    ASSERT_FROZEN(BA_DynamicDictionary_RemoveElementViaKey(&dictionary, &key1, sizeof(int), BA_BOOLEAN_FALSE));
    ASSERT_FROZEN(BA_DynamicDictionary_RemoveElementViaValue(&dictionary, &value1, sizeof(int), BA_BOOLEAN_FALSE));
    ASSERT_FROZEN(BA_DynamicDictionary_Shrink(&dictionary));
    free(dictionary.keys.internalArray);
    free(dictionary.values.internalArray);
}
