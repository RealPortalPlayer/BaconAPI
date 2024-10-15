// Purpose: Manages strings for you
// Created on: 10/14/24 @ 1:51 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Internal/CPlusPlusSupport.h"
#include "Internal/Boolean.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void* BA_StringManager_Get(int index);
BA_Boolean BA_StringManager_IsWideString(int index);

int BA_StringManager_Allocate(const void* originalString, BA_Boolean isWideString);
void BA_StringManager_Deallocate(int index);
void BA_StringManager_Replace(int index, const void* newString, BA_Boolean isWideString);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
