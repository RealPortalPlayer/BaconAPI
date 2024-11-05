// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/Configuration.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/String.h>
#include <string.h>

void Test(void) {
    {
        FILE* file = fopen("test.settings", "r");

        if (file != NULL) {
            BA_DynamicDictionary* dictionary = BA_Configuration_ParseFromFile(file);

            BA_ASSERT(dictionary != NULL, "Failed to allocate memory for a dictionary\n");

            for (int i = 0; i < dictionary->keys.used; i++) {
                BA_LOGGER_INFO("(%i) %s=%s\n", i, BA_DYNAMICARRAY_GET_ELEMENT(char, dictionary->keys, i), BA_DYNAMICARRAY_GET_ELEMENT(char, dictionary->values, i));
                free(dictionary->keys.internalArray[i]);
                free(dictionary->values.internalArray[i]);
            }

            free(dictionary->keys.internalArray);
            free(dictionary->values.internalArray);
            free(dictionary);
            fclose(file);
        } else
            BA_LOGGER_WARN("Failed to open test file, skipping BA_Configuration_ParseFromFile\n");
    }

    BA_DynamicDictionary* dictionary = BA_Configuration_Parse("test.hello=Hello, World!\n"
                                                              "test.goodbye=Goodbye, Moon!\n"
                                                              "// test.hello2=Hello, Moon?");

    BA_ASSERT(dictionary != NULL, "Failed to allocate memory for a dictionary\n");
    BA_ASSERT(dictionary->keys.used == 2, "Dictionary used does not match expected\n");
    BA_ASSERT(BA_Configuration_GetIndex(dictionary, "test.hello", BA_BOOLEAN_FALSE) == 0, "Index did not match expected\n");
    BA_ASSERT(BA_Configuration_GetIndex(dictionary, "test.goodbye", BA_BOOLEAN_FALSE) == 1, "Index did not match expected\n");
    BA_ASSERT(BA_Configuration_GetIndex(dictionary, "test.hello2", BA_BOOLEAN_FALSE) == -1, "Got index for commented out key\n");
    BA_ASSERT(BA_Configuration_GetIndex(dictionary, "test.goodbye2", BA_BOOLEAN_FALSE) == -1, "Got index for non-existent key\n");
    BA_ASSERT(BA_String_Equals(BA_Configuration_GetValue(dictionary, "test.hello", BA_BOOLEAN_FALSE), "Hello, World!", BA_BOOLEAN_FALSE), "Value does not match expected\n");
    BA_ASSERT(BA_String_Equals(BA_Configuration_GetValue(dictionary, "test.goodbye", BA_BOOLEAN_FALSE), "Goodbye, Moon!", BA_BOOLEAN_FALSE), "Value does not match expected\n");
    BA_ASSERT(BA_Configuration_GetValue(dictionary, "test.hello2", BA_BOOLEAN_FALSE) == NULL, "Found commented out key\n");
    BA_ASSERT(BA_Configuration_GetValue(dictionary, "test.goodbye2", BA_BOOLEAN_FALSE) == NULL, "Found non-existent key\n");
    BA_Configuration_Free(dictionary);
}