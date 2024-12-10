// Purpose: stdin prompt manager
// Created on: 12/9/24 @ 7:10 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Internal/CPlusPlusSupport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
const char* BA_Prompt_Get(void);

void BA_Prompt_Set(const char* prompt);
char* BA_Prompt_Show(void);
void BA_Prompt_Cancel(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
