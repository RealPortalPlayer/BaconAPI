// Purpose: Shows the difference between int and boolean.
// Created on: 10/27/22 @ 4:45 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "CPlusPlusSupport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef __cplusplus
typedef int BA_Boolean;
#else
typedef bool BA_Boolean;
#endif
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#ifndef __cplusplus
#   define BA_BOOLEAN_FALSE 0
#   define BA_BOOLEAN_TRUE 1
#else
#   define BA_BOOLEAN_FALSE false
#   define BA_BOOLEAN_TRUE true
#endif
