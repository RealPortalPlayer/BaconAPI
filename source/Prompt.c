// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BaconAPI/Prompt.h"
#include "BaconAPI/OperatingSystem.h"
#include "BaconAPI/Logger.h"

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#   include <termios.h>
#endif

#define BA_PROMPT_INPUT_SIZE 4024

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static const char* baPrompt = NULL;
static volatile BA_Boolean baPromptRunning = BA_BOOLEAN_TRUE;

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
static struct termios baPromptOldCapabilities;
static struct termios baPromptNewCapabilities;
#endif

const char* BA_Prompt_Get(void) {
    return baPrompt;
}

void BA_Prompt_Set(const char* prompt) {
    baPrompt = prompt;
}

char* BA_Prompt_Show(void) {
    BA_Boolean printed = BA_BOOLEAN_FALSE;
    int written = 0;
    char* input = calloc(1, sizeof(char) * (BA_PROMPT_INPUT_SIZE + 1));

    if (input == NULL)
        return NULL;

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT // TODO: Do the same for Windows?
    tcgetattr(0, &baPromptOldCapabilities);

    baPromptNewCapabilities = baPromptOldCapabilities;
    baPromptNewCapabilities.c_lflag &= ~ICANON;
    baPromptNewCapabilities.c_lflag &= ~ECHO;

    tcsetattr(0, TCSANOW, &baPromptNewCapabilities);
#endif

    baPromptRunning = BA_BOOLEAN_TRUE;

    while (baPromptRunning) {
        if (!printed) {
            BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "%s", baPrompt);
            fflush(stdout);

            printed = BA_BOOLEAN_TRUE;
        }

        char character;

        read(0, &character, 1);

        if (character == 127) {
            if (written == 0)
                continue;

            memmove(input + written - 1, input + written, BA_PROMPT_INPUT_SIZE - written);
            BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "\b \b"); // HACK: This is stupid
            fflush(stdout);

            written--;
            continue;
        }

        BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "%c", character);
        fflush(stdout);

        if (written >= BA_PROMPT_INPUT_SIZE || character == '\n')
            break;

        input[written++] = character;
    }

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    tcsetattr(0, TCSANOW, &baPromptOldCapabilities);
#endif

    if (!baPromptRunning) {
        free(input);
        return NULL;
    }

    return input;
}

void BA_Prompt_Cancel(void) {
    baPromptRunning = BA_BOOLEAN_FALSE;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
