// Purpose: Make assertions easier.
// Created on: 3/30/22 @ 11:45 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdlib.h>
#include <stdio.h>

#include "BaconAPI/Logger.h"
#include "BaconAPI/Internal/Boolean.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean BA_Assert_CheckLogsEnabled(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BA_ASSERT(check, ...) \
do {                          \
    if (BA_Assert_CheckLogsEnabled()) \
        BA_LOGGER_TRACE("Assert checking: %s\n", #check); \
    if (!(check)) {           \
        BA_Logger_LogImplementation(BA_BOOLEAN_TRUE, BA_LOGGER_LOG_LEVEL_FATAL, "Assertion Failed\nCode: %s\nMessage: ", #check); \
        BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_FATAL, __VA_ARGS__); \
        abort();              \
        while (BA_BOOLEAN_TRUE) continue; \
    }                         \
} while (BA_BOOLEAN_FALSE)

#define BA_ASSERT_ALWAYS(message) BA_ASSERT(BA_BOOLEAN_FALSE, message)
#define BA_ASSERT_NOT_IMPLEMENTED() BA_ASSERT_ALWAYS("This function is currently not implemented\n")
