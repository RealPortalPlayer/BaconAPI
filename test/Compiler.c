// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Internal/Compiler.h>
#include <BaconAPI/Logger.h>

void Test(void) {
    BA_LOGGER_INFO("Compiler: %s %s (%i)\n"
                   "Standard Version: %i\n", BA_COMPILER_NAME, BA_COMPILER_VERSION_STRING, BA_COMPILER_VERSION, BA_COMPILER_STANDARD_VERSION);
}
