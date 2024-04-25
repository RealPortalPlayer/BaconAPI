// Purpose: Simplify bitwise operations.
// Created on: 9/12/22 @ 11:36 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "../Internal/Boolean.h"

#define BA_BITWISE_IS_BIT_SET(mainBit, compareBit) ((mainBit & compareBit) == compareBit)
#define BA_BITWISE_SET_BIT(mainBit, bit) mainBit |= bit
#define BA_BITWISE_UNSET_BIT(mainBit, bit) mainBit &= ~bit
#define BA_BITWISE_TOGGLE_BIT(mainBit, bit) \
do {                                        \
    if (BA_BITWISE_IS_BIT_SET(mainBit, bit)) \
        BA_BITWISE_UNSET_BIT(mainBit, bit); \
    else                                    \
        BA_BITWISE_SET_BIT(mainBit, bit);   \
} while (BA_BOOLEAN_FALSE)
