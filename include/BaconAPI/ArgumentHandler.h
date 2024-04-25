// Purpose: Helps with argument handling.
// Created on: 3/28/22 @ 9:21 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Internal/CPlusPlusSupport.h"
#include "Internal/Boolean.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    /**
     * Will be the pointer to argumentValue.
     * If short value is defined, then it will be pointing to that instead,
     * no matter what.
     * Short arguments has a higher priority than normal arguments.
     */
    const char** value;

    /**
     * Same for value, but for the indexes instead.
     */
    int* index;
    const char* argumentValue;
    const char* shortValue;
    int argumentIndex;
    int shortIndex;
} BA_ArgumentHandler_ShortResults;

void BA_ArgumentHandler_Initialize(int argc, char** argv);
int BA_ArgumentHandler_GetCount(void);
char** BA_ArgumentHandler_GetVector(void);
const char* BA_ArgumentHandler_GetHelpMessage(void);

/**
  *  @return The index if the argument was found, -1 if not.
  */
int BA_ArgumentHandler_GetIndex(const char* argument, BA_Boolean ignoreDontParse);

/**
  * @return The value next to the argument if found, NULL if not.
  */
const char* BA_ArgumentHandler_GetValue(const char* argument, BA_Boolean ignoreDontParse);

int BA_ArgumentHandler_GetInformationWithShort(const char* argument, const char* shortArgument, BA_Boolean ignoreDontParse,
                                                BA_ArgumentHandler_ShortResults* results);

BA_Boolean BA_ArgumentHandler_ContainsArgumentOrShort(const char* argument, const char* shortArgument, BA_Boolean ignoreDontParse);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BA_ARGUMENTHANDLER_HELP_MESSAGE_NO_SHORT(argument, description) argument ": " description
#define BA_ARGUMENTHANDLER_HELP_MESSAGE(argument, short, description) BA_ARGUMENTHANDLER_HELP_MESSAGE_NO_SHORT(argument " (" short ")", description)
#define BA_ARGUMENTHANDLER_HELP_MESSAGE_ARGUMENTS(argument, arguments, short, description) BA_ARGUMENTHANDLER_HELP_MESSAGE(argument " " arguments, short, description)
#define BA_ARGUMENTHANDLER_HELP_MESSAGE_NO_SHORT_ARGUMENTS(argument, arguments, description) BA_ARGUMENTHANDLER_HELP_MESSAGE_NO_SHORT(argument " " arguments, description)
