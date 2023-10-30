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
BA_CPLUSPLUS_SUPPORT_GUARD_END()
