// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "BaconAPI/Logger.h"
#include "BaconAPI/ANSI.h"
#include "BaconAPI/Internal/CPlusPlusSupport.h"
#include "BaconAPI/ArgumentHandler.h"
#include "BaconAPI/BuiltInArguments.h"
#include "BaconAPI/Thread.h"
#include "BaconAPI/String.h"

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#elif BA_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#   include <io.h>
#   define fileno _fileno
#   define write(file, message, size) _write(file, message, (unsigned) size) // HACK: This is a stupid idea, but it's the only way to make MSVC shut up.
#endif

#ifdef BA_LOGGER_INITIALIZER_DEBUG_LOGS
#   define BA_LOGGER_INITIALIZER_DEBUG(...) printf(__VA_ARGS__)
#else
#   define BA_LOGGER_INITIALIZER_DEBUG(...)
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static BA_Logger_LogLevels baLoggerCurrentLogLevel = BA_LOGGER_LOG_LEVEL_INFO;
static BA_Thread_Lock baLoggerLock;

BA_Logger_LogLevels BA_Logger_GetLogLevel(void) {
    return baLoggerCurrentLogLevel;
}

void BA_Logger_LogImplementation(int includeHeader, BA_Logger_LogLevels logLevel, const char* message, ...) {
    static int alwaysUseStdout = -1;

    if (alwaysUseStdout == -1)
        alwaysUseStdout = BA_ArgumentHandler_ContainsArgumentOrShort(BA_BUILTINARGUMENTS_ALWAYS_USE_STDOUT,
                                                                      BA_BUILTINARGUMENTS_ALWAYS_USE_STDOUT_SHORT, 0);

    {
        static BA_Boolean initialized = BA_BOOLEAN_FALSE;
        static BA_Boolean initializing = BA_BOOLEAN_FALSE;

        if (!initialized) {
            BA_LOGGER_INITIALIZER_DEBUG("Initializing logger\n");

            if (initializing) {
                fprintf(stderr, "Logger called self during initializing, this is 100%% a bug with the engine\n"
                                "includeHeader: %i\n"
                                "logLevel: %i\n"
                                "message: %s\n", includeHeader, logLevel, message);
                abort();
            }

            BA_LOGGER_INITIALIZER_DEBUG("Creating thread lock\n");

            if (!BA_Thread_CreateLock(&baLoggerLock)) {
                printf("Failed to initialize logger lock: %i\n", errno);
                abort();
            }

            initializing = BA_BOOLEAN_TRUE;

#ifndef BA_LOGGER_FORCE_ENABLE_ALL_LEVELS
            BA_LOGGER_INITIALIZER_DEBUG("Getting log type\n");

            {
                BA_ArgumentHandler_ShortResults results;
                BA_Logger_LogLevels original = baLoggerCurrentLogLevel;

                if (BA_ArgumentHandler_GetInformationWithShort(BA_BUILTINARGUMENTS_LOG_LEVEL,
                                                                BA_BUILTINARGUMENTS_LOG_LEVEL_SHORT, 0, &results) != 0) {
                    if (BA_String_Equals(*results.value, "null", BA_BOOLEAN_TRUE))
                        baLoggerCurrentLogLevel = BA_LOGGER_LOG_LEVEL_NULL;
#ifdef BA_ALLOW_DEBUG_LOGS
                    else if (BA_String_Equals(*results.value, "trace", BA_BOOLEAN_TRUE) ||
                             BA_String_Equals(*results.value, "trc", BA_BOOLEAN_TRUE))
                        baLoggerCurrentLogLevel = BA_LOGGER_LOG_LEVEL_TRACE;
                    else if (BA_String_Equals(*results.value, "debug", BA_BOOLEAN_TRUE) ||
                             BA_String_Equals(*results.value, "dbg", BA_BOOLEAN_TRUE))
                        baLoggerCurrentLogLevel = BA_LOGGER_LOG_LEVEL_DEBUG;
#endif
                    else if (BA_String_Equals(*results.value, "info", BA_BOOLEAN_TRUE) ||
                             BA_String_Equals(*results.value, "inf", BA_BOOLEAN_TRUE))
                        baLoggerCurrentLogLevel = BA_LOGGER_LOG_LEVEL_INFO;
                    else if (BA_String_Equals(*results.value, "warn", BA_BOOLEAN_TRUE) ||
                             BA_String_Equals(*results.value, "wrn", BA_BOOLEAN_TRUE))
                        baLoggerCurrentLogLevel = BA_LOGGER_LOG_LEVEL_WARN;
                    else if (BA_String_Equals(*results.value, "error", BA_BOOLEAN_TRUE) ||
                             BA_String_Equals(*results.value, "err", BA_BOOLEAN_TRUE))
                        baLoggerCurrentLogLevel = BA_LOGGER_LOG_LEVEL_ERROR;
                    else if (BA_String_Equals(*results.value, "fatal", BA_BOOLEAN_TRUE) ||
                             BA_String_Equals(*results.value, "ftl", BA_BOOLEAN_TRUE))
                        baLoggerCurrentLogLevel = BA_LOGGER_LOG_LEVEL_FATAL;
                    else if (BA_Logger_IsLevelEnabled(BA_LOGGER_LOG_LEVEL_ERROR)) {
                        BA_Logger_LogHeader(stderr, BA_LOGGER_LOG_LEVEL_ERROR);
                        printf("Invalid log type: %s\n", *results.value);
                    }
                }

#ifdef BA_LOGGER_INITIALIZER_DEBUG_LOGS
                if (baLoggerCurrentLogLevel != original)
                    BA_LOGGER_INITIALIZER_DEBUG("Log type: %i (from: %i)\n", baLoggerCurrentLogLevel, original);
                else
                    BA_LOGGER_INITIALIZER_DEBUG("Log type: %i\n", baLoggerCurrentLogLevel);
#endif
            }
#endif

            initialized = BA_BOOLEAN_TRUE;
            initializing = BA_BOOLEAN_FALSE;
        }
    }

    BA_Thread_UseLock(&baLoggerLock);

    if (!BA_Logger_IsLevelEnabled(logLevel)) {
        BA_Thread_Unlock(&baLoggerLock);
        return;
    }

    static BA_Boolean antiRecursiveLog = BA_BOOLEAN_FALSE;
    FILE* output = stdout;

    if (!alwaysUseStdout && (logLevel == BA_LOGGER_LOG_LEVEL_ERROR || logLevel == BA_LOGGER_LOG_LEVEL_FATAL))
        output = stderr;

    if (antiRecursiveLog) {
        fprintf(output, "Logger called self, this is 100%% a bug with the engine\n"
                        "includeHeader: %i\n"
                        "logLevel: %i\n"
                        "message: %s\n", includeHeader, logLevel, message);
        abort();
    }

    antiRecursiveLog = BA_BOOLEAN_TRUE;

    if (includeHeader)
        BA_Logger_LogHeader(output, logLevel);

    va_list arguments;

    va_start(arguments, message);
    vfprintf(output, message, arguments);
    fflush(output); // HACK
    va_end(arguments);

    antiRecursiveLog = BA_BOOLEAN_FALSE;

    BA_Thread_Unlock(&baLoggerLock);
}

void BA_Logger_LogHeader(FILE* output, BA_Logger_LogLevels logLevel) {
    if (!BA_Logger_IsLevelEnabled(logLevel))
        return;

    {
        static int dontLogHeader = -1;

        if (dontLogHeader == -1)
            dontLogHeader = BA_ArgumentHandler_ContainsArgumentOrShort(BA_BUILTINARGUMENTS_DISABLE_LOG_HEADER, BA_BUILTINARGUMENTS_DISABLE_LOG_HEADER_SHORT, 0);

        if (dontLogHeader)
            return;
    }

#define BA_LOGGER_SAFE_PUTS(message) write(fileno(output), message, strlen(message))
    switch (logLevel) {
#ifdef BA_ALLOW_DEBUG_LOGS
        case BA_LOGGER_LOG_LEVEL_TRACE:
            if (BA_ANSI_IsEnabled())
                BA_LOGGER_SAFE_PUTS(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_BRIGHT_FOREGROUND_BLUE));

            BA_LOGGER_SAFE_PUTS("[TRC] ");
            break;

        case BA_LOGGER_LOG_LEVEL_DEBUG:
            if (BA_ANSI_IsEnabled())
                BA_LOGGER_SAFE_PUTS(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_BRIGHT_FOREGROUND_MAGENTA));

            BA_LOGGER_SAFE_PUTS("[DBG] ");
            break;
#endif

        case BA_LOGGER_LOG_LEVEL_INFO:
            if (BA_ANSI_IsEnabled())
                BA_LOGGER_SAFE_PUTS(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_BRIGHT_FOREGROUND_CYAN));

            BA_LOGGER_SAFE_PUTS("[INF] ");
            break;

        case BA_LOGGER_LOG_LEVEL_WARN:
            if (BA_ANSI_IsEnabled())
                BA_LOGGER_SAFE_PUTS(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_BRIGHT_FOREGROUND_YELLOW));

            BA_LOGGER_SAFE_PUTS("[WRN] ");
            break;

        case BA_LOGGER_LOG_LEVEL_ERROR:
            if (BA_ANSI_IsEnabled())
                BA_LOGGER_SAFE_PUTS(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_BRIGHT_FOREGROUND_RED));

            BA_LOGGER_SAFE_PUTS("[ERR] ");
            break;

        case BA_LOGGER_LOG_LEVEL_FATAL:
            if (BA_ANSI_IsEnabled()) {
                BA_LOGGER_SAFE_PUTS(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_BOLD));
                BA_LOGGER_SAFE_PUTS(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_FOREGROUND_RED));
            }

            BA_LOGGER_SAFE_PUTS("[FTL] ");
            break;

        default:
            BA_LOGGER_SAFE_PUTS("[UNK] ");
            break;
    }

    if (BA_ANSI_IsEnabled())
        BA_LOGGER_SAFE_PUTS(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_RESET));
#undef BA_LOGGER_SAFE_PUTS
}

BA_Boolean BA_Logger_IsLevelEnabled(BA_Logger_LogLevels logLevel) {
#ifndef BA_LOGGER_FORCE_ENABLE_ALL_LEVELS
    return baLoggerCurrentLogLevel != BA_LOGGER_LOG_LEVEL_NULL && logLevel <= baLoggerCurrentLogLevel;
#else
    return BA_BOOLEAN_TRUE;
#endif
}

void BA_Logger_SetLogLevel(BA_Logger_LogLevels logLevel) {
    static int dontChangeLogLevels = -1;

    if (dontChangeLogLevels == -1)
        dontChangeLogLevels = BA_ArgumentHandler_ContainsArgumentOrShort(BA_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS, BA_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS_SHORT, 0);

    if (dontChangeLogLevels)
        return;

    baLoggerCurrentLogLevel = logLevel;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
