// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>

#include "BaconAPI/ArgumentHandler.h"
#include "BaconAPI/Internal/CPlusPlusSupport.h"
#include "BaconAPI/BuiltInArguments.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static int baArgumentHandlerAddedArgumentsCount;
static char** baArgumentHandlerArgumentVector = NULL;

void BA_ArgumentHandler_Initialize(int argc, char** argv) {
    baArgumentHandlerAddedArgumentsCount = argc;
    baArgumentHandlerArgumentVector = argv;
}

int BA_ArgumentHandler_GetCount(void) {
    return baArgumentHandlerAddedArgumentsCount;
}

char** BA_ArgumentHandler_GetVector(void) {
    return baArgumentHandlerArgumentVector;
}

static int BA_ArgumentHandler_GetArgumentIndexImplementation(const char* argument) {
    if (baArgumentHandlerArgumentVector == NULL)
        return -1;
    
    for (int i = 1; i < baArgumentHandlerAddedArgumentsCount; i++) {
        if (strcmp(baArgumentHandlerArgumentVector[i], argument) != 0)
            continue;

        return i;
    }

    return -1;
}

int BA_ArgumentHandler_GetIndex(const char* argument, BA_Boolean ignoreDontParse) {
    static int dontParse = -2;

    if (dontParse == -2)
        dontParse = BA_ArgumentHandler_GetArgumentIndexImplementation(BA_BUILTINARGUMENTS_DONT_PARSE);

    int index = BA_ArgumentHandler_GetArgumentIndexImplementation(argument);

    return ignoreDontParse || dontParse == -1 || index < dontParse ? index : -1;
}

const char* BA_ArgumentHandler_GetValue(const char* argument, int ignoreDontParse) {
    int index = BA_ArgumentHandler_GetIndex(argument, ignoreDontParse);

    return index != -1 && index != baArgumentHandlerAddedArgumentsCount - 1 ? baArgumentHandlerArgumentVector[index + 1] : NULL;
}

int BA_ArgumentHandler_GetInformationWithShort(const char* argument, const char* shortArgument, BA_Boolean ignoreDontParse,
                                                BA_ArgumentHandler_ShortResults* results) {
    int returnResult = 0;

    results->argumentIndex = BA_ArgumentHandler_GetIndex(argument, ignoreDontParse);
    results->shortIndex = BA_ArgumentHandler_GetIndex(shortArgument, ignoreDontParse);

    if (results->argumentIndex != -1 && results->argumentIndex != baArgumentHandlerAddedArgumentsCount - 1) {
        results->argumentValue = baArgumentHandlerArgumentVector[results->argumentIndex + 1];
        results->value = &results->argumentValue;
        results->index = &results->argumentIndex;
        returnResult++;
    }

    if (results->shortIndex != -1 && results->shortIndex != baArgumentHandlerAddedArgumentsCount - 1) {
        results->shortValue = baArgumentHandlerArgumentVector[results->shortIndex + 1];
        results->value = &results->shortValue;
        results->index = &results->shortIndex;
        returnResult++;
    }

    return returnResult;
}

int BA_ArgumentHandler_ContainsArgumentOrShort(const char* argument, const char* shortArgument, BA_Boolean ignoreDontParse) {
    return BA_ArgumentHandler_GetIndex(argument, ignoreDontParse) != -1 ||
           BA_ArgumentHandler_GetIndex(shortArgument, ignoreDontParse) != -1;
}

const char* BA_ArgumentHandler_GetHelpMessage(void) {
#ifdef BA_ALLOW_DEBUG_LOGS
#   define BA_LAUNCHER_HELP_LOGGER_EXTRA_ARGUMENTS "/trace/trc/debug/dbg"
#else
#   define BA_LAUNCHER_HELP_LOGGER_EXTRA_ARGUMENTS
#endif
    
    return BA_ARGUMENTHANDLER_HELP_MESSAGE_NO_SHORT_ARGUMENTS(BA_BUILTINARGUMENTS_DONT_PARSE, "<argument>", "Do not parse arguments beyond this point\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE(BA_BUILTINARGUMENTS_DISABLE_ANSI_COLORING, BA_BUILTINARGUMENTS_DISABLE_ANSI_COLORING_SHORT, "Disable log colors\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE_ARGUMENTS(BA_BUILTINARGUMENTS_LOG_LEVEL, "<null" BA_LAUNCHER_HELP_LOGGER_EXTRA_ARGUMENTS "/warn/wrn/error/err/fatal/ftl>", BA_BUILTINARGUMENTS_LOG_LEVEL_SHORT, "Sets the current log level\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE(BA_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS, BA_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS_SHORT, "Prevent the client from changing the log level\n")
#ifdef BA_ALLOW_DEBUG_LOGS
           BA_ARGUMENTHANDLER_HELP_MESSAGE(BA_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS, BA_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS_SHORT, "Do not log assert checks, does nothing if log level is not trace\n")
#endif
           BA_ARGUMENTHANDLER_HELP_MESSAGE(BA_BUILTINARGUMENTS_DISABLE_LOG_HEADER, BA_BUILTINARGUMENTS_DISABLE_LOG_HEADER_SHORT, "Do not log the log level header\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE(BA_BUILTINARGUMENTS_SINGLE_THREADED, BA_BUILTINARGUMENTS_SINGLE_THREADED_SHORT, "Don't allow thread creation");
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
