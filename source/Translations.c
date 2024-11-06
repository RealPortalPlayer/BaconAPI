// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stddef.h>

#include "BaconAPI/Translations.h"
#include "BaconAPI/Storage/DynamicDictionary.h"
#include "BaconAPI/Debugging/Assert.h"
#include "BaconAPI/Configuration.h"
#include "BaconAPI/StringManager.h"
#include "BaconAPI/String.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static const char* baTranslationsLanguageCode = NULL;
static BA_DynamicArray baTranslationsKeys;

const char* BA_Translations_GetLanguageCode(const char* code) {
    return baTranslationsLanguageCode;
}

int BA_Translations_GetTranslationId(const char* key) {
    for (int i = 0; i < baTranslationsKeys.used; i++) {
        if (!BA_String_Equals(BA_DYNAMICARRAY_GET_ELEMENT(char, baTranslationsKeys, i), key, BA_BOOLEAN_FALSE))
            continue;

        return i;
    }

    return -1;
}

void BA_Translations_LoadLanguage(const char* code, const char* buffer) {
    if (baTranslationsLanguageCode == NULL)
        BA_ASSERT(BA_DynamicArray_Create(&baTranslationsKeys, 100), "Failed to allocate memory to store translation keys\n");

    baTranslationsLanguageCode = code;

    BA_DynamicDictionary* parsedConfiguration = BA_Configuration_Parse(buffer);

    BA_ASSERT(parsedConfiguration != NULL, "Failed to allocate memory for parsed language\n");

    // OPTIMIZE: There has to be a better way to do this
    // FIXME: If a key doesn't exist in the new language, then nothing will happen to it. That's a memory leak. Sad
    // FIXME: Actually, it's two memory leaks, since the value doesn't get deleted either
    for (int i = 0; i < parsedConfiguration->keys.used; i++) {
        char* key = BA_DYNAMICARRAY_GET_ELEMENT(char, parsedConfiguration->keys, i);
        char* value = BA_DYNAMICARRAY_GET_ELEMENT(char, parsedConfiguration->values, i);

        {
            BA_Boolean replaced = BA_BOOLEAN_FALSE;

            for (int j = 0; j < baTranslationsKeys.used; j++) {
                char* key2 = BA_DYNAMICARRAY_GET_ELEMENT(char, baTranslationsKeys, j);

                if (!BA_String_Equals(key2, key, BA_BOOLEAN_FALSE))
                    continue;

                replaced = BA_BOOLEAN_TRUE;

                BA_StringManager_Replace(j, value, BA_BOOLEAN_FALSE);
                free(key);
                free(value);
                break;
            }

            if (replaced)
                continue;
        }

        BA_StringManager_Allocate(value, BA_BOOLEAN_FALSE);
        BA_DynamicArray_AddElementToLast(&baTranslationsKeys, key);
        free(value);
    }

    free(parsedConfiguration->keys.internalArray);
    free(parsedConfiguration->values.internalArray);
    free(parsedConfiguration);
}

void BA_Translations_Destroy(void) {
    for (int i = 0; i < baTranslationsKeys.used; i++)
        free(baTranslationsKeys.internalArray[i]);

    free(baTranslationsKeys.internalArray);
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
