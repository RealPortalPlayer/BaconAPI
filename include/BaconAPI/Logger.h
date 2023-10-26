// Purpose: Makes logging easier.
// Created on: 3/29/22 @ 2:01 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <stdarg.h>
#include <stdio.h>

#include "Internal/OperatingSystem.h"
#include "Internal/CPlusPlusSupport.h"
#include "Internal/Boolean.h"

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   define BA_LOGGER_PRINTF_ATTRIBUTE __attribute__((__format__(__printf__, 3, 4)))
#elif BA_OPERATINGSYSTEM_WINDOWS
#   define BA_LOGGER_PRINTF_ATTRIBUTE
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef enum {
    BA_LOGGER_LOG_LEVEL_FATAL,
    BA_LOGGER_LOG_LEVEL_ERROR,
    BA_LOGGER_LOG_LEVEL_WARN,
    BA_LOGGER_LOG_LEVEL_INFO,
#ifdef BA_ALLOW_DEBUG_LOGS
    BA_LOGGER_LOG_LEVEL_DEBUG,
    BA_LOGGER_LOG_LEVEL_TRACE,
#endif
    BA_LOGGER_LOG_LEVEL_NULL
} BA_Logger_LogLevels;

BA_Logger_LogLevels BA_Logger_GetLogLevel(void);
BA_LOGGER_PRINTF_ATTRIBUTE void BA_Logger_LogImplementation(BA_Boolean includeHeader, BA_Logger_LogLevels logLevel, const char* message, ...);
void BA_Logger_LogHeader(FILE* output, BA_Logger_LogLevels logLevel);
BA_Boolean BA_Logger_IsLevelEnabled(BA_Logger_LogLevels logLevel);

void BA_Logger_SetLogLevel(BA_Logger_LogLevels logLevel);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#ifdef BA_ALLOW_DEBUG_LOGS
#   define BA_LOGGER_TRACE(...) BA_Logger_LogImplementation(BA_BOOLEAN_TRUE, BA_LOGGER_LOG_LEVEL_TRACE, __VA_ARGS__)
#   define BA_LOGGER_DEBUG(...) BA_Logger_LogImplementation(BA_BOOLEAN_TRUE, BA_LOGGER_LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#   define BA_LOGGER_TRACE(...)
#   define BA_LOGGER_DEBUG(...)
#endif
#define BA_LOGGER_INFO(...) BA_Logger_LogImplementation(BA_BOOLEAN_TRUE, BA_LOGGER_LOG_LEVEL_INFO, __VA_ARGS__)
#define BA_LOGGER_WARN(...) BA_Logger_LogImplementation(BA_BOOLEAN_TRUE, BA_LOGGER_LOG_LEVEL_WARN, __VA_ARGS__)
#define BA_LOGGER_ERROR(...) BA_Logger_LogImplementation(BA_BOOLEAN_TRUE, BA_LOGGER_LOG_LEVEL_ERROR, __VA_ARGS__)
#define BA_LOGGER_FATAL(...) BA_Logger_LogImplementation(BA_BOOLEAN_TRUE, BA_LOGGER_LOG_LEVEL_FATAL, __VA_ARGS__)
