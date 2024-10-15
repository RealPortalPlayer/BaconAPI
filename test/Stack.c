// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdlib.h>

#include <BaconAPI/Logger.h>
#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Debugging/Stack.h>

void Function4(void) {
    char* callTrace = BA_Stack_GetCallTrace();

    BA_LOGGER_INFO("Stack:\n%s\n", callTrace);
    free(callTrace);
}

void Function3(void) {
    BA_LOGGER_INFO("Function3\n");
    Function4();
}

void Function2(void) {
    BA_LOGGER_INFO("Function2\n");
    Function3();
}

void Function1(void) {
    BA_LOGGER_INFO("Function1\n");
    Function2();
}

void Test(void) {
    Function1();
}