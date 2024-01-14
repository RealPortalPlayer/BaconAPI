// Purpose: Stores the testing macros
// Created on: 1/9/24 @ 1:19 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdlib.h>
#include <stdio.h>

#define ASSERT(check) \
do {                  \
    printf("%s", #check); \
    if (!(check)) {   \
        printf(": FAIL\n"); \
        abort();      \
    }                 \
    printf("\n");     \
} while (0)

