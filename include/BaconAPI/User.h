// Purpose: User specific information.
// Created on: 9/11/22 @ 1:26 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Internal/CPlusPlusSupport.h"
#include "Internal/Boolean.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean BA_User_IsAdministrator(void);
const char* BA_User_GetName(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
