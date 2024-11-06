// Purpose: Translation system designed around the Configuration, and StringManager, system
// Created on: 11/5/24 @ 4:46 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Internal/CPlusPlusSupport.h"
#include "StringManager.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
const char* BA_Translations_GetLanguageCode(const char* code);
int BA_Translations_GetTranslationId(const char* key);

void BA_Translations_LoadLanguage(const char* code, const char* buffer);
void BA_Translations_Destroy(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BA_TRANSLATIONS_PRELOAD(functionName, key) \
const char* functionName(void) {                   \
static int translationId = -1;                 \
if (translationId == -1)                       \
translationId = BA_Translations_GetTranslationId(key); \
return BA_StringManager_Get(translationId);    \
}
